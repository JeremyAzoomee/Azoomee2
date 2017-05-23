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
#include <AzoomeeCommon/API/API.h>
#include "HQDataParser.h"
#include "HQHistoryManager.h"
#include "HQDataStorage.h"
#include "LoginLogicHandler.h"
#include "ChildSelectorScene.h"
#include "BaseScene.h"
#include "ChildAccountScene.h"
#include "AwaitingAdultPinLayer.h"
#include "ChildAccountSuccessScene.h"
#include "RoutePaymentSingleton.h"
#include "OrientationChangeScene.h"
#include "DeepLinkingSingleton.h"
#include "FlowDataSingleton.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "ApplePaymentSingleton.h"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "GooglePaymentSingleton.h"
#include "AmazonPaymentSingleton.h"
#endif

using namespace cocos2d;
using namespace Azoomee;


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
    newChildJustRegistered = false;
    
    LoginLogicHandler::getInstance()->setErrorMessageCodeToDisplay(errorCode);
    LoginLogicHandler::getInstance()->forceNewLogin();
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

void BackEndCaller::onLoginAnswerReceived(const std::string& responseString)
{
    CCLOG("Response string is: %s", responseString.c_str());
    if(ParentDataParser::getInstance()->parseParentLoginData(responseString))
    {
        HQDataStorage::getInstance()->HQListTitles.clear();
        HQDataStorage::getInstance()->HQListElements.clear();
        HQDataStorage::getInstance()->HQElementHighlights.clear();
        HQDataStorage::getInstance()->HQData.clear();
        HQDataStorage::getInstance()->HQGetContentUrls.clear();
        
        updateBillingData();
        getAvailableChildren();
        AnalyticsSingleton::getInstance()->signInSuccessEvent();
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
    HttpRequestCreator* request = API::UpdateBillingDataRequest(this);
    request->execute();
}

void BackEndCaller::onUpdateBillingDataAnswerReceived(const std::string& responseString)
{
    ParentDataParser::getInstance()->parseParentBillingData(responseString);
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
    
    HttpRequestCreator* request = API::GetAvailableChildrenRequest(this);
    request->execute();
}

void BackEndCaller::onGetChildrenAnswerReceived(const std::string& responseString)
{
    ParentDataParser::getInstance()->parseAvailableChildren(responseString);
    
    if(newChildJustRegistered)
    {
        newChildJustRegistered = false;
        auto orientationChangeScene = OrientationChangeScene::createSceneChildAccountNext(newChildRegisteredNextSceneID, 0,newChildName, oomeeAvatarNumber);
        Director::getInstance()->replaceScene(orientationChangeScene);
    }
    else if(FlowDataSingleton::getInstance()->isSignupFlow())
    {
        CCLOG("Just registered account : backendcaller");
        auto orientationChangeScene = OrientationChangeScene::createScene(CHILD_ACCOUNT_SCENE_AFTERSIGNUP, 0);
        Director::getInstance()->replaceScene(orientationChangeScene);
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
    
    const std::string& profileName = ParentDataProvider::getInstance()->getProfileNameForAnAvailableChildren(childNumber);
    HttpRequestCreator* request = API::ChildLoginRequest(profileName, this);
    request->execute();
    
    ChildDataParser::getInstance()->setLoggedInChildName(ParentDataProvider::getInstance()->getProfileNameForAnAvailableChildren(childNumber));
    ChildDataParser::getInstance()->setLoggedInChildNumber(childNumber);
}

void BackEndCaller::onChildLoginAnswerReceived(const std::string& responseString)
{
    ChildDataParser::getInstance()->parseChildLoginData(responseString);
    
    getHQContent(StringUtils::format("%s%s/%s", ConfigStorage::getInstance()->getServerUrl().c_str(), ConfigStorage::getInstance()->getPathForTag("HOME").c_str(), ChildDataProvider::getInstance()->getLoggedInChildId().c_str()), "HOME");
}

//GETTING GORDON.PNG-------------------------------------------------------------------------------------

void BackEndCaller::getGordon()
{
    const std::string& userId = ChildDataProvider::getInstance()->getParentOrChildId();
    const std::string& sessionId = ChildDataProvider::getInstance()->getParentOrChildCdnSessionId();
    
    HttpRequestCreator* request = API::GetGordenRequest(userId, sessionId, this);
    request->execute();
}

void BackEndCaller::onGetGordonAnswerReceived(const std::string& responseString)
{
    if(CookieDataParser::getInstance()->parseDownloadCookies(responseString))
    {
        HQHistoryManager::getInstance()->emptyHistory();
        auto baseScene = BaseScene::createScene();
        Director::getInstance()->replaceScene(baseScene);
    }
}

//REGISTER PARENT---------------------------------------------------------------------------

void BackEndCaller::registerParent(const std::string& emailAddress, const std::string& password, const std::string& pinNumber)
{
    FlowDataSingleton::getInstance()->setFlowToSignup(emailAddress, password);
    
    std::string source = "OTHER";
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    source = "IOS_INAPP";
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    source = "ANDROID_INAPP";
#endif
    
    HttpRequestCreator* request = API::RegisterParentRequest(emailAddress, password, pinNumber, source, this);
    request->execute();
    
    displayLoadingScreen();
}

void BackEndCaller::onRegisterParentAnswerReceived()
{
    AnalyticsSingleton::getInstance()->OnboardingAccountCreatedEvent();
    login(FlowDataSingleton::getInstance()->getUserName(), FlowDataSingleton::getInstance()->getPassword());
}

//REGISTER CHILD----------------------------------------------------------------------------

void BackEndCaller::registerChild(const std::string& childProfileName, const std::string& childGender, const std::string& childDOB, int oomeeNumber, int nextSceneID)
{
    displayLoadingScreen();
    
    newChildRegisteredNextSceneID = nextSceneID;
    
    newChildName = childProfileName;
    oomeeAvatarNumber = oomeeNumber;
    
    const std::string& oomeeUrl = ConfigStorage::getInstance()->getUrlForOomee(oomeeNumber);
    HttpRequestCreator* request = API::RegisterChildRequest(childProfileName, childGender, childDOB, oomeeUrl, this);
    request->execute();
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
    HttpRequestCreator* request = API::VerifyGooglePaymentRequest(orderId, iapSku, purchaseToken, this);
    request->execute();
}

//AMAZON VERIFY PAYMENT---------------------------------------------------------------------
void BackEndCaller::verifyAmazonPayment(const std::string& requestId, const std::string& receiptId, const std::string& amazonUserid)
{
    HttpRequestCreator* request = API::VerifyAmazonPaymentRequest(requestId, receiptId, amazonUserid, this);
    request->execute();
}

//APPLE VERIFY PAYMENT----------------------------------------------------------------------
void BackEndCaller::verifyApplePayment(const std::string& receiptData)
{
    HttpRequestCreator* request = API::VerifyApplePaymentRequest(receiptData, this);
    request->execute();
}

//GET CONTENT-------------------------------------------------------------------------------
void BackEndCaller::getHQContent(const std::string& url, const std::string& category)
{
    // Preview doesn't need to be encrypted
    if(category == "PreviewHOME")
    {
        HttpRequestCreator* request = API::GetPublicContentRequest(url, category, this);
        request->execute();
    }
    else
    {
        HttpRequestCreator* request = API::GetEncryptedContentRequest(url, category, this);
        request->execute();
    }
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
    HttpRequestCreator* request = API::ResetPaswordRequest(emailAddress, this);
    request->execute();
}

//HttpRequestCreatorResponseDelegate--------------------------------------------------------
void BackEndCaller::onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body)
{
    if(requestTag == API::TagGetGorden)
    {
        onGetGordonAnswerReceived(headers);
    }
    else if(requestTag == API::TagChildLogin)
    {
        onChildLoginAnswerReceived(body);
    }
    else if(requestTag == API::TagGetAvailableChildren)
    {
        onGetChildrenAnswerReceived(body);
    }
    else if(requestTag == API::TagLogin)
    {
        onLoginAnswerReceived(body);
    }
    else if(requestTag == API::TagRegisterChild)
    {
        onRegisterChildAnswerReceived();
    }
    else if(requestTag == API::TagRegisterParent)
    {
        onRegisterParentAnswerReceived();
    }
    else if(requestTag == API::TagParentPin)
    {
        onUpdateParentPinAnswerReceived(body);
    }
    else if(requestTag == "PreviewHOME")
    {
        HQDataParser::getInstance()->onGetPreviewContentAnswerReceived(body);
    }
    else if(requestTag == "deepLinkContentRequest")
    {
        DeepLinkingSingleton::getInstance()->contentDetailsResponse(body);
    }
    else if(requestTag == API::TagUpdateBillingData)
    {
        onUpdateBillingDataAnswerReceived(body);
    }
    else if(requestTag == "GROUP HQ")
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
    else
    {
        for(int i = 0; i < 6; i++)
        {
            if(requestTag == ConfigStorage::getInstance()->getNameForMenuItem(i))
            {
                HQDataParser::getInstance()->onGetContentAnswerReceived(body, requestTag);
                break;
            }
        }
    }
}

void BackEndCaller::onHttpRequestFailed(const std::string& requestTag, long errorCode)
{
    if(requestTag == API::TagRegisterParent)
    {
        AnalyticsSingleton::getInstance()->OnboardingAccountCreatedErrorEvent(errorCode);
        FlowDataSingleton::getInstance()->setErrorCode(errorCode);
        auto orientationChangeScene = OrientationChangeScene::createScene(ONBOARDING_SCENE, errorCode);
        Director::getInstance()->replaceScene(orientationChangeScene);
        return;
    }
    
    if(requestTag == API::TagRegisterChild)
    {
        AnalyticsSingleton::getInstance()->childProfileCreatedErrorEvent(errorCode);

        auto orientationChangeScene = OrientationChangeScene::createScene(CHILD_ACCOUNT_SCENE_CHILD_CREATION, errorCode);
        Director::getInstance()->replaceScene(orientationChangeScene);
        return;
    }
    
    if(requestTag == API::TagLogin)
    {
        FlowDataSingleton::getInstance()->setErrorCode(errorCode);
        LoginLogicHandler::getInstance()->setErrorMessageCodeToDisplay(errorCode);
        LoginLogicHandler::getInstance()->forceNewLogin();
    }
    
    if(requestTag == API::TagGetAvailableChildren)
    {
        LoginLogicHandler::getInstance()->setErrorMessageCodeToDisplay(errorCode);
        LoginLogicHandler::getInstance()->forceNewLogin();
        return;
    }
    
    if(requestTag == API::TagVerifyApplePayment || requestTag == API::TagVerifyAmazonPayment || requestTag == API::TagVerifyGooglePayment)
    {
        CCLOG("IAP Failed with Errorcode: %ld", errorCode);
        AnalyticsSingleton::getInstance()->iapBackEndRequestFailedEvent(errorCode);
        RoutePaymentSingleton::getInstance()->backendRequestFailed(errorCode);
        return;
    }
    //Dont do anything with a password Request attempt
    if(requestTag == API::TagResetPasswordRequest)
        return;
    
    ChildDataParser::getInstance()->setChildLoggedIn(false);
    getAvailableChildren();
}
