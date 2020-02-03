#include "HttpRequestCreator.h"

#include "../JWTSigner/JWTTool.h"
#include "../Data/ConfigStorage.h"
#include "../Analytics/AnalyticsSingleton.h"
#include "../Utils/StringFunctions.h"
#include "../Data/Parent/UserAccountManager.h"
#include "../Crashlytics/CrashlyticsConfig.h"
#include "../Data/AppConfig.h"
#include "API.h"

using namespace cocos2d;
using namespace cocos2d::network;


NS_AZOOMEE_BEGIN

HttpRequestCreator::HttpRequestCreator(HttpRequestCreatorResponseDelegate* delegate) :
  delegate(delegate)
{
    ;
}

void HttpRequestCreator::execute(float timeout)
{
    amountOfFails = 0;
    HttpRequest* request = buildHttpRequest();
    sendRequest(request, timeout);
}

void HttpRequestCreator::resendRequest()
{
	amountOfFails++;
	HttpRequest* request = buildHttpRequest();
	sendRequest(request);
}

void HttpRequestCreator::clearDelegate()
{
    delegate = nullptr;
}

void HttpRequestCreator::setRequestCallback(const cocos2d::network::ccHttpRequestCallback& requestCallback)
{
	_requestCallback = requestCallback;
}

int HttpRequestCreator::getAmountOfFails() const
{
	return amountOfFails;
}

//-----------------------------------------------------All requests below this line are used internally-------------------------------------------------------

int HttpRequestCreator::findPositionOfNthString(const std::string& string, const std::string& whatToFind, int whichOne) const
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

std::string HttpRequestCreator::getPathFromUrl(const std::string& url) const
{
    int from = findPositionOfNthString(url, "/", 3);
    int until = findPositionOfNthString(url, "?", 1);
    int length = until - from;
    
    return(url.substr(from, length)); //returning the path from the url by finding the first slash ( / ) sign after http:// - implemented a method to find 3rd /, as it can be https as well.
}

std::string HttpRequestCreator::getHostFromUrl(const std::string& url) const
{
    int from = findPositionOfNthString(url, "/", 2) + 1;
    int until = findPositionOfNthString(url, "/", 3);
    int length = until - from;
    
    return(url.substr(from, length)); //returning string between the second slash ( / ) (after http://) until the 3rd one (where path starts).
}

std::string HttpRequestCreator::getUrlParametersFromUrl(const std::string& url) const
{
    int from = findPositionOfNthString(url, "?", 1);
    int until = (int)url.length();
    
    if(from == until) return ""; //this means there is no question mark found in url.
    return(url.substr(from + 1)); //from is the position of the question mark, we get substring starting at from + 1 to exclude it from the final result.
}


std::string HttpRequestCreator::getDateFormatString() const                   //This function returns the current date in the required format.
{
    time_t rawtime;
    struct tm * ptm;
    time ( &rawtime );
    ptm = gmtime ( &rawtime );
    
    std::string myDateTime = StringUtils::format("%d-%s-%sT%s:%s:%sZ", ptm->tm_year + 1900, addLeadingZeroToDateElement(ptm->tm_mon + 1).c_str(), addLeadingZeroToDateElement(ptm->tm_mday).c_str(), addLeadingZeroToDateElement(ptm->tm_hour).c_str(), addLeadingZeroToDateElement(ptm->tm_min).c_str(), addLeadingZeroToDateElement(ptm->tm_sec).c_str());
    
    return myDateTime;
}

std::string HttpRequestCreator::addLeadingZeroToDateElement(int input) const               //This function adds a leading 0 to the date element if it has 1 character only.
{
    std::string dateFormat = StringUtils::format("%d", input);
    if(dateFormat.length() == 1)
    {
        dateFormat = StringUtils::format("0%s", dateFormat.c_str());
    }
    
    return dateFormat;
}

std::string HttpRequestCreator::getRequestURL() const
{
    return _requestURL;
}

