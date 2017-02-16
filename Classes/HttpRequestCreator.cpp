#include "HttpRequestCreator.h"
#include "JWTTool.h"
#include "JWTToolForceParent.h"
#include "BackEndCaller.h"
#include "HQDataParser.h"
#include "ConfigStorage.h"
#include "LoginScene.h"
#include "OnboardingScene.h"
#include "MixPanelSingleton.h"
#include "ChildAccountScene.h"

using namespace cocos2d;
using namespace network;

void HttpRequestCreator::createEncryptedGetHttpRequest()
{
    encrypted = true;
    method = "GET";
    createHttpRequest();
}

void HttpRequestCreator::createEncryptedPostHttpRequest()
{
    encrypted = true;
    method = "POST";
    createHttpRequest();
}

void HttpRequestCreator::createGetHttpRequest()
{
    encrypted = false;
    method = "GET";
    createHttpRequest();
}

void HttpRequestCreator::createPostHttpRequest()
{
    encrypted = false;
    method = "POST";
    createHttpRequest();
}

//-----------------------------------------------------All requests below this line are used internally-------------------------------------------------------

int HttpRequestCreator::findPositionOfNthString(std::string string, std::string whatToFind, int whichOne)
{
    int startSearchPos = 0;
    
    for(int i = 0; i < whichOne; i++)
    {
        if(string.find(whatToFind, startSearchPos) == string.npos)
        {
            return int(string.length());
        }
        else
        {
            startSearchPos = int(string.find(whatToFind, startSearchPos) + 1);
        }
    }
    
    return startSearchPos - 1;
}

std::string HttpRequestCreator::getPathFromUrl(std::string url)
{
    int from = findPositionOfNthString(url, "/", 3);
    int until = findPositionOfNthString(url, "?", 1);
    int length = until - from;
    
    return(url.substr(from, length)); //returning the path from the url by finding the first slash ( / ) sign after http:// - implemented a method to find 3rd /, as it can be https as well.
}

std::string HttpRequestCreator::getHostFromUrl(std::string url)
{
    int from = findPositionOfNthString(url, "/", 2) + 1;
    int until = findPositionOfNthString(url, "/", 3);
    int length = until - from;
    
    return(url.substr(from, length)); //returning string between the second slash ( / ) (after http://) until the 3rd one (where path starts).
}

std::string HttpRequestCreator::getUrlParametersFromUrl(std::string url)
{
    int from = findPositionOfNthString(url, "?", 1);
    int until = (int)url.length();
    
    if(from == until) return ""; //this means there is no question mark found in url.
    return(url.substr(from));
}


std::string HttpRequestCreator::getDateFormatString()                   //This function returns the current date in the required format.
{
    time_t rawtime;
    struct tm * ptm;
    time ( &rawtime );
    ptm = gmtime ( &rawtime );
    
    std::string myDateTime = StringUtils::format("%d-%s-%sT%s:%s:%sZ", ptm->tm_year + 1900, addLeadingZeroToDateElement(ptm->tm_mon + 1).c_str(), addLeadingZeroToDateElement(ptm->tm_mday).c_str(), addLeadingZeroToDateElement(ptm->tm_hour).c_str(), addLeadingZeroToDateElement(ptm->tm_min).c_str(), addLeadingZeroToDateElement(ptm->tm_sec).c_str());
    
    return myDateTime;
}

std::string HttpRequestCreator::addLeadingZeroToDateElement(int input)               //This function adds a leading 0 to the date element if it has 1 character only.
{
    std::string dateFormat = StringUtils::format("%d", input);
    if(dateFormat.length() == 1)
    {
        dateFormat = StringUtils::format("0%s", dateFormat.c_str());
    }
    
    return dateFormat;
}

