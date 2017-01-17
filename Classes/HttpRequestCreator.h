#include "cocos2d.h"
#include "network/HttpClient.h"

class HttpRequestCreator : public cocos2d::Ref
{
public:
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
    
    std::string getDateFormatString();
    std::string addLeadingZeroToDateElement(int input);
    
    void createHttpRequest();
    void onHttpRequestAnswerReceived(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    
    int findPositionOfNthString(std::string string, std::string whatToFind, int whichOne);
    std::string getPathFromUrl(std::string url);
    std::string getHostFromUrl(std::string url);
    std::string getUrlParametersFromUrl(std::string url);
};
