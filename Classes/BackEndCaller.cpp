#include "BackEndCaller.h"

#include <AzoomeeCommon/JWTSigner/JWTTool.h>
#include <AzoomeeCommon/Data/Child/ChildDataParser.h>
#include <AzoomeeCommon/Data/Child/ChildDataProvider.h>
#include <AzoomeeCommon/Data/Parent/ParentDataProvider.h>
#include <AzoomeeCommon/Data/Cookie/CookieDataParser.h>
#include <AzoomeeCommon/UI/ModalMessages.h>
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include "ParentDataParser.h"
#include "HQDataParser.h"
#include "LoginLogicHandler.h"
#include "ChildSelectorScene.h"
#include "BaseScene.h"
#include "OnboardingScene.h"
#include "ChildAccountScene.h"
#include "AwaitingAdultPinLayer.h"
#include "HQHistoryManager.h"
#include "OnboardingSuccessScene.h"
#include "ChildAccountSuccessScene.h"
#include "RoutePaymentSingleton.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "ApplePaymentSingleton.h"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "GooglePaymentSingleton.h"
#include "AmazonPaymentSingleton.h"
#endif

using namespace cocos2d;

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

BackEndCaller::~BackEndCaller(void)
{
}

bool BackEndCaller::init(void)
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
    accountJustRegistered = false;
    newChildJustRegistered = false;
    newSubscriptionJustStarted = false;
    
    LoginLogicHandler::getInstance()->setErrorMessageCodeToDisplay(errorCode);
    LoginLogicHandler::getInstance()->forceNewLogin();
}


//LOGGING IN BY PARENT-------------------------------------------------------------------------------

void BackEndCaller::login(std::string username, std::string password)
{
    displayLoadingScreen();
    
    HttpRequestCreator* httpRequestCreator = new HttpRequestCreator(this);
    httpRequestCreator->requestBody = StringUtils::format("{\"password\": \"%s\",\"userName\": \"%s\",\"appType\": \"CHILD_APP\"}", password.c_str(), username.c_str());
    httpRequestCreator->requestTag = "parentLogin";
    httpRequestCreator->createPostHttpRequest();
    
    UserDefault* def = UserDefault::getInstance();
    def->setStringForKey("username", username);
    def->flush();
    
    AnalyticsSingleton::getInstance()->registerAzoomeeEmail(username);
}

void BackEndCaller::onLoginAnswerReceived(std::string responseString)
{
    CCLOG("Response string is: %s", responseString.c_str());
    if(ParentDataParser::getInstance()->parseParentLoginData(responseString))
    {
        updateBillingData();
        getAvailableChildren();
        AnalyticsSingleton::getInstance()->signInSuccessEvent();
    }
    else
    {
        AnalyticsSingleton::getInstance()->signInFailEvent(0);
        getBackToLoginScreen(ERROR_CODE_INVALID_CREDENTIALS);
    }
}

//UPDATING BILLING DATA-------------------------------------------------------------------------------

void BackEndCaller::updateBillingData()
{
    HttpRequestCreator* httpRequestCreator = new HttpRequestCreator(this);
    httpRequestCreator->requestTag = "updateBilling";
    httpRequestCreator->createEncryptedGetHttpRequest();
}

void BackEndCaller::onUpdateBillingDataAnswerReceived(std::string responseString)
{
    ParentDataParser::getInstance()->parseParentBillingData(responseString);
}

//UPDATING PARENT DATA--------------------------------------------------------------------------------

void BackEndCaller::updateParentPin(Node *callBackTo)
{
    displayLoadingScreen();
    
    callBackNode = callBackTo;
    
    HttpRequestCreator* httpRequestCreator = new HttpRequestCreator(this);
    httpRequestCreator->requestTag = "updateParentPin";
    httpRequestCreator->createEncryptedGetHttpRequest();
}

void BackEndCaller::onUpdateParentPinAnswerReceived(std::string responseString)
{
    CCLOG("Update parent response string is: %s", responseString.c_str());
    if(ParentDataParser::getInstance()->parseUpdateParentData(responseString))
    {
        hideLoadingScreen();
        
        AwaitingAdultPinLayer *checkBack = (AwaitingAdultPinLayer *)callBackNode;
        CCLOG("Calling back awaitingsomething");
        checkBack->secondCheckForPin();
    }
}