void HttpRequestCreator::createHttpRequest()                            //The http request is being created from global variables. This method can't be run until setting up all variables, please see usage on top of this file.
{
    std::string host;
    
    if(!url.empty())
    {
        urlParameters = getUrlParametersFromUrl(url);
        requestPath = getPathFromUrl(url);
        host = getHostFromUrl(url);
    }
    else
    {
        host = ConfigStorage::getInstance()->getServerHost();
        
        if(requestPath.empty())
        {
            requestPath = ConfigStorage::getInstance()->getPathForTag(requestTag);
        }
    }
    
    std::string requestUrl = StringUtils::format("https://%s%s", host.c_str(), requestPath.c_str());
    if(!urlParameters.empty()) requestUrl = StringUtils::format("%s?%s", requestUrl.c_str(), urlParameters.c_str());   //In URL we need to add the ?
    
    HttpRequest *request = new HttpRequest();
    
    if(method == "POST") request->setRequestType(HttpRequest::Type::POST);
    if(method == "GET") request->setRequestType(HttpRequest::Type::GET);
    request->setUrl(requestUrl.c_str());
    
    const char *postData = requestBody.c_str();
    request->setRequestData(postData, strlen(postData));
    
    std::vector<std::string> headers;
    
    //Add no cache to requests, to avoid caching
    headers.push_back("Cache-Control: no-cache");
    
    if(!requestBody.empty()) headers.push_back("Content-Type: application/json;charset=UTF-8");    //Adding content type to header only, if there is data in the request.
    
    if(encrypted)                                                             //parentLogin (and register parent) is the only nonencrypted call. JWTTool is called unless the request is not coming from login.
    {
        std::string myRequestString;
        
        if((requestTag == "updateParentPin")||(requestTag == "updateParentActorStatus"))
        {
            auto myJWTTool = JWTToolForceParent::getInstance();
            myRequestString = myJWTTool->buildJWTString(method, requestPath.c_str(), host, urlParameters, requestBody);
        }
        else
        {
            auto myJWTTool = JWTTool::getInstance();
            myRequestString = myJWTTool->buildJWTString(method, requestPath.c_str(), host, urlParameters, requestBody);
        }
        
        const char *reqData = myRequestString.c_str();
        
        headers.push_back(StringUtils::format("x-az-req-datetime: %s", getDateFormatString().c_str()));
        headers.push_back(StringUtils::format("x-az-auth-token: %s", reqData));
    }
    
    request->setHeaders(headers);
    
    request->setResponseCallback(CC_CALLBACK_2(::HttpRequestCreator::onHttpRequestAnswerReceived, this));
    request->setTag(requestTag);
    HttpClient::getInstance()->setTimeoutForConnect(2);
    HttpClient::getInstance()->setTimeoutForRead(2);
    HttpClient::getInstance()->send(request);
}

void HttpRequestCreator::onHttpRequestAnswerReceived(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    CCLOG("Callback called!");
    
    if((response->getResponseCode() == 200)||(response->getResponseCode() == 201))
    {
        std::vector<char> responseHeader = *response->getResponseHeader();
        std::string responseHeaderString = std::string(responseHeader.begin(), responseHeader.end());
        
        std::vector<char> responseData = *response->getResponseData();
        std::string responseDataString = std::string(responseData.begin(), responseData.end());
        
        CCLOG("response code: %ld", response->getResponseCode());
        CCLOG("response string: %s", responseDataString.c_str());
        
        std::string requestTag = response->getHttpRequest()->getTag();
        
        if(requestTag == "getGordon") BackEndCaller::getInstance()->onGetGordonAnswerReceived(responseHeaderString);
        if(requestTag == "childLogin") BackEndCaller::getInstance()->onChildLoginAnswerReceived(responseDataString);
        if(requestTag == "getChildren") BackEndCaller::getInstance()->onGetChildrenAnswerReceived(responseDataString);
        if(requestTag == "parentLogin") BackEndCaller::getInstance()->onLoginAnswerReceived(responseDataString);
        if(requestTag == "registerChild") BackEndCaller::getInstance()->onRegisterChildAnswerReceived();
        if(requestTag == "registerParent") BackEndCaller::getInstance()->onRegisterParentAnswerReceived();
        if(requestTag == "updateParentPin") BackEndCaller::getInstance()->onUpdateParentPinAnswerReceived(responseDataString);
        if(requestTag == "updateParentActorStatus") BackEndCaller::getInstance()->onUpdateParentActorStatusAnswerReceived(responseDataString);
        if(requestTag == "PreviewHOME") HQDataParser::getInstance()->onGetPreviewContentAnswerReceived(responseDataString);
        
        for(int i = 0; i < 6; i++)
        {
            if(requestTag == ConfigStorage::getInstance()->getNameForMenuItem(i)) HQDataParser::getInstance()->onGetContentAnswerReceived(responseDataString, requestTag);
        }
        
        if(requestTag == "GROUP HQ") HQDataParser::getInstance()->onGetContentAnswerReceived(responseDataString, "GROUP HQ");
    }
    else
    {
        std::vector<char> responseData = *response->getResponseData();
        std::string responseDataString = std::string(responseData.begin(), responseData.end());
        
        CCLOG("response string: %s", responseDataString.c_str());
        CCLOG("response code: %ld", response->getResponseCode());

        handleError(response->getHttpRequest()->getTag(), response->getResponseCode(), responseDataString);
    }
}

void HttpRequestCreator::handleError(std::string requestTag, long errorCode, std::string responseString)
{
    if(errorCode == 401)
    {
        if(findPositionOfNthString(responseString, "\"message\":\"Invalid Request Time\"", 0) != responseString.length())
        {
            errorCode = 2001;
        }
    }
    
    if(requestTag == "registerParent")
    {
        MixPanelSingleton::getInstance()->mixPanel_OnboardingAccountCreatedErrorEvent(errorCode);
        Director::getInstance()->replaceScene(OnboardingScene::createScene(errorCode));
    }
    else if(requestTag == "registerChild")
    {
        MixPanelSingleton::getInstance()->mixPanel_childProfileCreatedErrorEvent(errorCode);
        Director::getInstance()->replaceScene(ChildAccountScene::createScene("", errorCode));
    }
    else Director::getInstance()->replaceScene(LoginScene::createScene(errorCode));
}
