#include "cocos2d.h"
#include "network/HttpClient.h"
#include "external/json/document.h"
#include "ModalMessages.h" //We have this file included here, because we want to show loading screen on scene when data communication on login (or somewhere else) starts.

class BackEndCaller : public cocos2d::Ref
{
    
public:
    /** Returns the shared instance of the Game Manager */
    static BackEndCaller* getInstance(void);
    
public:
    virtual ~BackEndCaller();
    bool init(void);
    
    void login(std::string username, std::string password);
    void onLoginAnswerReceived(std::string responseString);
    
    void onGetChildrenAnswerReceived(std::string responseString);
    void getAvailableChildren();
    
    void childLogin(int childNumber);
    void onChildLoginAnswerReceived(std::string responseString);
    
    void getGordon();
    void onGetGordonAnswerReceived(std::string responseString);
    
    void displayLoadingScreen();
    void hideLoadingScreen();
    
    ModalMessages *modalMessages;
    
};
