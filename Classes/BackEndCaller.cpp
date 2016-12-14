#include "BackEndCaller.h"

#include "network/HttpClient.h"

#include "external/json/document.h"
#include "external/json/writer.h"
#include "external/json/stringbuffer.h"

#include "JWTTool.h"

#include "OnboardingScene.h"
#include "ChildAccountScene.h"

USING_NS_CC;

#define CI_HOST "api.elb.ci.azoomee.ninja"
#define CI_URL "http://" CI_HOST
#define LOGIN_URL CI_URL"/api/auth/login"

#include "external/json/document.h"
#include "external/json/writer.h"
#include "external/json/stringbuffer.h"
#include "external/json/prettywriter.h"

//Including the following files to be able to change scenes

#include "ChildSelectorScene.h"
#include "BaseScene.h"

//Including DataStorage.h - responsible for storing all login information during the app runtime

#include "DataStorage.h"

/*
 REGISTER PARENT
 
 POST /api/user/v2/adult HTTP/1.1
 Host: api.azoomee.com
 Content-Type: application/json;charset=UTF-8
 Origin: file://
 Cookie: _ga=GA1.2.1873399231.1479295103
 Connection: keep-alive
 Accept: application/json, text/plain, <is star forwardslash star>  */ /*
User-Agent: Mozilla/5.0 (iPhone; CPU iPhone OS 10_1_1 like Mac OS X) AppleWebKit/602.2.14 (KHTML, like Gecko) Mobile/14B100 (4299203760)
Accept-Language: hu-hu
Accept-Encoding: gzip, deflate
Content-Length: 124
{"emailAddress":"infografiakft@gmail.com","over18":"true","termsAccepted":"true","password":"B0Ta1983!","source":"childApp"}
*/

/* 
 ADDING CHILD
 
 POST /api/user/child HTTP/1.1
 Host    api.azoomee.com
 Accept  application/json, text/plain, <is star forwardslash star>  */ /*
Accept-Encoding gzip, deflate
Accept-Language hu-hu
Content-Type    application/json;charset=UTF-8
X-AZ-AUTH-TOKEN eyJhbGciOiJIUzI1NiIsImtpZCI6ImE5NjBhZDFmLWMzYmEtNDBmNC1hNTdmLWI1MThiZTAzYjJiZSJ9.eyJpc3MiOiI4NTVmZmY4Yi01ZmU2LTQyMWEtYTdmOC1jNmI4NDk2ODMyNGUiLCJhdWQiOiJwYXJlbnQuYXpvb21lZS5jb20iLCJhcHBsaWNhdGlvbkNsYWltIjp7InNpZ25hdHVyZSI6InVzSmkvU2pZYU54bTA2TTZWQlVWZXpLNC9HVlBabkRncDRvbTJQbzU5WjQ9IiwicGFyZW50S2V5IjpudWxsfX0=.n91fFbovCyjXgRj1OnbFZ0twqm9FR3Ro5ftPcqXrFkQ=
Origin  https://parent.azoomee.com
Content-Length  133
X-AZ-INAPP  TRUE
User-Agent  Mozilla/5.0 (iPhone; CPU iPhone OS 10_1_1 like Mac OS X) AppleWebKit/602.2.14 (KHTML, like Gecko) Mobile/14B100 (4300254720)
Referer https://parent.azoomee.com/
X-AZ-REQ-DATETIME   2016-12-13T09:37:17Z
Connection  keep-alive
                                                                        
{"profileName":"Bence","dob":"2010-08-12","sex":"MALE","avatar":"https://media.azoomee.com/static/thumbs/oomee_00.png","password":""}
 
 */


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

std::string BackEndCaller::replaceAll(std::string& str, const std::string& from, const std::string& to) {
    if(from.empty())
        return "";
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
    
    return str;
}

std::string BackEndCaller::getDateElement(int input)
{
    std::string dateFormat = StringUtils::format("%d", input);
    if(dateFormat.length() == 1)
    {
        dateFormat = StringUtils::format("0%s", dateFormat.c_str());
    }
    
    return dateFormat;
}