//GETTING AVAILABLE CHILDREN--------------------------------------------------------------------------

void BackEndCaller::getAvailableChildren()
{
    ModalMessages::getInstance()->startLoading();
    
    HttpRequestCreator* httpRequestCreator = new HttpRequestCreator(this);
    httpRequestCreator->urlParameters = "expand=true";
    httpRequestCreator->requestTag = "getChildren";
    httpRequestCreator->createEncryptedGetHttpRequest();
}

void BackEndCaller::onGetChildrenAnswerReceived(std::string responseString)
{
    ParentDataParser::getInstance()->parseAvailableChildren(responseString);
    
    if(newChildJustRegistered)
    {
        newChildJustRegistered = false;
        auto childAccountSuccessScene = ChildAccountSuccessScene::createScene(newChildName, oomeeAvatarNumber);
        Director::getInstance()->replaceScene(childAccountSuccessScene);
    }
    else if(accountJustRegistered)
    {
        CCLOG("Just registered account : backendcaller");
        accountJustRegistered = false;
        auto onboardingSuccessScene = OnboardingSuccessScene::createScene(false);
        Director::getInstance()->replaceScene(onboardingSuccessScene);
    }
    else if(newSubscriptionJustStarted)
    {
        CCLOG("Just started new trial : backendcaller");
        newSubscriptionJustStarted = false;
        auto onboardingSuccessScene = OnboardingSuccessScene::createScene(true);
        Director::getInstance()->replaceScene(onboardingSuccessScene);
    }
    else if(RoutePaymentSingleton::getInstance()->checkIfAppleReceiptRefreshNeeded())
    {
        auto childSelectorScene = ChildSelectorScene::createScene();
        Director::getInstance()->replaceScene(childSelectorScene);
    }
}

//CHILDREN LOGIN----------------------------------------------------------------------------------------

void BackEndCaller::childLogin(int childNumber)
{
    HQDataParser::getInstance()->clearAllHQData();
    
    displayLoadingScreen();
    
    HttpRequestCreator* httpRequestCreator = new HttpRequestCreator(this);
    httpRequestCreator->requestBody = StringUtils::format("{\"userName\": \"%s\", \"password\": \"\"}", ParentDataProvider::getInstance()->getProfileNameForAnAvailableChildren(childNumber).c_str());
    httpRequestCreator->requestTag = "childLogin";
    httpRequestCreator->createEncryptedPostHttpRequest();
    
    ChildDataParser::getInstance()->setLoggedInChildName(ParentDataProvider::getInstance()->getProfileNameForAnAvailableChildren(childNumber));
    ChildDataParser::getInstance()->setLoggedInChildNumber(childNumber);
}

void BackEndCaller::onChildLoginAnswerReceived(std::string responseString)
{
    ChildDataParser::getInstance()->parseChildLoginData(responseString);
    
    getHQContent(StringUtils::format("%s%s/%s", ConfigStorage::getInstance()->getServerUrl().c_str(), ConfigStorage::getInstance()->getPathForTag("HOME").c_str(), ChildDataProvider::getInstance()->getLoggedInChildId().c_str()), "HOME");
}

//GETTING GORDON.PNG-------------------------------------------------------------------------------------

void BackEndCaller::getGordon()
{
    HttpRequestCreator* httpRequestCreator = new HttpRequestCreator(this);
    httpRequestCreator->urlParameters = StringUtils::format("userid=%s&sessionid=%s", ChildDataProvider::getInstance()->getParentOrChildId().c_str(), ChildDataProvider::getInstance()->getParentOrChildCdnSessionId().c_str());
    httpRequestCreator->requestTag = "getGordon";
    httpRequestCreator->createEncryptedGetHttpRequest();
}

void BackEndCaller::onGetGordonAnswerReceived(std::string responseString)
{
    if(CookieDataParser::getInstance()->parseDownloadCookies(responseString))
    {
        HQHistoryManager::getInstance()->emptyHistory();
        auto baseScene = BaseScene::createScene();
        Director::getInstance()->replaceScene(baseScene);
    }
}

//REGISTER PARENT---------------------------------------------------------------------------

