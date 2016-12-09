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
    
    //COMMON METHODS
    std::string replaceAll(std::string& str, const std::string& from, const std::string& to);
    std::string getPathFromUrl(std::string url);
    std::string getHostFromUrl(std::string url);
    int findPositionOfNthString(std::string string, std::string whatToFind, int whichOne);
    std::string getDateFormatString();
    std::string getDateElement(int input);
    //END OF COMMON METHODS
    
    
    void login(std::string username, std::string password);
    void onLoginAnswerReceived(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    
    void onGetChildrenAnswerReceived(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    void getAvailableChildren();
    
    void childLogin(int childNumber);
    void onChildLoginAnswerReceived(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    
    void getContent(std::string url, std::string category);
    void onGetContentAnswerReceived(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    
    void getGordon();
    void onGetGordonAnswerReceived(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    
    void displayError(std::string errorMessage);
    
    ModalMessages *modalMessages;
    
};
