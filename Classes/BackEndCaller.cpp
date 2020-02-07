#include "BackEndCaller.h"

#include <AzoomeeCommon/Data/Child/ChildManager.h>
#include <AzoomeeCommon/Data/Parent/UserAccountManager.h>
#include "RewardManager.h"
#include <AzoomeeCommon/Data/Cookie/CookieManager.h>
#include <AzoomeeCommon/UI/ModalMessages.h>
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include <AzoomeeCommon/Utils/StringFunctions.h>
#include <AzoomeeCommon/Net/Utils.h>
#include <AzoomeeCommon/API/API.h>
#include <AzoomeeCommon/Utils/SessionIdManager.h>
#include <AzoomeeCommon/ImageDownloader/ImageDownloader.h>
#include "ErrorCodes.h"
#include "HQHistoryManager.h"
#include "LoginController.h"
#include "ChildSelectorScene.h"
#include "RequestAdultPinLayer.h"
#include "RoutePaymentSingleton.h"
#include "SceneManagerScene.h"
#include "DeepLinkingSingleton.h"
#include "FlowDataSingleton.h"
#include "OfflineChecker.h"
#include "ForceUpdateSingleton.h"
#include "IAPProductDataHandler.h"
#include "ChildCreator.h"
#include "PopupMessageBox.h"

#include "MarketingAssetManager.h"

#include "AzoomeeOomeeMaker/DataObjects/OomeeMakerDataHandler.h"
#include <AzoomeeCommon/Device.h>
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
    LoginController::getInstance()->forceNewLogin();
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
    if(Device::getInstance()->getClientAnonymousIp() != "0.0.0.0")
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
	
	if(password != UserAccountManager::kAnonLoginPW)
	{
    	UserDefault* def = UserDefault::getInstance();
    	def->setStringForKey(UserAccountManager::kStoredUsernameKey, username);
    	def->flush();
	}
    AnalyticsSingleton::getInstance()->registerAzoomeeEmail(username);
    
    request->execute();
}

void BackEndCaller::onLoginAnswerReceived(const std::string& responseString, const std::string& headerString)
{
	IAPProductDataHandler::getInstance()->fetchProductData();
    
    cocos2d::log("Response string is: %s", responseString.c_str());
    if(UserAccountManager::getInstance()->parseParentLoginData(responseString))
    {
		UserAccountManager::getInstance()->setLoggedInParentCountryCode(StringFunctions::getValueFromHttpResponseHeaderForKey(API::kAZCountryCodeKey, headerString));
		MarketingAssetManager::getInstance()->downloadMarketingAssets();
        OomeeMaker::OomeeMakerDataHandler::getInstance()->getLatestDataAsync();
		if(UserAccountManager::getInstance()->isLoggedInParentAnonymous())
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
			UserAccountManager::getInstance()->setHasLoggedInOnDevice(true);
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
	const std::string& anonEmail = userDefault->getStringForKey(UserAccountManager::kAnonEmailKey, "");
	
	if(anonEmail == "")
	{
		displayLoadingScreen();
		HttpRequestCreator* request = API::GetAnonCredentials(this);
		request->execute();
	}
	else
	{
		login(anonEmail, UserAccountManager::kAnonLoginPW);
	}
}


//UPDATING BILLING DATA-------------------------------------------------------------------------------

void BackEndCaller::updateBillingData()
{
    UserAccountManager::getInstance()->setBillingDataAvailable(false);
    HttpRequestCreator* request = API::UpdateBillingDataRequest(UserAccountManager::getInstance()->getLoggedInParentId(), this);
    request->execute();
}

void BackEndCaller::onUpdateBillingDataAnswerReceived(const std::string& responseString)
{
    UserAccountManager::getInstance()->parseParentBillingData(responseString);
}

//UPDATING PARENT DATA--------------------------------------------------------------------------------

void BackEndCaller::updateParentPin(AwaitingAdultPinLayer *callBackTo)
{
    displayLoadingScreen();
    
    callBackNode = callBackTo;
    
    HttpRequestCreator* request = API::UpdateParentPinRequest(UserAccountManager::getInstance()->getLoggedInParentId(), this);
    request->execute();
}

void BackEndCaller::onUpdateParentPinAnswerReceived(const std::string& responseString)
{
    cocos2d::log("Update parent response string is: %s", responseString.c_str());
    if(UserAccountManager::getInstance()->parseUpdateParentData(responseString))
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
    HttpRequestCreator* request = API::getParentDetailsRequest(UserAccountManager::getInstance()->getLoggedInParentId(), this);
    request->execute();
}

//GETTING AVAILABLE CHILDREN--------------------------------------------------------------------------

void BackEndCaller::getAvailableChildren()
{
    ModalMessages::getInstance()->startLoading();
    
    HttpRequestCreator* request = API::GetAvailableChildrenRequest(UserAccountManager::getInstance()->getLoggedInParentId(), this);
    request->execute();
}

void BackEndCaller::onGetChildrenAnswerReceived(const std::string& responseString)
{
    
    UserAccountManager::getInstance()->parseAvailableChildren(responseString);
    if(UserAccountManager::getInstance()->getAmountOfAvailableChildren() == 0)
    {
		Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::AddChild));
    }
    else
    {
		if(UserAccountManager::getInstance()->isLoggedInParentAnonymous())
		{
			UserDefault* userDefault = UserDefault::getInstance();
			bool anonOnboardingComplete = userDefault->getBoolForKey("anonOnboardingComplete", false);
			if(!anonOnboardingComplete)
			{
				Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::AddChild));
			}
			else
			{
				childLogin(0); // auto login default child
				return;
			}
		}
		else
		{
            const auto targetScene = (!UserAccountManager::getInstance()->isPaidUser() && LoginController::getInstance()->getOrigin() == LoginOrigin::IAP_PAYWALL) ? SceneNameEnum::IAP : SceneNameEnum::ChildSelector;
            Director::getInstance()->replaceScene(SceneManagerScene::createScene(targetScene));
		}
    }
	ModalMessages::getInstance()->stopLoading();
}