void BackEndCaller::registerParent(std::string emailAddress, std::string password, std::string pinNumber)
{
    //Save emailAddress and password, so onRegisterParentAnswerReceived can login after success
    this->registerParentUsername = emailAddress;
    this->registerParentPassword = password;
    
    std::string source = "OTHER";
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    source = "IOS_INAPP";
    
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    source = "ANDROID_INAPP";
    
#endif
    
    HttpRequestCreator* httpRequestCreator = new HttpRequestCreator(this);
    httpRequestCreator->requestBody = StringUtils::format("{\"emailAddress\":\"%s\",\"over18\":\"true\",\"termsAccepted\":\"true\",\"password\":\"%s\",\"source\":\"%s\",\"pinNumber\":\"%s\"}", emailAddress.c_str(),password.c_str(),source.c_str(),pinNumber.c_str());
    httpRequestCreator->requestTag = "registerParent";
    httpRequestCreator->createPostHttpRequest();
    
    displayLoadingScreen();
}

void BackEndCaller::onRegisterParentAnswerReceived()
{
    accountJustRegistered = true;
    AnalyticsSingleton::getInstance()->OnboardingAccountCreatedEvent();
    login(this->registerParentUsername, this->registerParentPassword);
}

//REGISTER CHILD----------------------------------------------------------------------------

void BackEndCaller::registerChild(std::string childProfileName, std::string childGender, std::string childDOB, int oomeeNumber)
{
    displayLoadingScreen();
    
    newChildName = childProfileName;
    oomeeAvatarNumber = oomeeNumber;
    
    HttpRequestCreator* httpRequestCreator = new HttpRequestCreator(this);
    httpRequestCreator->requestBody = StringUtils::format("{\"profileName\":\"%s\",\"dob\":\"%s\",\"sex\":\"%s\",\"avatar\":\"%s\",\"password\":\"\"}",childProfileName.c_str(),childDOB.c_str(),childGender.c_str(),ConfigStorage::getInstance()->getUrlForOomee(oomeeNumber).c_str());
    httpRequestCreator->requestTag = "registerChild";
    httpRequestCreator->createEncryptedPostHttpRequest();
}

void BackEndCaller::onRegisterChildAnswerReceived()
{
    newChildJustRegistered = true;
    AnalyticsSingleton::getInstance()->childProfileCreatedSuccessEvent(oomeeAvatarNumber);
    getAvailableChildren();
}

//GOOGLE VERIFY PAYMENT---------------------------------------------------------------------
void BackEndCaller::verifyGooglePayment(const std::string& orderId, const std::string& iapSku, const std::string& purchaseToken)
{
    HttpRequestCreator* httpRequestCreator = new HttpRequestCreator(this);
    httpRequestCreator->requestBody = StringUtils::format("{\"orderId\": \"%s\", \"subscriptionId\": \"%s\", \"purchaseToken\": \"%s\"}", orderId.c_str(), iapSku.c_str(), purchaseToken.c_str());
    httpRequestCreator->requestTag = "iabGooglePaymentMade";
    httpRequestCreator->createEncryptedPostHttpRequest();
}

//AMAZON VERIFY PAYMENT---------------------------------------------------------------------
void BackEndCaller::verifyAmazonPayment(const std::string& requestId, const std::string& receiptId, const std::string& amazonUserid)
{
    HttpRequestCreator* httpRequestCreator = new HttpRequestCreator(this);
    httpRequestCreator->requestBody = StringUtils::format("{\"requestId\": \"%s\", \"receiptId\": \"%s\", \"amazonUserId\": \"%s\"}", requestId.c_str(), receiptId.c_str(), amazonUserid.c_str());
    httpRequestCreator->requestTag = "iapAmazonPaymentMade";
    httpRequestCreator->createEncryptedPostHttpRequest();
}

//APPLE VERIFY PAYMENT----------------------------------------------------------------------
void BackEndCaller::verifyApplePayment(const std::string& receiptData)
{
    HttpRequestCreator* httpRequestCreator = new HttpRequestCreator(this);
    httpRequestCreator->requestBody = StringUtils::format("{\"receipt-data\": \"%s\"}", receiptData.c_str());
    httpRequestCreator->requestTag = "iapApplePaymentMade";
    httpRequestCreator->createEncryptedPostHttpRequest();
}