std::string BackEndCaller::getDateFormatString()
{
    time_t rawtime;
    struct tm * ptm;
    time ( &rawtime );
    
    ptm = gmtime ( &rawtime );
    
    std::string myDateTime = StringUtils::format("%d-%s-%sT%s:%s:%sZ", ptm->tm_year + 1900, getDateElement(ptm->tm_mon + 1).c_str(), getDateElement(ptm->tm_mday).c_str(), getDateElement(ptm->tm_hour).c_str(), getDateElement(ptm->tm_min).c_str(), getDateElement(ptm->tm_sec).c_str());
    
    
    
    return myDateTime;
}

void BackEndCaller::displayError(std::string errorMessage)
{
    auto myMessage = (Label *)Director::getInstance()->getRunningScene()->getChildByTag(0)->getChildByTag(3);
    myMessage->setString(errorMessage);
}

//LOGGING IN BY PARENT--------------------------------------------------------------------------

void BackEndCaller::onLoginAnswerReceived(HttpClient *sender, HttpResponse *response)
{
    if (response && response->getResponseCode() == 200 && response->getResponseData())
    {
        CCLOG("\n\n\nweb connection success");
        std::vector<char> myResponse = *response->getResponseData();
        
        std::string myResponseString = std::string(myResponse.begin(), myResponse.end());
        if(DataStorage::getInstance()->parseParentLoginData(myResponseString)) getAvailableChildren();
    }
    else
    {
        CCLOG("Response code: %ld", response->getResponseCode());
    }
}

void BackEndCaller::login(std::string username, std::string password)
{
    modalMessages = ModalMessages::create();
    Director::getInstance()->getRunningScene()->addChild(modalMessages);
    modalMessages->startLoading();
    
    HttpRequest *request = new HttpRequest();
    request->setRequestType(HttpRequest::Type::POST);
    request->setUrl(LOGIN_URL);
    
    std::string myPostString = StringUtils::format("{\"password\": \"%s\",\"userName\": \"%s\",\"appType\": \"CHILD_APP\"}", password.c_str(), username.c_str());
    //std::string myPostString = StringUtils::format("{\"password\": \"%s\",\"userName\": \"%s\"}", password.c_str(), username.c_str());
    const char *postData = myPostString.c_str();
    
    request->setRequestData(postData, strlen(postData));
    
    CCLOG("request data is: %s", request->getRequestData());
    
    std::vector<std::string> headers;
    headers.push_back("Content-Type: application/json;charset=utf-8");
    request->setHeaders(headers);
    
    request->setResponseCallback(CC_CALLBACK_2(BackEndCaller::onLoginAnswerReceived, this));
    request->setTag("login");
    HttpClient::getInstance()->send(request);
}

//REGISTER PARENT---------------------------------------------------------------------------

void BackEndCaller::registerParent(std::string emailAddress, std::string password, std::string pinNumber)
{
    //Save emailAddress and password, so onRegisterParentAnswerReceived can login after success
    this->registerParentUsername = emailAddress;
    this->registerParentPassword = password;
    
    modalMessages = ModalMessages::create();
    Director::getInstance()->getRunningScene()->addChild(modalMessages);
    modalMessages->startLoading();
    
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
        auto _OnboardingScene = OnboardingScene::createSceneWithErrorCode(response->getResponseCode());
        Director::getInstance()->replaceScene(_OnboardingScene);
    }
    
}
//GETTING AVAILABLE CHILDREN----------------------------------------------------------------


void BackEndCaller::onGetChildrenAnswerReceived(HttpClient *sender, HttpResponse *response)
{
    std::string myResponseString = std::string("");
    
    if (response && response->getResponseData())
    {
        std::vector<char> myResponse = *response->getResponseData();
        myResponseString = std::string(myResponse.begin(), myResponse.end());
        CCLOG("children data: %s", myResponseString.c_str());
    }
    
    if(response->getResponseCode() == 200)
    {
        CCLOG("GET CHILDREN SUCCESS");
        DataStorage::getInstance()->parseAvailableChildren(myResponseString);
        modalMessages->stopLoading();
        Director::getInstance()->getRunningScene()->removeChild(modalMessages);
        
        auto childSelectorScene = ChildSelectorScene::createScene();
        Director::getInstance()->replaceScene(childSelectorScene);
    }
    else
    {
        CCLOG("GET CHILDREN FAIL Response code: %ld", response->getResponseCode());
        CCLOG("GET CHILDREN FAIL Response: %s", myResponseString.c_str());
    }
}


