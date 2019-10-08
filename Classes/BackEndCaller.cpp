#include "BackEndCaller.h"

#include <AzoomeeCommon/Data/ConfigStorage.h>
#include <AzoomeeCommon/Data/Child/ChildManager.h>
#include <AzoomeeCommon/Data/Parent/ParentManager.h>
#include <AzoomeeCommon/Data/Cookie/CookieManager.h>
#include <AzoomeeCommon/UI/ModalMessages.h>
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include <AzoomeeCommon/Utils/StringFunctions.h>
#include <AzoomeeCommon/Net/Utils.h>
#include <AzoomeeCommon/API/API.h>
#include <AzoomeeCommon/Utils/SessionIdManager.h>
#include <AzoomeeCommon/ImageDownloader/ImageDownloader.h>
#include <AzoomeeCommon/ErrorCodes.h>
#include "HQHistoryManager.h"
#include "LoginLogicHandler.h"
#include "ChildSelectorScene.h"
#include <AzoomeeCommon/UI/RequestAdultPinLayer.h>
#include "RoutePaymentSingleton.h"
#include "SceneManagerScene.h"
#include "DeepLinkingSingleton.h"
#include "FlowDataSingleton.h"
#include "OfflineHubScene.h"
#include "OfflineChecker.h"
#include "ForceUpdateSingleton.h"
#include "IAPProductDataHandler.h"
#include "ChildCreator.h"

#include "RewardDisplayHandler.h"

#include "MarketingAssetManager.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "ApplePaymentSingleton.h"
#include <AzoomeeCommon/Utils/IosNativeFunctionsSingleton.h>
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "GooglePaymentSingleton.h"
#include "AmazonPaymentSingleton.h"
#include "platform/android/jni/JniHelper.h"
static const std::string kAzoomeeActivityJavaClassName = "org/cocos2dx/cpp/AppActivity";
#endif

using namespace cocos2d;

NS_AZOOMEE_BEGIN


static BackEndCaller *_sharedBackEndCaller = NULL;

BackEndCaller* BackEndCaller::getInstance()
{
    if (! _sharedBackEndCaller)
    {
        _sharedBackEndCaller = new BackEndCaller();
        _sharedBackEndCaller->init();
    }
    
    return _sharedBackEndCaller;
}

BackEndCaller::~BackEndCaller()
{
}

bool BackEndCaller::init()
{
    return true;
}

//---------------------LOADING SCREEN----------------------------------
void BackEndCaller::displayLoadingScreen()
{
    ModalMessages::getInstance()->startLoading();
}

void BackEndCaller::hideLoadingScreen()
{
    ModalMessages::getInstance()->stopLoading();
}

//---------------------ERROR HANDLING----------------------------------

void BackEndCaller::getBackToLoginScreen(long errorCode)
{
    FlowDataSingleton::getInstance()->setErrorCode(errorCode);
    LoginLogicHandler::getInstance()->forceNewLogin();
}

//OFFLINE CHECK

void BackEndCaller::offlineCheck()
{
    HttpRequestCreator* request = API::OfflineCheck(this);
    request->execute();
}

//GETTING IP

void BackEndCaller::ipCheck()
{
    if(ConfigStorage::getInstance()->getClientAnonymousIp() != "0.0.0.0")
    {
        return;
    }
    
    HttpRequestCreator* request = API::IpCheck(this);
    request->execute(3.0f); // 3 second timeout on request
}

//LOGGING IN BY PARENT-------------------------------------------------------------------------------

void BackEndCaller::login(const std::string& username, const std::string& password)
{
    displayLoadingScreen();
    
    HttpRequestCreator* request = API::LoginRequest(username, password, this);
	
	if(password != ConfigStorage::kAnonLoginPW)
	{
    	UserDefault* def = UserDefault::getInstance();
    	def->setStringForKey(ConfigStorage::kStoredUsernameKey, username);
    	def->flush();
	}
    AnalyticsSingleton::getInstance()->registerAzoomeeEmail(username);
    
    request->execute();
}

