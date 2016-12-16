#include "cocos2d.h"
#include "network/HttpClient.h"
#include "external/json/document.h"
#include "ModalMessages.h"

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
    
    void registerParent(std::string emailAddress, std::string password, std::string pinNumber);
    void onRegisterParentAnswerReceived(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    
    void getAvailableChildren();
    void onGetChildrenAnswerReceived(std::string responseString);
    
    void childLogin(int childNumber);
    void onChildLoginAnswerReceived(std::string responseString);
    
    void registerChild(std::string childProfileName, std::string childGender, std::string childDOB, int oomeeNumber);
    void onRegisterChildLoginAnswerReceived(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    
    void getContent();
    void onGetContentAnswerReceived(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    
    void getGordon();
    void onGetGordonAnswerReceived(std::string responseString);
    
    void displayLoadingScreen();
    void hideLoadingScreen();
    
    void reloadLoginSceneWithError(long errorCode, std::string errorMessage);
    void reloadChildSelectorSceneWithError(long errorCode, std::string errorMessage);
    
    //Saved here from registerParent, if onRegisterParentAnswerReceived success, then login.
    std::string registerParentUsername;
    std::string registerParentPassword;
    
    ModalMessages* modalMessages;
    
};