void BackEndCaller::getAvailableChildren()
{
    std::string requestPath = StringUtils::format("/api/user/adult/%s/owns", DataStorage::getInstance()->getParentLoginValue("id").c_str());
    std::string requestUrl = StringUtils::format(CI_URL"%s?expand=true", requestPath.c_str());
    
    HttpRequest *request = new HttpRequest();
    request->setRequestType(HttpRequest::Type::GET);
    request->setUrl(requestUrl.c_str());
    
    
    auto myJWTTool = JWTTool::getInstance();
    
    //std::string buildJWTString(std::string method, std::string path, std::string host, std::string queryParams, std::string requestBody);
    
    std::string myRequestString = myJWTTool->buildJWTString("GET", requestPath.c_str(), CI_HOST, "expand=true", "");
    //To be commented out when using local debug std::string myRequestString = myJWTTool->buildJWTString("GET", "/api/test", CI_HOST);
    const char *reqData = myRequestString.c_str();
    
    request->setRequestData(reqData, strlen(reqData));
    
    CCLOG("request data is: %s", request->getRequestData());
    
    std::vector<std::string> headers;
    headers.push_back(StringUtils::format("x-az-req-datetime: %s", getDateFormatString().c_str()));
    //headers.push_back("Content-Type: application/json;charset=UTF-8"); //If no data, no content type in headers
    headers.push_back(StringUtils::format("x-az-auth-token: %s", reqData));
    
    request->setHeaders(headers);
    
    request->setResponseCallback(CC_CALLBACK_2(BackEndCaller::onGetChildrenAnswerReceived, this));
    request->setTag("GET children");
    HttpClient::getInstance()->send(request);
}

//CHILDREN LOGIN-------------------------------------------------------------------------------

void BackEndCaller::childLogin(int childNumber)
{
    modalMessages = ModalMessages::create();
    Director::getInstance()->getRunningScene()->addChild(modalMessages);
    modalMessages->startLoading();
    
    std::string requestPath = "/api/auth/switchProfile";
    std::string requestUrl = StringUtils::format(CI_URL"%s", requestPath.c_str());
    
    HttpRequest *request = new HttpRequest();
    request->setRequestType(HttpRequest::Type::POST);
    request->setUrl(requestUrl.c_str());
    
    std::string requestBody = StringUtils::format("{\"userName\": \"%s\", \"password\": \"\"}", DataStorage::getInstance()->getValueFromOneAvailableChild(childNumber, "profileName").c_str());
    
    const char *postData = requestBody.c_str();
    request->setRequestData(postData, strlen(postData));
    
    auto myJWTTool = JWTTool::getInstance();
    std::string myRequestString = myJWTTool->buildJWTString("POST", requestPath.c_str(), CI_HOST, "", requestBody);
    const char *reqData = myRequestString.c_str();
    
    //request->setRequestData(reqData, strlen(reqData));
    
    std::vector<std::string> headers;
    headers.push_back("Content-Type: application/json;charset=UTF-8");
    headers.push_back(StringUtils::format("x-az-req-datetime: %s", getDateFormatString().c_str()));
    headers.push_back(StringUtils::format("x-az-auth-token: %s", reqData));
    
    
    //headers.push_back(StringUtils::format("%s", requestBody.c_str()));
    
    request->setHeaders(headers);
    
    request->setResponseCallback(CC_CALLBACK_2(BackEndCaller::onChildLoginAnswerReceived, this));
    request->setTag("Child login");
    HttpClient::getInstance()->send(request);

}