void BackEndCaller::onLoginAnswerReceived(const std::string& responseString, const std::string& headerString)
{
	IAPProductDataHandler::getInstance()->fetchProductData();
    
    cocos2d::log("Response string is: %s", responseString.c_str());
    if(ParentManager::getInstance()->parseParentLoginData(responseString))
    {
		ParentManager::getInstance()->setLoggedInParentCountryCode(getValueFromHttpResponseHeaderForKey(API::kAZCountryCodeKey, headerString));
		MarketingAssetManager::getInstance()->downloadMarketingAssets();
		if(ParentManager::getInstance()->isLoggedInParentAnonymous())
		{
			AnalyticsSingleton::getInstance()->setIsUserAnonymous(true);
			getAvailableChildren();
		}
		else
		{
			AnalyticsSingleton::getInstance()->setIsUserAnonymous(false);
			AnalyticsSingleton::getInstance()->signInSuccessEvent();

			if(RoutePaymentSingleton::getInstance()->receiptDataFileExists())
			{
				Director::getInstance()->getScheduler()->schedule([&](float dt){
					RoutePaymentSingleton::getInstance()->retryReceiptValidation();
				}, this, 1.0, 0, 0, false, "receiptValidation");
				
			}
			else
			{
				getAvailableChildren();
				updateBillingData();
			}
			getParentDetails();
			ConfigStorage::getInstance()->setFirstSlideShowSeen();
		}
		
		
		
    }
    else
    {
        AnalyticsSingleton::getInstance()->signInFailEvent(0);
        FlowDataSingleton::getInstance()->setErrorCode(ERROR_CODE_INVALID_CREDENTIALS);
        getBackToLoginScreen(ERROR_CODE_INVALID_CREDENTIALS);
    }
}

//LOGGING IN BY DEVICE IDENTIFIER
void BackEndCaller::anonymousDeviceLogin()
{
	UserDefault* userDefault = UserDefault::getInstance();
	const std::string& anonEmail = userDefault->getStringForKey(ConfigStorage::kAnonEmailKey, "");
	
	if(anonEmail == "")
	{
		displayLoadingScreen();
		HttpRequestCreator* request = API::GetAnonCredentials(this);
		request->execute();
	}
	else
	{
		login(anonEmail, ConfigStorage::kAnonLoginPW);
	}
}


//UPDATING BILLING DATA-------------------------------------------------------------------------------

void BackEndCaller::updateBillingData()
{
    ParentManager::getInstance()->setBillingDataAvailable(false);
    HttpRequestCreator* request = API::UpdateBillingDataRequest(ParentManager::getInstance()->getLoggedInParentId(), this);
    request->execute();
}

void BackEndCaller::onUpdateBillingDataAnswerReceived(const std::string& responseString)
{
    ParentManager::getInstance()->parseParentBillingData(responseString);
}

//UPDATING PARENT DATA--------------------------------------------------------------------------------

void BackEndCaller::updateParentPin(AwaitingAdultPinLayer *callBackTo)
{
    displayLoadingScreen();
    
    callBackNode = callBackTo;
    
    HttpRequestCreator* request = API::UpdateParentPinRequest(this);
    request->execute();
}

void BackEndCaller::onUpdateParentPinAnswerReceived(const std::string& responseString)
{
    cocos2d::log("Update parent response string is: %s", responseString.c_str());
    if(ParentManager::getInstance()->parseUpdateParentData(responseString))
    {
        hideLoadingScreen();
        
        RequestAdultPinLayer *checkBack = (RequestAdultPinLayer *)callBackNode;
        cocos2d::log("Calling back awaitingsomething");
        checkBack->checkPinAgainstStoredPin();
    }
}

// GETTING PARENT DATA--------------------------------------------------------------------------------

void BackEndCaller::getParentDetails()
{
    HttpRequestCreator* request = API::getParentDetailsRequest(ParentManager::getInstance()->getLoggedInParentId(), this);
    request->execute();
}

//GETTING AVAILABLE CHILDREN--------------------------------------------------------------------------

void BackEndCaller::getAvailableChildren()
{
    ModalMessages::getInstance()->startLoading();
    
    HttpRequestCreator* request = API::GetAvailableChildrenRequest(this);
    request->execute();
}

