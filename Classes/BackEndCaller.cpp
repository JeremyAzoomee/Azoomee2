USING_NS_CC;

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

#define CI_HOST "api.elb.ci.azoomee.ninja"
#define CI_URL "http://" CI_HOST

using namespace network;
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

//LOGGING IN BY PARENT-------------------------------------------------------------------------------


void BackEndCaller::login(std::string username, std::string password)
{
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
        
    auto childSelectorScene = ChildSelectorScene::createScene();
    Director::getInstance()->replaceScene(childSelectorScene);
}

//CHILDREN LOGIN----------------------------------------------------------------------------------------

void BackEndCaller::childLogin(int childNumber)
{
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
