#include "BackEndCaller.h"

#include <AzoomeeCommon/Data/ConfigStorage.h>
#include <AzoomeeCommon/Data/Child/ChildDataParser.h>
#include <AzoomeeCommon/Data/Child/ChildDataProvider.h>
#include <AzoomeeCommon/Data/Child/ChildDataStorage.h>
#include <AzoomeeCommon/Data/Parent/ParentDataParser.h>
#include <AzoomeeCommon/Data/Parent/ParentDataProvider.h>
#include <AzoomeeCommon/Data/Cookie/CookieDataParser.h>
#include <AzoomeeCommon/UI/ModalMessages.h>
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include <AzoomeeCommon/Utils/StringFunctions.h>
#include <AzoomeeCommon/Net/Utils.h>
#include <AzoomeeCommon/API/API.h>
#include <AzoomeeCommon/Utils/SessionIdManager.h>
#include "HQDataParser.h"
#include "HQHistoryManager.h"
#include "LoginLogicHandler.h"
#include "ChildSelectorScene.h"
#include "BaseScene.h"
#include <AzoomeeCommon/UI/RequestAdultPinLayer.h>
#include "RoutePaymentSingleton.h"
#include "SceneManagerScene.h"
#include "DeepLinkingSingleton.h"
#include "FlowDataSingleton.h"
#include "OfflineHubScene.h"
#include "OfflineChecker.h"
#include "ForceUpdateSingleton.h"
#include "IAPProductDataHandler.h"

#include "DynamicNodeHandler.h"

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
    request->execute();
}

//LOGGING IN BY PARENT-------------------------------------------------------------------------------

void BackEndCaller::login(const std::string& username, const std::string& password)
{
    displayLoadingScreen();
    
    HttpRequestCreator* request = API::LoginRequest(username, password, this);
    
    UserDefault* def = UserDefault::getInstance();
    def->setStringForKey("username", username);
    def->flush();
    
    AnalyticsSingleton::getInstance()->registerAzoomeeEmail(username);
    
    request->execute();
}