void BackEndCaller::onGetChildrenAnswerReceived(const std::string& responseString)
{
    
    ParentManager::getInstance()->parseAvailableChildren(responseString);
    if(ParentManager::getInstance()->getAmountOfAvailableChildren() == 0)
    {
		Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::AddChildFirstTime));
    }
    else
    {
		if(ParentManager::getInstance()->isLoggedInParentAnonymous())
		{
			UserDefault* userDefault = UserDefault::getInstance();
			bool anonOnboardingComplete = userDefault->getBoolForKey("anonOnboardingComplete", false);
			if(!anonOnboardingComplete)
			{
				Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::AddChildAnon));
			}
			else
			{
				childLogin(0); // auto login default child
				return;
			}
		}
		else
		{
            const auto targetScene = (!ParentManager::getInstance()->isPaidUser() && LoginLogicHandler::getInstance()->getOrigin() == LoginOrigin::IAP_PAYWALL) ? SceneNameEnum::IAP : SceneNameEnum::ChildSelector;
            Director::getInstance()->replaceScene(SceneManagerScene::createScene(targetScene));
		}
    }
	ModalMessages::getInstance()->stopLoading();
}

//CHILDREN LOGIN----------------------------------------------------------------------------------------

void BackEndCaller::childLogin(int childNumber)
{
    displayLoadingScreen();
    
    const std::string& profileName = ParentManager::getInstance()->getChild(childNumber)->getProfileName();
    HttpRequestCreator* request = API::ChildLoginRequest(profileName, this);
    request->execute();
}

void BackEndCaller::onChildLoginAnswerReceived(const std::string& responseString, const std::string& headerString)
{
    if((!ParentManager::getInstance()->parseChildLoginData(responseString)))
    {
        LoginLogicHandler::getInstance()->doLoginLogic();
        return;
    }
    ParentManager::getInstance()->setLoggedInParentCountryCode(getValueFromHttpResponseHeaderForKey(API::kAZCountryCodeKey, headerString));
	getChildInventory();
    getGordon();
	HQHistoryManager::getInstance()->emptyHistory();
}

//GETTING GORDON.PNG-------------------------------------------------------------------------------------

void BackEndCaller::getGordon()
{
    
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    IosNativeFunctionsSingleton::getInstance()->deleteHttpCookies(); //ios handles cookies on OS level. Removal of earlier cookies is important to avoid watching premium content with a free user.
#endif
    
    const std::string& userId = ChildManager::getInstance()->getParentOrChildId();
    const std::string& sessionId = ChildManager::getInstance()->getParentOrChildCdnSessionId();
    
    HttpRequestCreator* request = API::GetGordenRequest(userId, sessionId, this);
    request->execute();
}

void BackEndCaller::onGetGordonAnswerReceived(const std::string& responseString)
{
    if(CookieManager::getInstance()->parseDownloadCookies(responseString))
    {
		ContentItemPoolDownloadHandler::getInstance()->getLatestData([](bool success){ //on complete
			if(success)
			{
				HQStructureDownloadHandler::getInstance()->getLatestData([](bool success){ //on complete
					if(success)
					{
						RewardDisplayHandler::getInstance()->getPendingRewards();
                        
                        SceneNameEnum nextScene = SceneNameEnum::Base;
                        
                        if(ParentManager::getInstance()->isLoggedInParentAnonymous())
                        {
                            if(LoginLogicHandler::getInstance()->getOrigin() == LoginOrigin::IAP_PAYWALL)
                            {
                                nextScene = SceneNameEnum::IAP;
                            }
                            else if(LoginLogicHandler::getInstance()->getOrigin() == LoginOrigin::SIGNUP)
                            {
                                nextScene = SceneNameEnum::Signup;
                            }
                        }
                        Director::getInstance()->replaceScene(SceneManagerScene::createScene(nextScene));
					}
					else
					{
						LoginLogicHandler::getInstance()->doLoginLogic();
					}
				});
			}
			else
			{
				LoginLogicHandler::getInstance()->doLoginLogic();
			}
		});
    }
}

//REGISTER PARENT---------------------------------------------------------------------------