cocos2d::network::HttpRequest* HttpRequestCreator::buildHttpRequest()                            //The http request is being created from global variables. This method can't be run until setting up all variables, please see usage on top of this file.
{
    std::string hostPrefix = AppConfig::getInstance()->getServerUrlPrefix();
    std::string host;
    
    if(!_url.empty())
    {
        _urlParameters = getUrlParametersFromUrl(_url);
        _requestPath = getPathFromUrl(_url);
        host = getHostFromUrl(_url);
    }
    else
    {
        host = AppConfig::getInstance()->getServerHost();
    }
    
    std::string requestUrl = hostPrefix + host + _requestPath;
    if(!_urlParameters.empty())
    {
        requestUrl = requestUrl + "?" + _urlParameters;
    }
    _requestURL = requestUrl;
    
    HttpRequest* request = new HttpRequest();
    
    if(_method == "POST") request->setRequestType(HttpRequest::Type::POST);
    if(_method == "GET") request->setRequestType(HttpRequest::Type::GET);
    if(_method == "PATCH") request->setRequestType(HttpRequest::Type::PATCH);
    if(_method == "PUT") request->setRequestType(HttpRequest::Type::PUT);
	if(_method == "DELETE") request->setRequestType(HttpRequest::Type::DELETE);
    request->setUrl(requestUrl.c_str());
    
    const char* postData = _requestBody.c_str();
    request->setRequestData(postData, strlen(postData) + 1); //+1 is required to get the termination string. Otherwise random memory garbage can be added to the string by accident.
    std::vector<std::string> headers;
    
    //Add no cache to requests, to avoid caching
    headers.push_back("Cache-Control: no-cache");
    
    if(!_requestBody.empty())
    {
        headers.push_back("Content-Type: application/json;charset=UTF-8");    //Adding content type to header only, if there is data in the request.
    }
    
    if(_encrypted)                                                             //parentLogin (and register parent) is the only nonencrypted call. JWTTool is called unless the request is not coming from login.
    {
        std::string myRequestString;
        
        auto jwtTool = JWTTool();
        jwtTool.setRequestBody(_requestBody);
        jwtTool.setQueryParams(_urlParameters);
        jwtTool.setHost(host);
        jwtTool.setPath(_requestPath);
        jwtTool.setMethod(_method);
        
        
        //if(ConfigStorage::getInstance()->isParentSignatureRequiredForRequest(_requestTag))
        if(_signAsParent)
        {
            jwtTool.setForceParent(true);
        }
        
        myRequestString = jwtTool.buildJWTString();
        
        headers.push_back("x-az-req-datetime: " + getDateFormatString());
        headers.push_back("x-az-auth-token: " + myRequestString);
        
        //add country code to the request headers
        
        headers.push_back("X-AZ-COUNTRYCODE: " + UserAccountManager::getInstance()->getLoggedInParentCountryCode());
    }
    
    headers.push_back(StringUtils::format("x-az-appversion: %s", AppConfig::getInstance()->getVersionInformationForRequestHeader().c_str()));
    
    request->setHeaders(headers);
    
//    for(int i = 0; i < request->getHeaders().size(); i++)
//    {
//        cocos2d::log("HEADERS %s", request->getHeaders().at(i).c_str());
//    }
	
	request->setResponseCallback(_requestCallback);
    request->setTag(_requestTag);
    
    return request;
}

void HttpRequestCreator::sendRequest(cocos2d::network::HttpRequest* request, float timeout)
{
    HttpClient::getInstance()->setTimeoutForConnect(timeout); // This is copied by the created http request object, so next request wont change the value for previous requests
    HttpClient::getInstance()->setTimeoutForRead(timeout);
    
    HttpClient::getInstance()->send(request);
	
	if(_requestTag != API::TagOfflineCheck)
	{
		setCrashlyticsKeyWithString(CrashlyticsConsts::kLastHttpRequestTagKey, _requestTag);
	}
}

void HttpRequestCreator::onHttpRequestAnswerReceived(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{	
    std::string responseHeaderString  = std::string(response->getResponseHeader()->begin(), response->getResponseHeader()->end());
    std::string responseDataString = std::string(response->getResponseData()->begin(), response->getResponseData()->end());
    std::string requestTag = response->getHttpRequest()->getTag();
    
    cocos2d::log("Request tag: %s", requestTag.c_str());
    cocos2d::log("Response code: %ld", response->getResponseCode());
    cocos2d::log("Response header: %s", responseHeaderString.c_str());
    cocos2d::log("Response string: %s", responseDataString.c_str());
	
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
    //cocos2d::log("request body: %s", response->getHttpRequest()->getRequestData());
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
    
    if(response->getResponseCode() != -1)
    {
        AnalyticsSingleton::getInstance()->httpRequestFailed(requestTag, errorCode, StringFunctions::getValueFromHttpResponseHeaderForKey("x-az-qid", responseHeaderString));
    }
    
    if((errorCode == 401)&&(findPositionOfNthString(responseDataString, "Invalid Request Time", 1) != responseDataString.length()))
    {
        errorCode = 2001;
    }

    handleEventAfterError(requestTag, errorCode);
}

void HttpRequestCreator::handleEventAfterError(const std::string& requestTag, long errorCode)
{
    if(delegate != nullptr)
    {
        delegate->onHttpRequestFailed(requestTag, errorCode);
    }
}
  
NS_AZOOMEE_END