//CHILDREN LOGIN----------------------------------------------------------------------------------------

void BackEndCaller::childLogin(int childNumber)
{
    displayLoadingScreen();
    
    const std::string& profileName = UserAccountManager::getInstance()->getChild(childNumber)->getProfileName();
    HttpRequestCreator* request = API::ChildLoginRequest(profileName, this);
    request->execute();
}

void BackEndCaller::onChildLoginAnswerReceived(const std::string& responseString, const std::string& headerString)
{
    if((!UserAccountManager::getInstance()->parseChildLoginData(responseString)))
    {
        LoginController::getInstance()->doLoginLogic();
        return;
    }
    UserAccountManager::getInstance()->setLoggedInParentCountryCode(StringFunctions::getValueFromHttpResponseHeaderForKey(API::kAZCountryCodeKey, headerString));
	getChildInventory();
    OomeeMaker::OomeeMakerDataHandler::getInstance()->getOomeesForChild(ChildManager::getInstance()->getLoggedInChild()->getId(), false);
    getSessionCookies();
	HQHistoryManager::getInstance()->emptyHistory();
    
    // Update rewards feed
    RewardManager::getInstance()->getLatestRewardStrategy();
    RewardManager::getInstance()->checkForPendingRewards();
}

// - getSessionCookies

void BackEndCaller::getSessionCookies()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    IosNativeFunctionsSingleton::getInstance()->deleteHttpCookies(); //ios handles cookies on OS level. Removal of earlier cookies is important to avoid watching premium content with a free user.
#endif
    
    const std::string& userId = ChildManager::getInstance()->getLoggedInChild()->getId();
    const std::string& sessionId = ChildManager::getInstance()->getLoggedInChild()->getCDNSessionId();
    
    HttpRequestCreator* request = API::GetSessionCookiesRequest(userId, sessionId, false, this);
    request->execute();
}

void BackEndCaller::onSessionCookiesAnswerReceived(const std::string& responseString)
{
    if(CookieManager::getInstance()->parseDownloadCookies(responseString))
    {
		ContentItemPoolDownloadHandler::getInstance()->getLatestData([](bool success){ //on complete
			if(success)
			{
				HQStructureDownloadHandler::getInstance()->getLatestData([](bool success){ //on complete
					if(success)
					{
                        SceneNameEnum nextScene = SceneNameEnum::Base;
                        
                        if(UserAccountManager::getInstance()->isLoggedInParentAnonymous())
                        {
                            if(LoginController::getInstance()->getOrigin() == LoginOrigin::IAP_PAYWALL)
                            {
                                nextScene = SceneNameEnum::IAP;
                            }
                            else if(LoginController::getInstance()->getOrigin() == LoginOrigin::SIGNUP)
                            {
                                nextScene = SceneNameEnum::Signup;
                            }
                        }
                        Director::getInstance()->replaceScene(SceneManagerScene::createScene(nextScene));
					}
					else
					{
						LoginController::getInstance()->doLoginLogic();
					}
				});
			}
			else
			{
				LoginController::getInstance()->doLoginLogic();
			}
		});
    }
}

//REGISTER PARENT---------------------------------------------------------------------------

void BackEndCaller::registerParent(const std::string& emailAddress, const std::string& password, const std::string& pinNumber, const std::string& marketingAccepted)
{
    FlowDataSingleton::getInstance()->setFlowToSignup(emailAddress, password);
    const std::string &sourceDevice = Device::getInstance()->getDeviceInformation();
    
    std::string source = "OTHER";
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    source = "IOS_INAPP";
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    source = "ANDROID_INAPP";
#endif
    
	HttpRequestCreator* request = API::RegisterParentRequest(UserAccountManager::getInstance()->getLoggedInParentId(),emailAddress, password, pinNumber, source, sourceDevice, marketingAccepted, this);
    request->execute();
    
    displayLoadingScreen();
}