void BackEndCaller::registerParent(const std::string& emailAddress, const std::string& password, const std::string& pinNumber, const std::string& marketingAccepted)
{
    FlowDataSingleton::getInstance()->setFlowToSignup(emailAddress, password);
    const std::string &sourceDevice = ConfigStorage::getInstance()->getDeviceInformation();
    
    std::string source = "OTHER";
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    source = "IOS_INAPP";
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    source = "ANDROID_INAPP";
#endif
    
	HttpRequestCreator* request = API::RegisterParentRequest(ParentManager::getInstance()->getLoggedInParentId(),emailAddress, password, pinNumber, source, sourceDevice, marketingAccepted, this);
    request->execute();
    
    displayLoadingScreen();
}

void BackEndCaller::onRegisterParentAnswerReceived()
{
	IAPProductDataHandler::getInstance()->fetchProductData();
	UserDefault* userDefault = UserDefault::getInstance();
	userDefault->setBoolForKey(ConfigStorage::kAnonOnboardingCompleteKey, false);
	userDefault->setStringForKey(ConfigStorage::kAnonEmailKey, "");
    ConfigStorage::getInstance()->setFirstSlideShowSeen();
    AnalyticsSingleton::getInstance()->OnboardingAccountCreatedEvent();
    FlowDataSingleton::getInstance()->setSuccessFailPath(SIGNUP_SUCCESS);
    login(FlowDataSingleton::getInstance()->getUserName(), FlowDataSingleton::getInstance()->getPassword());
}

//REGISTER CHILD----------------------------------------------------------------------------

void BackEndCaller::registerChild(const std::string& childProfileName, const std::string& childGender, const std::string& childDOB, int oomeeNumber)
{
    displayLoadingScreen();
    
    FlowDataSingleton::getInstance()->addChildData(childProfileName, oomeeNumber);
    
    const std::string& oomeeUrl = ConfigStorage::getInstance()->getUrlForOomee(oomeeNumber);
    HttpRequestCreator* request = API::RegisterChildRequest(childProfileName, childGender, childDOB, oomeeUrl, this);
    request->execute();
}

void BackEndCaller::onRegisterChildAnswerReceived()
{
    AnalyticsSingleton::getInstance()->childProfileCreatedSuccessEvent();
    getAvailableChildren();
}

//UPDATE CHILD----------------------------------------------------------------------------

void BackEndCaller::updateChild(const std::string& childId, const std::string& childProfileName, const std::string& childGender, const std::string& childDOB, int oomeeNumber)
{
    displayLoadingScreen();
    
    FlowDataSingleton::getInstance()->addChildData(childProfileName, oomeeNumber);
    
    const std::string& oomeeUrl = ConfigStorage::getInstance()->getUrlForOomee(oomeeNumber);
    const std::string& ownerId = ParentManager::getInstance()->getLoggedInParentId();
    HttpRequestCreator* request = API::UpdateChildRequest(childId, childProfileName, childGender, childDOB, oomeeUrl, ownerId, this);
    request->execute();
}

void BackEndCaller::onUpdateChildAnswerReceived()
{
    getAvailableChildren();
}

void BackEndCaller::updateChildAvatar(const std::string &childId, const std::string &imageData)
{
    displayLoadingScreen();
    
    HttpRequestCreator* request = API::UpdateChildAvatar(childId, imageData, this);
    request->execute();
}

//GOOGLE VERIFY PAYMENT---------------------------------------------------------------------
void BackEndCaller::verifyGooglePayment(const std::string& orderId, const std::string& iapSku, const std::string& purchaseToken)
{
    HttpRequestCreator* request = API::VerifyGooglePaymentRequest(orderId, iapSku, purchaseToken, this);
    request->execute(30.0f);
}

//AMAZON VERIFY PAYMENT---------------------------------------------------------------------
void BackEndCaller::verifyAmazonPayment(const std::string& requestId, const std::string& receiptId, const std::string& amazonUserid)
{
    HttpRequestCreator* request = API::VerifyAmazonPaymentRequest(requestId, receiptId, amazonUserid, this);
    request->execute(30.0f);
}

