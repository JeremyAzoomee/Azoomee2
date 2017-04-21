#include "HttpRequestCreator.h"
#include <AzoomeeCommon/JWTSigner/JWTTool.h>
#include <AzoomeeCommon/JWTSigner/JWTToolForceParent.h>
#include "BackEndCaller.h"
#include "HQDataParser.h"
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include "LoginScene.h"
#include "OnboardingScene.h"
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include "ChildAccountScene.h"
#include <AzoomeeCommon/Data/Child/ChildDataParser.h>
#include "RoutePaymentSingleton.h"
#include "GooglePaymentSingleton.h"
#include "AmazonPaymentSingleton.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
 #include "ApplePaymentSingleton.h"
#endif

using namespace cocos2d;
using namespace network;
using namespace Azoomee;


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
        
        const char *reqData = myRequestString.c_str();
        
        headers.push_back(StringUtils::format("x-az-req-datetime: %s", getDateFormatString().c_str()));
        headers.push_back(StringUtils::format("x-az-auth-token: %s", reqData));
    }
    
    headers.push_back(StringUtils::format("x-az-appversion: %s", ConfigStorage::getInstance()->getVersionNumberWithPlatform().c_str()));
    
    request->setHeaders(headers);
    
    request->setResponseCallback(CC_CALLBACK_2(::HttpRequestCreator::onHttpRequestAnswerReceived, this));
    request->setTag(requestTag);
    HttpClient::getInstance()->setTimeoutForConnect(2);
    HttpClient::getInstance()->setTimeoutForRead(2);
    HttpClient::getInstance()->send(request);
}

void HttpRequestCreator::onHttpRequestAnswerReceived(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    if((response->getResponseCode() == 200)||(response->getResponseCode() == 201))
    {
        std::vector<char> responseHeader = *response->getResponseHeader();
        std::string responseHeaderString = std::string(responseHeader.begin(), responseHeader.end());
        
        std::vector<char> responseData = *response->getResponseData();
        std::string responseDataString = std::string(responseData.begin(), responseData.end());
        
        CCLOG("request tag: %s", requestTag.c_str());
        CCLOG("request body: %s", response->getHttpRequest()->getRequestData());
        CCLOG("response code: %ld", response->getResponseCode());
        CCLOG("response header: %s", responseHeaderString.c_str());
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
        if(requestTag == "iapAmazonPaymentMade") AmazonPaymentSingleton::getInstance()->onAmazonPaymentMadeAnswerReceived(responseDataString);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        if(requestTag == "iapApplePaymentMade")
            ApplePaymentSingleton::getInstance()->onAnswerReceived(responseDataString);
#endif
        if(requestTag == "iabGooglePaymentMade") GooglePaymentSingleton::getInstance()->onGooglePaymentVerificationAnswerReceived(responseDataString);
        if(requestTag == "updateBilling") BackEndCaller::getInstance()->onUpdateBillingDataAnswerReceived(responseDataString);
        
        for(int i = 0; i < 6; i++)
        {
            if(requestTag == ConfigStorage::getInstance()->getNameForMenuItem(i)) HQDataParser::getInstance()->onGetContentAnswerReceived(responseDataString, requestTag);
        }
        
        if(requestTag == "GROUP HQ") HQDataParser::getInstance()->onGetContentAnswerReceived(responseDataString, "GROUP HQ");
    }
    else
    {
        handleError(response);
    }
}

void HttpRequestCreator::handleError(network::HttpResponse *response)
{
    std::vector<char> responseData = *response->getResponseData();
    std::string responseString = std::string(responseData.begin(), responseData.end());
    std::string requestTag = response->getHttpRequest()->getTag();
    long errorCode = response->getResponseCode();
    
    CCLOG("request tag: %s", requestTag.c_str());
    CCLOG("request body: %s", response->getHttpRequest()->getRequestData());
    CCLOG("response string: %s", responseString.c_str());
    CCLOG("response code: %ld", response->getResponseCode());
    
    //-----------------------Handle error code--------------------------
    
    if(amountOfFails < 2)
    {
        amountOfFails++;
        createHttpRequest();
        return;
    }
    
    if((errorCode == 401)&&(findPositionOfNthString(responseString, "Invalid Request Time", 1) != responseString.length())) errorCode = 2001;
    
    handleEventAfterError(requestTag, errorCode);
}

void HttpRequestCreator::handleEventAfterError(std::string requestTag, long errorCode)
{
    HttpClient::getInstance()->destroyInstance();
    
    std::map<std::string, Scene*> returnMap;
    
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
        Director::getInstance()->replaceScene(LoginScene::createScene(errorCode));
        return;
    }
    
    if(requestTag == "getChildren")
    {
        Director::getInstance()->replaceScene(LoginScene::createScene(errorCode));
        return;
    }
    
    if(requestTag == "iapAmazonPaymentMade")
    {
        CCLOG("IAP Failed with Errorcode: %ld", errorCode);
        AnalyticsSingleton::getInstance()->iapBackEndRequestFailedEvent(errorCode);
        AmazonPaymentSingleton::getInstance()->backendRequestFailed();
        return;
    }
    if(requestTag == "iapApplePaymentMade")
    {
        CCLOG("IAP Failed with Errorcode: %ld", errorCode);
        AnalyticsSingleton::getInstance()->iapBackEndRequestFailedEvent(errorCode);
        
        #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            ApplePaymentSingleton::getInstance()->backendRequestFailed(errorCode);
        #endif
        return;
    }
    
    if(requestTag == "iabGooglePaymentMade")
    {
        CCLOG("IAP Failed with Errorcode: %ld", errorCode);
        AnalyticsSingleton::getInstance()->iapBackEndRequestFailedEvent(errorCode);
        GooglePaymentSingleton::getInstance()->backendRequestFailed();
        return;
    }
    
    ChildDataParser::getInstance()->setChildLoggedIn(false);
    BackEndCaller::getInstance()->getAvailableChildren();
}
