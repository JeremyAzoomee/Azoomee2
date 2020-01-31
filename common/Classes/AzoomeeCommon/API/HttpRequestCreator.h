#ifndef AzoomeeCommon_HttpRequestCreator_h
#define AzoomeeCommon_HttpRequestCreator_h

#include "../Azoomee.h"
#include <cocos/cocos2d.h>
#include <cocos/network/HttpClient.h>


NS_AZOOMEE_BEGIN

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
    
    // Build and execute the request
    void execute(float timeout = 10.0f);
    // Build and return the request only, without sending it
    cocos2d::network::HttpRequest* buildHttpRequest();
    // Send the specified request
    void sendRequest(cocos2d::network::HttpRequest* request, float timeout = 10.0f);
	
	void resendRequest();
	
    void clearDelegate();
	
	void setRequestCallback(const cocos2d::network::ccHttpRequestCallback& requestCallback);
	
	int getAmountOfFails() const;
    std::string getRequestURL() const;
	
    std::string _requestBody = "";
    std::string _urlParameters = "";
    std::string _requestTag = "";
    std::string _method = "GET";
    std::string _requestPath = "";
    std::string _url = "";
    bool _encrypted = false;
    bool _signAsParent = false;
    
private:
    // The full request URL, populated after buildHttpRequest is called
    std::string _requestURL;
    
    // The delegate for callbacks on success/failure
    HttpRequestCreatorResponseDelegate* delegate = nullptr;
	cocos2d::network::ccHttpRequestCallback _requestCallback = nullptr;
    std::string getDateFormatString() const;
    std::string addLeadingZeroToDateElement(int input) const;
    
    void onHttpRequestAnswerReceived(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    
    int findPositionOfNthString(const std::string& string, const std::string& whatToFind, int whichOne) const;
    std::string getPathFromUrl(const std::string& url) const;
    std::string getHostFromUrl(const std::string& url) const;
    std::string getUrlParametersFromUrl(const std::string& url) const;
    
    void handleError(cocos2d::network::HttpResponse *response);   //std::string requestTag, long errorCode, std::string responseString);
    
    //adding retry ability to error handling, so as two level step back - step1: kidselector, step2: main login
    int amountOfFails;
    void handleEventAfterError(const std::string& requestTag, long errorCode);
};

NS_AZOOMEE_END

#endif
