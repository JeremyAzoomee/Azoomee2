#ifndef AzoomeeCommon_HttpRequestCreator_h
#define AzoomeeCommon_HttpRequestCreator_h

#include <cocos/cocos2d.h>
#include <cocos/network/HttpClient.h>


namespace Azoomee
{

/**
 * A delegate to respond to success or failure responses to a HttpRequest
 * using the HttpRequestCreator.
 */
struct HttpRequestCreatorResponseDelegate
{
    virtual void onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body) = 0;
    virtual void onHttpRequestFailed(const std::string& requestTag, long errorCode) = 0;
};


class HttpRequestCreator : public cocos2d::Ref
{
public:
    
    HttpRequestCreator(HttpRequestCreatorResponseDelegate* delegate);
    
    void createEncryptedGetHttpRequest();
    void createEncryptedPostHttpRequest();
    void createPostHttpRequest();
    void createGetHttpRequest();
    
    std::string requestBody;
    std::string urlParameters;
    std::string requestTag;
    std::string method;
    std::string requestPath;
    std::string url;
    bool encrypted;
    
private:
    // The delegate for callbacks on success/failure
    HttpRequestCreatorResponseDelegate* delegate = nullptr;
    
    std::string getDateFormatString();
    std::string addLeadingZeroToDateElement(int input);
    
    void createHttpRequest();
    void onHttpRequestAnswerReceived(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    
    int findPositionOfNthString(std::string string, std::string whatToFind, int whichOne);
    std::string getPathFromUrl(std::string url);
    std::string getHostFromUrl(std::string url);
    std::string getUrlParametersFromUrl(std::string url);
    
    void handleError(cocos2d::network::HttpResponse *response);   //std::string requestTag, long errorCode, std::string responseString);
    
    //adding retry ability to error handling, so as two level step back - step1: kidselector, step2: main login
    int amountOfFails;
    void handleEventAfterError(const std::string& requestTag, long errorCode);
};
  
}

#endif
