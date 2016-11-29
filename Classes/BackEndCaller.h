#include "cocos2d.h"
#include "network/HttpClient.h"
#include "external/json/document.h"
#include "ModalMessages.h" //We have this file included here, because we want to show loading screen on scene when data communication on login (or somewhere else) starts.

class BackEndCaller : public cocos2d::Object
{
    
public:
    /** Returns the shared instance of the Game Manager */
    static BackEndCaller* getInstance(void);
    
public:
    virtual ~BackEndCaller();
    bool init(void);
    
    std::string replaceAll(std::string& str, const std::string& from, const std::string& to);
    
    void login(std::string username, std::string password);
    void onLoginAnswerReceived(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    
    void onGetChildrenAnswerReceived(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    void getAvailableChildren();
    
    void childLogin(int childNumber);
    void onChildLoginAnswerReceived(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    
    void getContent();
    void onGetContentAnswerReceived(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    
    void getGordon();
    void onGetGordonAnswerReceived(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    
    bool parseLoginData(std::string responseData);
    void displayError(std::string errorMessage);
    
    std::string getDateFormatString();
    std::string getDateElement(int input);
    
    rapidjson::Document parentLoginData;
    rapidjson::Document childrenData;
    rapidjson::Document childLoginData;
    rapidjson::Document contentData;
    
    std::string dataDownloadCookies;
    std::vector<std::string> jsonKeys;
    
    ModalMessages *modalMessages;
    
};