void BackEndCaller::onChildLoginAnswerReceived(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    std::string myResponseString = StringUtils::format("");
    
    if (response && response->getResponseData())
    {
        std::vector<char> myResponse = *response->getResponseData();
        
        for(int i = 0; i < myResponse.size(); i++)
        {
            myResponseString = StringUtils::format("%s%c", myResponseString.c_str(), myResponse[i]);
        }
        
        CCLOG("%s", myResponseString.c_str());
    }
    
    if(response->getResponseCode() == 200)
    {
        CCLOG("CHILDREN LOGIN SUCCESS");
        DataStorage::getInstance()->parseChildLoginData(myResponseString);
        getContent();
    }
    else
    {
        CCLOG("CHILDREN LOGIN FAIL Response code: %ld", response->getResponseCode());
        CCLOG("CHILDREN LOGIN FAIL Response: %s", myResponseString.c_str());
    }
    
    
}

//REGISTER CHILD----------------------------------------------------------------------------

void BackEndCaller::registerChild(std::string childProfileName, std::string childGender, std::string childDOB, int oomeeNumber)
{
    modalMessages = ModalMessages::create();
    Director::getInstance()->getRunningScene()->addChild(modalMessages);
    modalMessages->startLoading();
    
    std::string requestPath = "/api/user/child";
    std::string requestUrl = StringUtils::format(CI_URL"%s", requestPath.c_str());
    
    HttpRequest *request = new HttpRequest();
    request->setRequestType(HttpRequest::Type::POST);
    request->setUrl(requestUrl.c_str());
    
    std::string requestBody = StringUtils::format("{\"profileName\":\"%s\",\"dob\":\"%s\",\"sex\":\"%s\",\"avatar\":\"https://media.azoomee.com/static/thumbs/oomee_%02d.png\",\"password\":\"\"}",childProfileName.c_str(),childDOB.c_str(),childGender.c_str(),oomeeNumber);
    CCLOG("This is the request body: %s", requestBody.c_str());
    
    const char *postData = requestBody.c_str();
    request->setRequestData(postData, strlen(postData));
    
    auto myJWTTool = JWTTool::getInstance();
    std::string myRequestString = myJWTTool->buildJWTString("POST", requestPath.c_str(), CI_HOST, "", requestBody);
    const char *reqData = myRequestString.c_str();
    
    //request->setRequestData(reqData, strlen(reqData));
    
    std::vector<std::string> headers;
    headers.push_back("Content-Type: application/json;charset=UTF-8");
    headers.push_back(StringUtils::format("x-az-req-datetime: %s", getDateFormatString().c_str()));
    headers.push_back(StringUtils::format("x-az-auth-token: %s", reqData));
    
    
    //headers.push_back(StringUtils::format("%s", requestBody.c_str()));
    
    request->setHeaders(headers);
    
    request->setResponseCallback(CC_CALLBACK_2(BackEndCaller::onRegisterChildLoginAnswerReceived, this));
    request->setTag("Add child");
    HttpClient::getInstance()->send(request);
    
}

void BackEndCaller::onRegisterChildLoginAnswerReceived(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    std::string myResponseString = StringUtils::format("");
    
    if (response && response->getResponseData())
    {
        std::vector<char> myResponse = *response->getResponseData();
        
        for(int i = 0; i < myResponse.size(); i++)
        {
            myResponseString = StringUtils::format("%s%c", myResponseString.c_str(), myResponse[i]);
        }
        
        CCLOG("%s", myResponseString.c_str());
    }
    
    if(response->getResponseCode() == 201)
    {
        CCLOG("CHILDREN REGISTRATION SUCCESS");
        getAvailableChildren();
    }
    else
    {
        CCLOG("CHILDREN REGISTRATION FAIL Response code: %ld", response->getResponseCode());
        CCLOG("CHILDREN REGISTRATION FAIL Response: %s", myResponseString.c_str());
        
        //Restart the ChildSelectorScene with error
        auto _ChildAccountScene = ChildAccountScene::createSceneWithErrorCode(response->getResponseCode());
        Director::getInstance()->replaceScene(_ChildAccountScene);
    }
    
    
}

//GETTING CONTENT

