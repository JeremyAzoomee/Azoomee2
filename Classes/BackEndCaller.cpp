#include "BackEndCaller.h"

#include "network/HttpClient.h"

#include "external/json/document.h"
#include "external/json/writer.h"
#include "external/json/stringbuffer.h"

#include "external/json/document.h"
#include "external/json/writer.h"
#include "external/json/stringbuffer.h"
#include "external/json/prettywriter.h"

#include "JWTTool.h"
#include "DataStorage.h"
#include "HQDataProvider.h"
#include "LoginScene.h"
#include "ChildSelectorScene.h"
#include "BaseScene.h"
#include "HttpRequestCreator.h"
#include "OnboardingScene.h"
#include "ChildAccountScene.h"
#include "ModalMessages.h"

#define CI_HOST "api.elb.ci.azoomee.ninja"
#define CI_URL "http://" CI_HOST

using namespace cocos2d;
using namespace network;

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


//LOGGING IN BY PARENT-------------------------------------------------------------------------------


void BackEndCaller::login(std::string username, std::string password)
{
    displayLoadingScreen();
    
    HttpRequestCreator* httpRequestCreator = new HttpRequestCreator();
    httpRequestCreator->requestPath = "/api/auth/login";
    httpRequestCreator->requestBody = StringUtils::format("{\"password\": \"%s\",\"userName\": \"%s\",\"appType\": \"CHILD_APP\"}", password.c_str(), username.c_str());
    httpRequestCreator->requestTag = "parentLogin";
    httpRequestCreator->createPostHttpRequest();
}

void BackEndCaller::onLoginAnswerReceived(std::string responseString)
{
    if(DataStorage::getInstance()->parseParentLoginData(responseString)) getAvailableChildren();
}

//GETTING AVAILABLE CHILDREN--------------------------------------------------------------------------

void BackEndCaller::getAvailableChildren()
{
    HttpRequestCreator* httpRequestCreator = new HttpRequestCreator();
    httpRequestCreator->requestPath = StringUtils::format("/api/user/adult/%s/owns", DataStorage::getInstance()->getParentLoginValue("id").c_str());
    httpRequestCreator->urlParameters = "expand=true";
    httpRequestCreator->requestTag = "getChildren";
    httpRequestCreator->createEncryptedGetHttpRequest();
}

void BackEndCaller::onGetChildrenAnswerReceived(std::string responseString)
{
    DataStorage::getInstance()->parseAvailableChildren(responseString);
        
    auto childSelectorScene = ChildSelectorScene::createScene(0);
    Director::getInstance()->replaceScene(childSelectorScene);
}

//CHILDREN LOGIN----------------------------------------------------------------------------------------

void BackEndCaller::childLogin(int childNumber)
{
    displayLoadingScreen();
    
    HttpRequestCreator* httpRequestCreator = new HttpRequestCreator();
    httpRequestCreator->requestPath = "/api/auth/switchProfile";
    httpRequestCreator->requestBody = StringUtils::format("{\"userName\": \"%s\", \"password\": \"\"}", DataStorage::getInstance()->getValueFromOneAvailableChild(childNumber, "profileName").c_str());
    httpRequestCreator->requestTag = "childLogin";
    httpRequestCreator->createEncryptedPostHttpRequest();
}

void BackEndCaller::onChildLoginAnswerReceived(std::string responseString)
{
    DataStorage::getInstance()->parseChildLoginData(responseString);
    HQDataProvider::getInstance()->getContent(StringUtils::format(CI_URL"/api/electricdreams/view/categories/home/%s", DataStorage::getInstance()->getChildLoginValue("id").c_str()), "HOME");
}

//GETTING GORDON.PNG-------------------------------------------------------------------------------------

void BackEndCaller::getGordon()
{
    HttpRequestCreator* httpRequestCreator = new HttpRequestCreator();
    httpRequestCreator->requestPath = "/api/porthole/pixel/gordon.png";
    httpRequestCreator->urlParameters = StringUtils::format("userid=%s&sessionid=%s", DataStorage::getInstance()->getParentOrChildLoginValue("id").c_str(), DataStorage::getInstance()->getParentOrChildLoginValue("cdn-sessionid").c_str());
    httpRequestCreator->requestTag = "getGordon";
    httpRequestCreator->createEncryptedGetHttpRequest();
}

void BackEndCaller::onGetGordonAnswerReceived(std::string responseString)
{
    if(DataStorage::getInstance()->parseDownloadCookies(responseString))
    {
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
    
    ModalMessages::getInstance()->startLoading();
    
    HttpRequest *request = new HttpRequest();
    request->setRequestType(HttpRequest::Type::POST);
    request->setUrl(CI_URL"/api/user/v2/adult");
    
    std::string source = "OTHER";
    //Need to pass SOURCE attribute to server #TODO
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    source = "IOS_INAPP";
    
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    source = "ANDROID_INAPP";
    
#endif
    
    std::string myPostString = StringUtils::format("{\"emailAddress\":\"%s\",\"over18\":\"true\",\"termsAccepted\":\"true\",\"password\":\"%s\",\"source\":\"%s\",\"pinNumber\":\"%s\"}", emailAddress.c_str(),password.c_str(),source.c_str(),pinNumber.c_str());
    const char *postData = myPostString.c_str();
    
    request->setRequestData(postData, strlen(postData));
    
    CCLOG("request data is: %s", request->getRequestData());
    
    std::vector<std::string> headers;
    headers.push_back("Content-Type: application/json;charset=utf-8");
    request->setHeaders(headers);
    
    request->setResponseCallback(CC_CALLBACK_2(BackEndCaller::onRegisterParentAnswerReceived, this));
    request->setTag("registerParent");
    HttpClient::getInstance()->send(request);
}

void BackEndCaller::onRegisterParentAnswerReceived(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    if (response && response->getResponseCode() == 201 && response->getResponseData())
    {
        CCLOG("\n\n\nregister parent success");
        std::vector<char> myResponse = *response->getResponseData();
        CCLOG("register parent feedback: %s", std::string(myResponse.begin(), myResponse.end()).c_str());
        
        login(this->registerParentUsername, this->registerParentPassword);
    }
    else
    {
        CCLOG("Response code: %ld", response->getResponseCode());
        
        //Restart the Onboarding with error
        auto _OnboardingScene = OnboardingScene::createScene(response->getResponseCode());
        Director::getInstance()->replaceScene(_OnboardingScene);
    }
    
}
//GETTING AVAILABLE CHILDREN----------------------------------------------------------------


//REGISTER CHILD----------------------------------------------------------------------------

void BackEndCaller::registerChild(std::string childProfileName, std::string childGender, std::string childDOB, int oomeeNumber)
{
    displayLoadingScreen();
    
    HttpRequestCreator* httpRequestCreator = new HttpRequestCreator();
    httpRequestCreator->requestPath = "/api/user/child";
    httpRequestCreator->requestBody = StringUtils::format("{\"profileName\":\"%s\",\"dob\":\"%s\",\"sex\":\"%s\",\"avatar\":\"https://media.azoomee.com/static/thumbs/oomee_%02d.png\",\"password\":\"\"}",childProfileName.c_str(),childDOB.c_str(),childGender.c_str(),oomeeNumber);
    httpRequestCreator->requestTag = "registerChild";
    httpRequestCreator->createEncryptedPostHttpRequest();
}

void BackEndCaller::onRegisterChildAnswerReceived()
{
    getAvailableChildren();
}