//GET CONTENT-------------------------------------------------------------------------------
void BackEndCaller::getHQContent(const std::string& url, const std::string& category)
{
    HttpRequestCreator* httpRequestCreator = new HttpRequestCreator(this);
    httpRequestCreator->url = url;
    httpRequestCreator->requestBody = "";
    httpRequestCreator->requestTag = category;
    
    if(category == "PreviewHOME")
    {
        httpRequestCreator->createGetHttpRequest();
    }
    else
    {
        httpRequestCreator->createEncryptedGetHttpRequest();
    }
}

//HttpRequestCreatorResponseDelegate--------------------------------------------------------
void BackEndCaller::onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body)
{
    if(requestTag == "getGordon") onGetGordonAnswerReceived(headers);
    if(requestTag == "childLogin") onChildLoginAnswerReceived(body);
    if(requestTag == "getChildren") onGetChildrenAnswerReceived(body);
    if(requestTag == "parentLogin") onLoginAnswerReceived(body);
    if(requestTag == "registerChild") onRegisterChildAnswerReceived();
    if(requestTag == "registerParent") onRegisterParentAnswerReceived();
    if(requestTag == "updateParentPin") onUpdateParentPinAnswerReceived(body);
    if(requestTag == "PreviewHOME") HQDataParser::getInstance()->onGetPreviewContentAnswerReceived(body);
    if(requestTag == "updateBilling") onUpdateBillingDataAnswerReceived(body);
    if(requestTag == "GROUP HQ") HQDataParser::getInstance()->onGetContentAnswerReceived(body, requestTag);
    
    for(int i = 0; i < 6; i++)
    {
        if(requestTag == ConfigStorage::getInstance()->getNameForMenuItem(i))
        {
            HQDataParser::getInstance()->onGetContentAnswerReceived(body, requestTag);
        }
    }
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    if(requestTag == "iapApplePaymentMade") ApplePaymentSingleton::getInstance()->onAnswerReceived(body);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if(requestTag == "iabGooglePaymentMade") GooglePaymentSingleton::getInstance()->onGooglePaymentVerificationAnswerReceived(body);
    if(requestTag == "iapAmazonPaymentMade") AmazonPaymentSingleton::getInstance()->onAmazonPaymentMadeAnswerReceived(body);
#endif
}

void BackEndCaller::onHttpRequestFailed(const std::string& requestTag, long errorCode)
{
    if(requestTag == "registerParent")
    {
        AnalyticsSingleton::getInstance()->OnboardingAccountCreatedErrorEvent(errorCode);
        Director::getInstance()->replaceScene(OnboardingScene::createScene(errorCode));
        return;
    }
    
    if(requestTag == "registerChild")
    {
        AnalyticsSingleton::getInstance()->childProfileCreatedErrorEvent(errorCode);
        Director::getInstance()->replaceScene(ChildAccountScene::createScene("", errorCode));
        return;
    }
    
    if(requestTag == "parentLogin")
    {
        LoginLogicHandler::getInstance()->setErrorMessageCodeToDisplay(errorCode);
        LoginLogicHandler::getInstance()->forceNewLogin();
    }
    
    if(requestTag == "getChildren")
    {
        LoginLogicHandler::getInstance()->setErrorMessageCodeToDisplay(errorCode);
        LoginLogicHandler::getInstance()->forceNewLogin();
        return;
    }
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    if(requestTag == "iapApplePaymentMade")
    {
        CCLOG("IAP Failed with Errorcode: %ld", errorCode);
        AnalyticsSingleton::getInstance()->iapBackEndRequestFailedEvent(errorCode);
        ApplePaymentSingleton::getInstance()->backendRequestFailed(errorCode);
        return;
    }
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if(requestTag == "iapAmazonPaymentMade")
    {
        CCLOG("IAP Failed with Errorcode: %ld", errorCode);
        AnalyticsSingleton::getInstance()->iapBackEndRequestFailedEvent(errorCode);
        AmazonPaymentSingleton::getInstance()->backendRequestFailed();
        return;
    }
    
    if(requestTag == "iabGooglePaymentMade")
    {
        CCLOG("IAP Failed with Errorcode: %ld", errorCode);
        AnalyticsSingleton::getInstance()->iapBackEndRequestFailedEvent(errorCode);
        GooglePaymentSingleton::getInstance()->backendRequestFailed();
        return;
    }
#endif
    
    ChildDataParser::getInstance()->setChildLoggedIn(false);
    getAvailableChildren();
}