void BackEndCaller::onLoginAnswerReceived(const std::string& responseString, const std::string& headerString)
{
    IAPProductDataHandler::getInstance()->fetchProductData();
    
    cocos2d::log("Response string is: %s", responseString.c_str());
    if(ParentDataParser::getInstance()->parseParentLoginData(responseString))
    {
        ConfigStorage::getInstance()->setFirstSlideShowSeen();
        ParentDataParser::getInstance()->setLoggedInParentCountryCode(getValueFromHttpResponseHeaderForKey("X-AZ-COUNTRYCODE", headerString));
        AnalyticsSingleton::getInstance()->signInSuccessEvent();
        AnalyticsSingleton::getInstance()->setIsUserAnonymous(false);
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
    displayLoadingScreen();
    
    std::string deviceId = ConfigStorage::getInstance()->getDeviceAdvertisingId();
    
    if(deviceId == "") deviceId = "SESSID:" + SessionIdManager::getInstance()->getCurrentSessionId();
    
    HttpRequestCreator* request = API::AnonymousDeviceLoginRequest(deviceId, this);
    request->execute();
}

void BackEndCaller::onAnonymousDeviceLoginAnswerReceived(const std::string &responseString, const std::string& headerString)
{
    IAPProductDataHandler::getInstance()->fetchProductData();
    
    cocos2d::log("Response string is: %s", responseString.c_str());
    if(ParentDataParser::getInstance()->parseParentLoginDataFromAnonymousDeviceLogin(responseString))
    {
        AnalyticsSingleton::getInstance()->setIsUserAnonymous(true);
        ParentDataParser::getInstance()->setLoggedInParentCountryCode(getValueFromHttpResponseHeaderForKey("X-AZ-COUNTRYCODE", headerString));
        HQDataParser::getInstance()->parseHQGetContentUrls(responseString);
        DynamicNodeHandler::getInstance()->getCTAFiles();
        getGordon();
    }
    else
    {
        AnalyticsSingleton::getInstance()->signInFailEvent(0);
        FlowDataSingleton::getInstance()->setErrorCode(ERROR_CODE_INVALID_CREDENTIALS);
        getBackToLoginScreen(ERROR_CODE_INVALID_CREDENTIALS);
    }
}

//UPDATING BILLING DATA-------------------------------------------------------------------------------

void BackEndCaller::updateBillingData()
{
    ParentDataParser::getInstance()->setBillingDataAvailable(false);
    HttpRequestCreator* request = API::UpdateBillingDataRequest(this);
    request->execute();
}

void BackEndCaller::onUpdateBillingDataAnswerReceived(const std::string& responseString)
{
    ParentDataParser::getInstance()->parseParentBillingData(responseString);
    // fire event to add parent button to child select scene if paid account
    if(Director::getInstance()->getRunningScene()->getName() == ChildSelectorScene::kSceneName)
    {
        if(ParentDataProvider::getInstance()->isPaidUser())
        {
            ChildSelectorScene* childSelectScene = dynamic_cast<ChildSelectorScene*>(Director::getInstance()->getRunningScene()->getChildByName(ChildSelectorScene::kSceneName));
            if(childSelectScene)
            {
                childSelectScene->setParentButtonVisible(true);
            }
        }
    }
}

//GETTING FORCE UPDATE INFORMATION

void BackEndCaller::getForceUpdateData()
{
    HttpRequestCreator* request = API::GetForceUpdateInformationRequest(this);
    request->execute();
}

void BackEndCaller::onGetForceUpdateDataAnswerReceived(const std::string& responseString)
{
    ForceUpdateSingleton::getInstance()->onForceUpdateDataReceived(responseString);
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
    if(ParentDataParser::getInstance()->parseUpdateParentData(responseString))
    {
        hideLoadingScreen();
        
        RequestAdultPinLayer *checkBack = (RequestAdultPinLayer *)callBackNode;
        cocos2d::log("Calling back awaitingsomething");
        checkBack->checkPinAgainstStoredPin();
    }
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
    ModalMessages::getInstance()->stopLoading();
    AnalyticsSingleton::getInstance()->registerIdentifier(ParentDataProvider::getInstance()->getLoggedInParentId());
    ParentDataParser::getInstance()->parseAvailableChildren(responseString);
    Director::getInstance()->replaceScene(SceneManagerScene::createScene(ChildSelector));
    
    Director::getInstance()->getScheduler()->schedule([&](float dt){
        DynamicNodeHandler::getInstance()->handleSuccessFailEvent();
    }, this, 0.5, 0, 0, false, "eventHandler");
}

//CHILDREN LOGIN----------------------------------------------------------------------------------------

void BackEndCaller::childLogin(int childNumber)
{
    displayLoadingScreen();
    
    const std::string& profileName = ParentDataProvider::getInstance()->getProfileNameForAnAvailableChild(childNumber);
    HttpRequestCreator* request = API::ChildLoginRequest(profileName, this);
    request->execute();
    
    ChildDataParser::getInstance()->setLoggedInChildName(ParentDataProvider::getInstance()->getProfileNameForAnAvailableChild(childNumber));
    ChildDataParser::getInstance()->setLoggedInChildNumber(childNumber);
}

void BackEndCaller::onChildLoginAnswerReceived(const std::string& responseString, const std::string& headerString)
{
    if((!ChildDataParser::getInstance()->parseChildLoginData(responseString)) || (!HQDataParser::getInstance()->parseHQGetContentUrls(responseString)))
    {
        LoginLogicHandler::getInstance()->doLoginLogic();
    }
    
    ParentDataParser::getInstance()->setLoggedInParentCountryCode(getValueFromHttpResponseHeaderForKey("X-AZ-COUNTRYCODE", headerString));
    DynamicNodeHandler::getInstance()->getCTAFiles();
    getGordon();
}

//GETTING GORDON.PNG-------------------------------------------------------------------------------------

void BackEndCaller::getGordon()
{
    
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    IosNativeFunctionsSingleton::getInstance()->deleteHttpCookies(); //ios handles cookies on OS level. Removal of earlier cookies is important to avoid watching premium content with a free user.
#endif
    
    const std::string& userId = ChildDataProvider::getInstance()->getParentOrChildId();
    const std::string& sessionId = ChildDataProvider::getInstance()->getParentOrChildCdnSessionId();
    
    HttpRequestCreator* request = API::GetGordenRequest(userId, sessionId, this);
    request->execute();
}

void BackEndCaller::onGetGordonAnswerReceived(const std::string& responseString)
{
    if(CookieDataParser::getInstance()->parseDownloadCookies(responseString))
    {
        Director::getInstance()->replaceScene(SceneManagerScene::createScene(BaseWithNoHistory));
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
    
    HttpRequestCreator* request = API::RegisterParentRequest(emailAddress, password, pinNumber, source, sourceDevice, marketingAccepted, this);
    request->execute();
    
    displayLoadingScreen();
}

void BackEndCaller::onRegisterParentAnswerReceived()
{
    IAPProductDataHandler::getInstance()->fetchProductData();
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
    AnalyticsSingleton::getInstance()->childProfileCreatedSuccessEvent(FlowDataSingleton::getInstance()->getOomeeColourNumber());
    getAvailableChildren();
}

//UPDATE CHILD----------------------------------------------------------------------------

void BackEndCaller::updateChild(const std::string& childId, const std::string& childProfileName, const std::string& childGender, const std::string& childDOB, int oomeeNumber)
{
    displayLoadingScreen();
    
    FlowDataSingleton::getInstance()->addChildData(childProfileName, oomeeNumber);
    
    const std::string& oomeeUrl = ConfigStorage::getInstance()->getUrlForOomee(oomeeNumber);
    const std::string& ownerId = ParentDataProvider::getInstance()->getLoggedInParentId();
    const std::string& url = ConfigStorage::getInstance()->getServerUrl() + "/api/user/child/" + childId;
    HttpRequestCreator* request = API::UpdateChildRequest(url, childId, childProfileName, childGender, childDOB, oomeeUrl, ownerId, this);
    request->execute();
}

void BackEndCaller::onUpdateChildAnswerReceived()
{
    getAvailableChildren();
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
void BackEndCaller::verifyApplePayment(const std::string& receiptData)
{
    HttpRequestCreator* request = API::VerifyApplePaymentRequest(receiptData, this);
    request->execute(30.0f);
}

//GET CONTENT-------------------------------------------------------------------------------
void BackEndCaller::getHQContent(const std::string& url, const std::string& category)
{
        HttpRequestCreator* request = API::GetEncryptedContentRequest(url, category, this);
        request->execute();
}

// DEEPLINK CONTENT DETAILS REQUEST ----------------------------------------------------------------
void BackEndCaller::getElectricDreamsContent(const std::string& requestId, const std::string& contentID)
{
    if(ChildDataStorage::getInstance()->childLoggedIn)
    {
        HttpRequestCreator* request = API::GetElectricDreamsContent(requestId, ChildDataStorage::getInstance()->loggedInChildId, contentID, this);
        request->execute();
    }
}

// RESET PASSWORD REQUEST ----------------------------------------------------------------
void BackEndCaller::resetPasswordRequest(const std::string& emailAddress)
{
    HttpRequestCreator* request = API::ResetPaswordRequest(Net::urlEncode(stringToLower(emailAddress)), this);
    request->execute();
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
    else if(requestTag == API::TagGetAvailableChildren)
    {
        onGetChildrenAnswerReceived(body);
    }
    else if(requestTag == API::TagLogin)
    {
        onLoginAnswerReceived(body, headers);
    }
    else if(requestTag == API::TagAnonymousDeviceLogin)
    {
        onAnonymousDeviceLoginAnswerReceived(body, headers);
    }
    else if(requestTag == API::TagRegisterChild)
    {
        onRegisterChildAnswerReceived();
    }
    else if(requestTag == API::TagUpdateChild)
    {
        onUpdateChildAnswerReceived();
    }
    else if(requestTag == API::TagRegisterParent)
    {
        onRegisterParentAnswerReceived();
    }
    else if(requestTag == API::TagParentPin)
    {
        onUpdateParentPinAnswerReceived(body);
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
    else if(requestTag == ConfigStorage::kGroupHQName)
    {
        HQDataParser::getInstance()->onGetContentAnswerReceived(body, requestTag);
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
    else if(requestTag == API::TagGetForceUpdateInformation)
    {
        onGetForceUpdateDataAnswerReceived(body);
    }
    else
    {
        std::vector<std::string> hqNames = ConfigStorage::getInstance()->getHqNames();
        hqNames.push_back(ConfigStorage::kGroupHQName);
        for(const std::string& hqName : hqNames)
        {
            if(requestTag == hqName)
            {
                HQDataParser::getInstance()->onGetContentAnswerReceived(body, requestTag);
                break;
            }
        }
    }
}

void BackEndCaller::onHttpRequestFailed(const std::string& requestTag, long errorCode)
{
    if(requestTag == API::TagGetForceUpdateInformation)
    {
        return; //if the file cannot be found, we do nothing, update won't be forced.
    }
    
    if(requestTag == API::TagOfflineCheck)
    {
        OfflineChecker::getInstance()->onOfflineCheckFailed();
        return;
    }
    
    if(requestTag == API::TagRegisterParent)
    {
        AnalyticsSingleton::getInstance()->OnboardingAccountCreatedErrorEvent(errorCode);
        hideLoadingScreen();
        FlowDataSingleton::getInstance()->setErrorCode(errorCode);
        MessageBox::createWith(errorCode, nullptr);
        return;
    }
    
    if(requestTag == API::TagRegisterChild)
    {
        AnalyticsSingleton::getInstance()->childProfileCreatedErrorEvent(errorCode);
        hideLoadingScreen();
        FlowDataSingleton::getInstance()->setErrorCode(errorCode);
        DynamicNodeHandler::getInstance()->startAddChildFlow();
        return;
    }
    
    if(requestTag == API::TagUpdateChild)
    {
        AnalyticsSingleton::getInstance()->childProfileUpdateErrorEvent(errorCode);
        hideLoadingScreen();
        FlowDataSingleton::getInstance()->setErrorCode(errorCode);
        DynamicNodeHandler::getInstance()->startAddChildFlow();
        return;
    }
    
    if(requestTag == API::TagLogin)
    {
        FlowDataSingleton::getInstance()->setErrorCode(errorCode);
        LoginLogicHandler::getInstance()->forceNewLogin();
    }
    
    if(requestTag == API::TagGetAvailableChildren)
    {

        FlowDataSingleton::getInstance()->setErrorCode(errorCode);
        if(errorCode == -1)
        {
            Director::getInstance()->replaceScene(SceneManagerScene::createScene(OfflineHub));
            return;
        }
        
        LoginLogicHandler::getInstance()->forceNewLogin();
        return;
    }
    
    if(requestTag == API::TagVerifyApplePayment || requestTag == API::TagVerifyAmazonPayment || requestTag == API::TagVerifyGooglePayment)
    {
        cocos2d::log("IAP Failed with Errorcode: %ld", errorCode);
        AnalyticsSingleton::getInstance()->iapBackEndRequestFailedEvent(errorCode);
        RoutePaymentSingleton::getInstance()->backendRequestFailed(errorCode);
        return;
    }
    //Dont do anything with a password Request attempt
    if(requestTag == API::TagResetPasswordRequest)
        return;
    
    if(requestTag == API::TagUpdateBillingData)
        return;
    
    FlowDataSingleton::getInstance()->setErrorCode(errorCode);
    LoginLogicHandler::getInstance()->doLoginLogic();
}

NS_AZOOMEE_END
