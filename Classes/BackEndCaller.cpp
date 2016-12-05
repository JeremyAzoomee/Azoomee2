#include "BackEndCaller.h"

#include "network/HttpClient.h"

#include "external/json/document.h"
#include "external/json/writer.h"
#include "external/json/stringbuffer.h"

#include "JWTTool.h"

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

//GETTING AVAILABLE CHILDREN----------------------------------------------------------------


void BackEndCaller::onGetChildrenAnswerReceived(HttpClient *sender, HttpResponse *response)
{
    std::string myResponseString = std::string("");
    
    if (response && response->getResponseData())
    {
        std::vector<char> myResponse = *response->getResponseData();
        myResponseString = std::string(myResponse.begin(), myResponse.end());
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

//GETTING CONTENT

void BackEndCaller::getContent()
{
    std::string requestPath = StringUtils::format("/api/content/v2/user/%s", DataStorage::getInstance()->getChildLoginValue("id").c_str());
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

void BackEndCaller::setCookiesForAndroid(std::string url, std::string cookieString)
{
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    cocos2d::JniMethodInfo methodInfo;
    
    if (! cocos2d::JniHelper::getStaticMethodInfo(methodInfo, "org/cocos2dx/cpp/AppActivity", "setCookies", "(Ljava/lang/String;Ljava/lang/String;)V"))
    {
        return;
    }
    
    jstring jurl = methodInfo.env->NewStringUTF(url.c_str());
    jstring jcookieString = methodInfo.env->NewStringUTF(cookieString.c_str());
    
    methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, jurl, jcookieString);
    methodInfo.env->DeleteLocalRef(methodInfo.classID);
    
    #endif
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

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            
            setCookiesForAndroid("https://media.azoomee.ninja", "cookie = 1"); //DataStorage::getInstance()->dataDownloadCookies);
            
#endif
            modalMessages->stopLoading();
            Director::getInstance()->getRunningScene()->removeChild(modalMessages);
            
            auto baseScene = BaseScene::createScene();
            Director::getInstance()->replaceScene(baseScene);
        }
    }
}

void BackEndCaller::getData()
{
    std::string requestUrl = "https://media.azoomee.ninja/free/f50a74dd-185f-4010-ab6f-b34858b96bcd/video_stream.m3u8";
    
    HttpRequest *request = new HttpRequest();
    request->setRequestType(HttpRequest::Type::GET);
    request->setUrl(requestUrl.c_str());
    
    std::vector<std::string> headers;
    headers.push_back(StringUtils::format("Cookie: %s", DataStorage::getInstance()->dataDownloadCookiesForCpp.c_str()));
    request->setHeaders(headers);
    
    request->setResponseCallback(CC_CALLBACK_2(BackEndCaller::onGetDataAnswerReceived, this));
    request->setTag("GET data");
    HttpClient::getInstance()->send(request);
}

void BackEndCaller::onGetDataAnswerReceived(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    std::string responseString = StringUtils::format("");
    
    if (response && response->getResponseData())
    {
        std::vector<char> myResponse = *response->getResponseHeader();
        responseString = std::string(myResponse.begin(), myResponse.end());
    }
}
