#include "BackEndCaller.h"

#include <AzoomeeCommon/Data/Child/ChildDataParser.h>
#include <AzoomeeCommon/Data/Child/ChildDataProvider.h>
#include "ParentDataParser.h"
#include <AzoomeeCommon/Data/Parent/ParentDataProvider.h>
#include <AzoomeeCommon/Data/Cookie/CookieDataParser.h>
#include "HQDataParser.h"
#include "LoginScene.h"
#include "ChildSelectorScene.h"
#include "BaseScene.h"
#include "HttpRequestCreator.h"
#include <AzoomeeCommon/UI/ModalMessages.h>
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include "AwaitingAdultPinLayer.h"
#include "HQHistoryManager.h"
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include "OnboardingSuccessScene.h"
#include "ChildAccountSuccessScene.h"
#include "PaymentSingleton.h"

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
    newTrialJustStarted = false;
    auto loginScene = LoginScene::createScene(errorCode);
    Director::getInstance()->replaceScene(loginScene);
}


//LOGGING IN BY PARENT-------------------------------------------------------------------------------

void BackEndCaller::autoLogin()
{
    UserDefault* def = UserDefault::getInstance();
    std::string username = def->getStringForKey("username", "");
    std::string password = def->getStringForKey("password", "");
    def->flush();
    
    login(username, password);
}

void BackEndCaller::login(std::string username, std::string password)
{
    displayLoadingScreen();
    
    HttpRequestCreator* httpRequestCreator = new HttpRequestCreator();
    httpRequestCreator->requestBody = StringUtils::format("{\"password\": \"%s\",\"userName\": \"%s\",\"appType\": \"CHILD_APP\"}", password.c_str(), username.c_str());
    httpRequestCreator->requestTag = "parentLogin";
    httpRequestCreator->createPostHttpRequest();
    
    UserDefault* def = UserDefault::getInstance();
    def->setStringForKey("username", username);
    def->setStringForKey("password", password);
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
    HttpRequestCreator* httpRequestCreator = new HttpRequestCreator();
    httpRequestCreator->requestTag = "updateBilling";
    httpRequestCreator->createEncryptedGetHttpRequest();
}

void BackEndCaller::onUpdateBillingDataAnswerReceived(std::string responseString)
{
    ParentDataParser::getInstance()->parseParentBillingData(responseString);
    AnalyticsSingleton::getInstance()->registerBillingStatus(ParentDataProvider::getInstance()->getBillingStatus());
}

//UPDATING PARENT DATA--------------------------------------------------------------------------------

void BackEndCaller::updateParent(Node *callBackTo, std::string target) //"pin" or "actorstatus"
{
    displayLoadingScreen();
    
    callBackNode = callBackTo;
    
    HttpRequestCreator* httpRequestCreator = new HttpRequestCreator();
    
    httpRequestCreator->requestTag = "updateParentPin";
    if(target == "actorstatus") httpRequestCreator->requestTag = "updateParentActorStatus";
    
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

void BackEndCaller::onUpdateParentActorStatusAnswerReceived(std::string responseString)
{
    CCLOG("Update parent response string is: %s", responseString.c_str());
    if(ParentDataParser::getInstance()->parseUpdateParentData(responseString))
    {
        hideLoadingScreen();
        
        ChildSelectorScene *checkBack = (ChildSelectorScene *)callBackNode;
        CCLOG("Calling back awaitingsomething");
        checkBack->secondCheckForAuthorisation();
    }
}

//GETTING AVAILABLE CHILDREN--------------------------------------------------------------------------

void BackEndCaller::getAvailableChildren()
{
    ModalMessages::getInstance()->startLoading();
    
    HttpRequestCreator* httpRequestCreator = new HttpRequestCreator();
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
        auto onboardingSuccessScene = OnboardingSuccessScene::createScene(PaymentSingleton::getInstance()->OS_is_IAP_Compatible(),false);
        Director::getInstance()->replaceScene(onboardingSuccessScene);
    }
    else if(newTrialJustStarted)
    {
        CCLOG("Just started new trial : backendcaller");
        newTrialJustStarted = false;
        auto onboardingSuccessScene = OnboardingSuccessScene::createScene(PaymentSingleton::getInstance()->OS_is_IAP_Compatible(),true);
        Director::getInstance()->replaceScene(onboardingSuccessScene);
    }
    else
    {
        auto childSelectorScene = ChildSelectorScene::createScene(0);
        Director::getInstance()->replaceScene(childSelectorScene);
    }
}

//CHILDREN LOGIN----------------------------------------------------------------------------------------

void BackEndCaller::childLogin(int childNumber)
{
    HQDataParser::getInstance()->clearAllHQData();
    
    displayLoadingScreen();
    
    HttpRequestCreator* httpRequestCreator = new HttpRequestCreator();
    httpRequestCreator->requestBody = StringUtils::format("{\"userName\": \"%s\", \"password\": \"\"}", ParentDataProvider::getInstance()->getProfileNameForAnAvailableChildren(childNumber).c_str());
    httpRequestCreator->requestTag = "childLogin";
    httpRequestCreator->createEncryptedPostHttpRequest();
    
    ChildDataParser::getInstance()->setLoggedInChildName(ParentDataProvider::getInstance()->getProfileNameForAnAvailableChildren(childNumber));
    ChildDataParser::getInstance()->setLoggedInChildNumber(childNumber);
}

void BackEndCaller::onChildLoginAnswerReceived(std::string responseString)
{
    ChildDataParser::getInstance()->parseChildLoginData(responseString);
    
    HQDataParser::getInstance()->getContent(StringUtils::format("%s%s/%s", ConfigStorage::getInstance()->getServerUrl().c_str(), ConfigStorage::getInstance()->getPathForTag("HOME").c_str(), ChildDataProvider::getInstance()->getLoggedInChildId().c_str()), "HOME");
}

//GETTING GORDON.PNG-------------------------------------------------------------------------------------

void BackEndCaller::getGordon()
{
    HttpRequestCreator* httpRequestCreator = new HttpRequestCreator();
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
    
    HttpRequestCreator* httpRequestCreator = new HttpRequestCreator();
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
    
    HttpRequestCreator* httpRequestCreator = new HttpRequestCreator();
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