void BackEndCaller::onRegisterParentAnswerReceived()
{
	IAPProductDataHandler::getInstance()->fetchProductData();
	UserDefault* userDefault = UserDefault::getInstance();
	userDefault->setBoolForKey(UserAccountManager::kAnonOnboardingCompleteKey, false);
	userDefault->setStringForKey(UserAccountManager::kAnonEmailKey, "");
    UserAccountManager::getInstance()->setHasLoggedInOnDevice(true);
    AnalyticsSingleton::getInstance()->OnboardingAccountCreatedEvent();
    FlowDataSingleton::getInstance()->setSuccessFailPath(SIGNUP_SUCCESS);
    login(FlowDataSingleton::getInstance()->getUserName(), FlowDataSingleton::getInstance()->getPassword());
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
    HttpRequestCreator* request = API::VerifyGooglePaymentRequest(UserAccountManager::getInstance()->getLoggedInParentId(), orderId, iapSku, purchaseToken, this);
    request->execute(30.0f);
}

//AMAZON VERIFY PAYMENT---------------------------------------------------------------------
void BackEndCaller::verifyAmazonPayment(const std::string& requestId, const std::string& receiptId, const std::string& amazonUserid)
{
    HttpRequestCreator* request = API::VerifyAmazonPaymentRequest(UserAccountManager::getInstance()->getLoggedInParentId(), requestId, receiptId, amazonUserid, this);
    request->execute(30.0f);
}

//APPLE VERIFY PAYMENT----------------------------------------------------------------------
void BackEndCaller::verifyApplePayment(const std::string& receiptData, const std::string& transactionID)
{
    HttpRequestCreator* request = API::VerifyApplePaymentRequest(UserAccountManager::getInstance()->getLoggedInParentId(), receiptData, transactionID, this);
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
    HttpRequestCreator* request = API::ResetPaswordRequest(Net::urlEncode(StringFunctions::stringToLower(emailAddress)), this);
    request->execute();
}

void BackEndCaller::getChildInventory()
{
	const ChildRef& child = ChildManager::getInstance()->getLoggedInChild();
	if(child)
	{
        ChildManager::getInstance()->updateInventory();
	}
}


//HttpRequestCreatorResponseDelegate--------------------------------------------------------
void BackEndCaller::onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body)
{
    if(requestTag == API::TagIpCheck)
    {
        Device::getInstance()->setClientAnonymousIp(body);
        AnalyticsSingleton::getInstance()->registerAnonymousIp(Device::getInstance()->getClientAnonymousIp());
    }
    else if(requestTag == API::TagGetSessionCookies)
    {
        onSessionCookiesAnswerReceived(headers);
    }
    else if(requestTag == API::TagChildLogin)
    {
        onChildLoginAnswerReceived(body, headers);
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
		UserAccountManager::getInstance()->saveAnonCredentialsToDevice(userId);
		login(userId, UserAccountManager::kAnonLoginPW);
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
        UserAccountManager::getInstance()->parseParentDetails(body);
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
        Device::getInstance()->setClientAnonymousIp("0.0.0.0");
        AnalyticsSingleton::getInstance()->registerAnonymousIp(Device::getInstance()->getClientAnonymousIp());
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
			LoginController::getInstance()->forceNewLogin();
		}
		else
		{
        	const auto& errorMessageText = LocaleManager::getInstance()->getErrorMessageWithCode(errorCode);
                
            PopupMessageBox* messageBox = PopupMessageBox::create();
            messageBox->setTitle(errorMessageText.at(ERROR_TITLE));
            messageBox->setBody(errorMessageText.at(ERROR_BODY));
            messageBox->setButtonText(_("Back"));
            messageBox->setButtonColour(Style::Color::darkIndigo);
            messageBox->setPatternColour(Style::Color::azure);
            messageBox->setButtonPressedCallback([this](MessagePopupBase* pSender){
                pSender->removeFromParent();
            });
            Director::getInstance()->getRunningScene()->addChild(messageBox, 1);
		}
    }
    else if(requestTag == API::TagLogin)
    {
        FlowDataSingleton::getInstance()->setErrorCode(errorCode);
        LoginController::getInstance()->forceNewLogin();
    }
    else if(requestTag == API::TagGetAvailableChildren)
    {

        FlowDataSingleton::getInstance()->setErrorCode(errorCode);
        if(errorCode == ERROR_CODE_OFFLINE)
        {
            Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::OfflineHub));
            return;
        }
        
        LoginController::getInstance()->forceNewLogin();
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
        	LoginController::getInstance()->doLoginLogic();
		}
    }
}

NS_AZOOMEE_END