//APPLE VERIFY PAYMENT----------------------------------------------------------------------
void BackEndCaller::verifyApplePayment(const std::string& receiptData, const std::string& transactionID)
{
    HttpRequestCreator* request = API::VerifyApplePaymentRequest(receiptData, transactionID, this);
    request->execute(30.0f);
}

//GET CONTENT-------------------------------------------------------------------------------
void BackEndCaller::getHQContent(const std::string& url, const std::string& category)
{
        HttpRequestCreator* request = API::GetEncryptedContentRequest(url, category, this);
        request->execute();
}

// DEEPLINK CONTENT DETAILS REQUEST ----------------------------------------------------------------
void BackEndCaller::GetContent(const std::string& requestId, const std::string& contentID)
{
    if(ChildManager::getInstance()->isChildLoggedIn())
    {
        HttpRequestCreator* request = API::GetContent(requestId, ChildManager::getInstance()->getLoggedInChild()->getId(), contentID, this);
        request->execute();
    }
}

// RESET PASSWORD REQUEST ----------------------------------------------------------------
void BackEndCaller::resetPasswordRequest(const std::string& emailAddress)
{
    HttpRequestCreator* request = API::ResetPaswordRequest(Net::urlEncode(stringToLower(emailAddress)), this);
    request->execute();
}

void BackEndCaller::getChildInventory()
{
	const ChildRef& child = ChildManager::getInstance()->getLoggedInChild();
	if(child)
	{
		HttpRequestCreator* request = API::GetInventory(child->getId(), this);
		request->execute();
	}
}


//HttpRequestCreatorResponseDelegate--------------------------------------------------------
void BackEndCaller::onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body)
{
    if(requestTag == API::TagIpCheck)
    {
        ConfigStorage::getInstance()->setClientAnonymousIp(body);
        AnalyticsSingleton::getInstance()->registerAnonymousIp(ConfigStorage::getInstance()->getClientAnonymousIp());
    }
    else if(requestTag == API::TagGetGorden)
    {
        onGetGordonAnswerReceived(headers);
    }
    else if(requestTag == API::TagChildLogin)
    {
        onChildLoginAnswerReceived(body, headers);
    }
	else if(requestTag == API::TagGetInventory)
	{
		ChildManager::getInstance()->parseChildInventory(body);
	}
    else if(requestTag == API::TagGetAvailableChildren)
    {
        onGetChildrenAnswerReceived(body);
    }
    else if(requestTag == API::TagLogin)
    {
        onLoginAnswerReceived(body, headers);
    }
	else if(requestTag == API::TagGetAnonCredentials)
	{
		rapidjson::Document json;
		json.Parse(body.c_str());
		const std::string& userId = getStringFromJson("id", json);
		ParentManager::getInstance()->saveAnonCredentialsToDevice(userId);
		login(userId, ConfigStorage::kAnonLoginPW);
	}
    else if(requestTag == API::TagRegisterChild)
    {
        onRegisterChildAnswerReceived();
    }
    else if(requestTag == API::TagUpdateChild)
    {
        onUpdateChildAnswerReceived();
    }
    else if(requestTag == API::TagUpdateChildAvatar)
    {
        rapidjson::Document json;
        json.Parse(body.c_str());
		ChildManager::getInstance()->parseAvatarUpdate(body);
        ImageDownloaderRef imageDownloader = ImageDownloader::create("imageCache/", ImageDownloader::CacheMode::File );
        imageDownloader->downloadImage(nullptr, getStringFromJson("avatar", json), true);
        hideLoadingScreen();
    }
    else if(requestTag == API::TagRegisterParent)
    {
        onRegisterParentAnswerReceived();
    }
    else if(requestTag == API::TagParentPin)
    {
        onUpdateParentPinAnswerReceived(body);
    }
    else if(requestTag == API::TagGetParentDetails)
    {
        ParentManager::getInstance()->parseParentDetails(body);
    }
    else if(requestTag == "deepLinkContentRequest")
    {
        DeepLinkingSingleton::getInstance()->contentDetailsResponse(body);
    }
    else if(requestTag == API::TagUpdateBillingData)
    {
        onUpdateBillingDataAnswerReceived(body);
    }
    else if(requestTag == API::TagOfflineCheck)
    {
        OfflineChecker::getInstance()->onOfflineCheckAnswerReceived();
        ipCheck();
    }
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    else if(requestTag == API::TagVerifyApplePayment)
    {
        ApplePaymentSingleton::getInstance()->onAnswerReceived(body);
    }
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    else if(requestTag == API::TagVerifyGooglePayment)
    {
        GooglePaymentSingleton::getInstance()->onGooglePaymentVerificationAnswerReceived(body);
    }
    else if(requestTag == API::TagVerifyAmazonPayment)
    {
        AmazonPaymentSingleton::getInstance()->onAmazonPaymentMadeAnswerReceived(body);
    }
#endif
    else if(requestTag == API::TagResetPasswordRequest)
        //Dont do anything with a password Request attempt
        return;
    else
    {
		
    }
}

