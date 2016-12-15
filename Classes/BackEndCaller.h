#include "cocos2d.h"
#include "network/HttpClient.h"
#include "external/json/document.h"

class BackEndCaller : public cocos2d::Ref
{
    
public:
    /** Returns the shared instance of the Game Manager */
    static BackEndCaller* getInstance(void);
    
    void reloadLoginSceneWithError(long errorCode, std::string errorMessage);
    
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
    
    void registerParent(std::string emailAddress, std::string password, std::string pinNumber);
    void onRegisterParentAnswerReceived(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    
    //Saved here from registerParent, if onRegisterParentAnswerReceived success, then login.
    std::string registerParentUsername;
    std::string registerParentPassword;
    
    void onGetChildrenAnswerReceived(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    void getAvailableChildren();
    
    void childLogin(int childNumber);
    void onChildLoginAnswerReceived(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    
    void registerChild(std::string childProfileName, std::string childGender, std::string childDOB, int oomeeNumber);
    void onRegisterChildLoginAnswerReceived(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    
    void getContent();
    void onGetContentAnswerReceived(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    
    void getGordon();
    void onGetGordonAnswerReceived(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
    
    void displayError(std::string errorMessage);
    
};