void BackEndCaller::getContent()
{
    std::string requestPath = StringUtils::format("/api/electricdreams/view/categories/home/%s", DataStorage::getInstance()->getChildLoginValue("id").c_str());
    std::string requestUrl = StringUtils::format(CI_URL"%s", requestPath.c_str());
    
    HttpRequest *request = new HttpRequest();
    request->setRequestType(HttpRequest::Type::GET);
    request->setUrl(requestUrl.c_str());
    
    auto myJWTTool = JWTTool::getInstance();
    
    //std::string buildJWTString(std::string method, std::string path, std::string host, std::string queryParams, std::string requestBody);
    
    std::string myRequestString = myJWTTool->buildJWTString("GET", requestPath.c_str(), CI_HOST, "", "");
    const char *reqData = myRequestString.c_str();
    
    request->setRequestData(reqData, strlen(reqData));
    
    CCLOG("request data is: %s", request->getRequestData());
    
    std::vector<std::string> headers;
    headers.push_back(StringUtils::format("x-az-req-datetime: %s", getDateFormatString().c_str()));
    headers.push_back(StringUtils::format("x-az-auth-token: %s", reqData));
    
    request->setHeaders(headers);
    
    request->setResponseCallback(CC_CALLBACK_2(BackEndCaller::onGetContentAnswerReceived, this));
    request->setTag("GET content");
    HttpClient::getInstance()->send(request);

}

void BackEndCaller::onGetContentAnswerReceived(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    std::string responseString = "";
    
    if (response && response->getResponseData())
    {
        std::vector<char> myResponse = *response->getResponseData();
        responseString = std::string(myResponse.begin(), myResponse.end());
        CCLOG("get content data: %s", responseString.c_str());
    }
    
    if(response->getResponseCode() == 200)
    {
        CCLOG("GET CONTENT SUCCESS");
        
        DataStorage::getInstance()->parseContentData(responseString);
        
        getGordon();
    }
    else
    {
        CCLOG("GET CONTENT FAIL Response code: %ld", response->getResponseCode());
        CCLOG("GET CONTENT FAIL Response: %s", responseString.c_str());
    }
}

//GETTING GORDON.PNG

void BackEndCaller::getGordon()
{
    std::string requestPath = "/api/porthole/pixel/gordon.png";
    std::string requestUrl = StringUtils::format(CI_URL"%s?userid=%s&sessionid=%s", requestPath.c_str(), DataStorage::getInstance()->getParentOrChildLoginValue("id").c_str(), DataStorage::getInstance()->getParentOrChildLoginValue("cdn-sessionid").c_str());
    
    auto myJWTTool = JWTTool::getInstance();
    
    std::string myRequestString = myJWTTool->buildJWTString("GET", requestPath.c_str(), CI_HOST, StringUtils::format("userid=%s&sessionid=%s", requestPath.c_str(), DataStorage::getInstance()->getParentOrChildLoginValue("cdn-sessionid").c_str()), "");
    
    HttpRequest *request = new HttpRequest();
    request->setRequestType(HttpRequest::Type::GET);
    request->setUrl(requestUrl.c_str());
    
    //request->setRequestData(reqData, strlen(reqData));
    
    CCLOG("request data is: %s", request->getRequestData());
    
    std::vector<std::string> headers;
    headers.push_back(StringUtils::format("x-az-req-datetime: %s", getDateFormatString().c_str()));
    headers.push_back(StringUtils::format("x-az-auth-token: %s", myRequestString.c_str()));
    
    request->setHeaders(headers);
    
    request->setResponseCallback(CC_CALLBACK_2(BackEndCaller::onGetGordonAnswerReceived, this));
    request->setTag("GET content");
    HttpClient::getInstance()->send(request);
    
}

void BackEndCaller::onGetGordonAnswerReceived(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    std::string responseString = StringUtils::format("");
    
    if (response && response->getResponseData())
    {
        std::vector<char> myResponse = *response->getResponseHeader();
        responseString = std::string(myResponse.begin(), myResponse.end());
        
        if(DataStorage::getInstance()->parseDownloadCookies(responseString))
        {
            modalMessages->stopLoading();
            Director::getInstance()->getRunningScene()->removeChild(modalMessages);
            
            auto baseScene = BaseScene::createScene();
            Director::getInstance()->replaceScene(baseScene);
        }
    }
}