void BackEndCaller::onHttpRequestFailed(const std::string& requestTag, long errorCode)
{
    if(requestTag == API::TagIpCheck)
    {
        ConfigStorage::getInstance()->setClientAnonymousIp("0.0.0.0");
        AnalyticsSingleton::getInstance()->registerAnonymousIp(ConfigStorage::getInstance()->getClientAnonymousIp());
    }
    else if(requestTag == API::TagOfflineCheck)
    {
        OfflineChecker::getInstance()->onOfflineCheckFailed();
    }
    else if(requestTag == API::TagRegisterParent)
    {
        AnalyticsSingleton::getInstance()->OnboardingAccountCreatedErrorEvent(errorCode);
        hideLoadingScreen();
        FlowDataSingleton::getInstance()->setErrorCode(errorCode);
		if(errorCode == ERROR_CODE_ALREADY_REGISTERED)
		{
			LoginLogicHandler::getInstance()->forceNewLogin();
		}
		else
		{
        	MessageBox::createWith(errorCode, nullptr);
		}
    }
    else if(requestTag == API::TagRegisterChild)
    {
        AnalyticsSingleton::getInstance()->childProfileCreatedErrorEvent(errorCode);
        hideLoadingScreen();
        FlowDataSingleton::getInstance()->setErrorCode(errorCode);
    }
    else if(requestTag == API::TagUpdateChild)
    {
        AnalyticsSingleton::getInstance()->childProfileUpdateErrorEvent(errorCode);
        hideLoadingScreen();
        FlowDataSingleton::getInstance()->setErrorCode(errorCode);
    }
    else if(requestTag == API::TagLogin)
    {
        FlowDataSingleton::getInstance()->setErrorCode(errorCode);
        LoginLogicHandler::getInstance()->forceNewLogin();
    }
    else if(requestTag == API::TagGetAvailableChildren)
    {

        FlowDataSingleton::getInstance()->setErrorCode(errorCode);
        if(errorCode == ERROR_CODE_OFFLINE)
        {
            Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::OfflineHub));
            return;
        }
        
        LoginLogicHandler::getInstance()->forceNewLogin();
    }
    else if(requestTag == API::TagVerifyApplePayment || requestTag == API::TagVerifyAmazonPayment || requestTag == API::TagVerifyGooglePayment)
    {
        cocos2d::log("IAP Failed with Errorcode: %ld", errorCode);
        AnalyticsSingleton::getInstance()->iapBackEndRequestFailedEvent(errorCode);
        RoutePaymentSingleton::getInstance()->backendRequestFailed(errorCode);
    }
	else if(requestTag == API::TagResetPasswordRequest || requestTag == API::TagUpdateBillingData || requestTag == API::TagGetForceUpdateInformation || requestTag == API::TagGetParentDetails || requestTag == API::TagGetInventory) //Dont do anything with a password Request attempt
    {
        return;
    }
    else
    {
		if(errorCode != ERROR_CODE_OFFLINE)
		{
        	FlowDataSingleton::getInstance()->setErrorCode(errorCode);
        	LoginLogicHandler::getInstance()->doLoginLogic();
		}
    }
}

NS_AZOOMEE_END
