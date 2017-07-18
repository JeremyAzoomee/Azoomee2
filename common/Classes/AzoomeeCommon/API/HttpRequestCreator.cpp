#include "HttpRequestCreator.h"

#include "../JWTSigner/JWTTool.h"
#include "../JWTSigner/JWTToolForceParent.h"
#include "../Data/ConfigStorage.h"
#include "../Analytics/AnalyticsSingleton.h"
#include "../Utils/StringFunctions.h"

using namespace cocos2d;
using namespace cocos2d::network;


NS_AZOOMEE_BEGIN

HttpRequestCreator::HttpRequestCreator(HttpRequestCreatorResponseDelegate* delegate) :
  delegate(delegate)
{
    ;
}

void HttpRequestCreator::execute()
{
    amountOfFails = 0;
    HttpRequest* request = buildHttpRequest();
    sendRequest(request);
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

cocos2d::network::HttpRequest* HttpRequestCreator::buildHttpRequest()                            //The http request is being created from global variables. This method can't be run until setting up all variables, please see usage on top of this file.
{
    std::string hostPrefix = ConfigStorage::getInstance()->getServerUrlPrefix();
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
    
    std::string requestUrl =  hostPrefix + host + requestPath;
    if(!urlParameters.empty()) requestUrl = requestUrl + "?" + urlParameters;
    
    HttpRequest* request = new HttpRequest();
    
    if(method == "POST") request->setRequestType(HttpRequest::Type::POST);
    if(method == "GET") request->setRequestType(HttpRequest::Type::GET);
    if(method == "PATCH") request->setRequestType(HttpRequest::Type::PATCH);
    if(method == "PUT") request->setRequestType(HttpRequest::Type::PUT);
    request->setUrl(requestUrl.c_str());
    
    const char* postData = requestBody.c_str();
    request->setRequestData(postData, strlen(postData));
    
    std::vector<std::string> headers;
    
    //Add no cache to requests, to avoid caching
    headers.push_back("Cache-Control: no-cache");
    
    if(!requestBody.empty()) headers.push_back("Content-Type: application/json;charset=UTF-8");    //Adding content type to header only, if there is data in the request.
    
    if(encrypted)                                                             //parentLogin (and register parent) is the only nonencrypted call. JWTTool is called unless the request is not coming from login.
    {
        std::string myRequestString;
        
        if(ConfigStorage::getInstance()->isParentSignatureRequiredForRequest(requestTag))
        {
            auto myJWTTool = JWTToolForceParent::getInstance();
            myRequestString = myJWTTool->buildJWTString(method, requestPath.c_str(), host, urlParameters, requestBody);
        }
        else
        {
            auto myJWTTool = JWTTool::getInstance();
            myRequestString = myJWTTool->buildJWTString(method, requestPath.c_str(), host, urlParameters, requestBody);
        }
        
        headers.push_back("x-az-req-datetime: " + getDateFormatString());
        headers.push_back("x-az-auth-token: " + myRequestString);
    }
    
    headers.push_back(StringUtils::format("x-az-appversion: %s", ConfigStorage::getInstance()->getVersionNumberWithPlatform().c_str()));
    
    request->setHeaders(headers);
    
    for(int i = 0; i < request->getHeaders().size(); i++)
    {
        cocos2d::log("%s", request->getHeaders().at(i).c_str());
    }
    
    request->setResponseCallback(CC_CALLBACK_2(HttpRequestCreator::onHttpRequestAnswerReceived, this));
    request->setTag(requestTag);
    
    return request;
}

void HttpRequestCreator::sendRequest(cocos2d::network::HttpRequest* request)
{
    HttpClient::getInstance()->setTimeoutForConnect(5);
    HttpClient::getInstance()->setTimeoutForRead(10);
    
    if(ConfigStorage::getInstance()->isImmediateRequestSendingRequired(requestTag)) HttpClient::getInstance()->sendImmediate(request);
    else HttpClient::getInstance()->send(request);
}

void HttpRequestCreator::onHttpRequestAnswerReceived(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    std::string responseHeaderString  = std::string(response->getResponseHeader()->begin(), response->getResponseHeader()->end());
    std::string responseDataString = std::string(response->getResponseData()->begin(), response->getResponseData()->end());
    std::string requestTag = response->getHttpRequest()->getTag();
    
    cocos2d::log("request tag: %s", requestTag.c_str());
    cocos2d::log("request body: %s", response->getHttpRequest()->getRequestData());
    cocos2d::log("response code: %ld", response->getResponseCode());
    cocos2d::log("response header: %s", responseHeaderString.c_str());
    cocos2d::log("response string: %s", responseDataString.c_str());
    
    if((response->getResponseCode() == 200)||(response->getResponseCode() == 201)||(response->getResponseCode() == 204))
    {
        if(delegate != nullptr)
        {
            delegate->onHttpRequestSuccess(requestTag, responseHeaderString, responseDataString);
        }
    }
    else
    {
        handleError(response);
    }
}

void HttpRequestCreator::handleError(network::HttpResponse *response)
{
    const std::string& responseHeaderString  = std::string(response->getResponseHeader()->begin(), response->getResponseHeader()->end());
    const std::string& responseDataString = std::string(response->getResponseData()->begin(), response->getResponseData()->end());
    const std::string& requestTag = response->getHttpRequest()->getTag();
    long errorCode = response->getResponseCode();
    
    cocos2d::log("request tag: %s", requestTag.c_str());
    cocos2d::log("request body: %s", response->getHttpRequest()->getRequestData());
    cocos2d::log("response string: %s", responseDataString.c_str());
    cocos2d::log("response code: %ld", response->getResponseCode());
    
    //-----------------------Handle error code--------------------------
    
    if(amountOfFails < 2)
    {
        amountOfFails++;
        HttpRequest* request = buildHttpRequest();
        sendRequest(request);
        return;
    }
    
    if(response->getResponseCode() != -1) AnalyticsSingleton::getInstance()->httpRequestFailed(requestTag, errorCode, getQidFromResponseHeader(responseHeaderString));
    
    if((errorCode == 401)&&(findPositionOfNthString(responseDataString, "Invalid Request Time", 1) != responseDataString.length())) errorCode = 2001;

    handleEventAfterError(requestTag, errorCode);
}

void HttpRequestCreator::handleEventAfterError(const std::string& requestTag, long errorCode)
{
    if(delegate != nullptr)
        delegate->onHttpRequestFailed(requestTag, errorCode);
}

std::string HttpRequestCreator::getQidFromResponseHeader(std::string responseHeaderString)
{
    const std::vector<std::string>& responseHeaderVector = splitStringToVector(responseHeaderString, "\n");
    for(int i = 0; i < responseHeaderVector.size(); i++)
    {
        if(responseHeaderVector.at(i).compare(0, 9, "x-az-qid:") == 0)
        {
            return splitStringToVector(responseHeaderVector.at(i), ": ").back();
        }
    }
    
    return "null";
}
  
NS_AZOOMEE_END
