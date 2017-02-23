#include "cocos2d.h"
#include <time.h>

class AnalyticsSingleton : public cocos2d::Ref
{
    
private:
    void mixPanel_createOSSpecficCall(std::string eventID);
    void mixPanel_createOSSpecficCall(std::string eventID, std::map<std::string, std::string> map);
    
    void mixPanel_OSSpecificSuperPropertiesCall(std::string Key, std::string Property);
    
    void appsflyer_createOSSpecficCall(std::string eventID);
    void appsflyer_createOSSpecficCall(std::string eventID, std::map<std::string, std::string> map);
    
    std::string storedTitle;
    std::string storedDescription;
    std::string storedType;
    std::string storedContentID;
    
    time_t timeOpenedContent;
    
public:
    static AnalyticsSingleton* getInstance(void);
    virtual ~AnalyticsSingleton();
    bool init(void);

    //---------------SUPER PROPERTIES---------
    
    void registerAppVersion();
    void registerParentID(std::string ParentID);
    void registerNoOfChildren(int noOfChildren);
    void registerAzoomeeEmail(std::string emailAddress);
    void registerAccountStatus(std::string Status);   //NEED TO BE ADDED TO CODE

    void registerChildID(std::string ChildID);
    void registerChildGender(std::string ChildGender); //NEED TO BE ADDED TO CODE
    void registerChildDOB(std::string ChildDOB); //NEED TO BE ADDED TO CODE

    void logoutChildEvent();
    void logoutParentEvent();

    //-------------Startup--------------------

    void firstLaunchEvent();

    void signInSuccessEvent();
    void signInFailEvent(int errorCode); // NEED TO UNDERSTAND HOW TO PASS THE ERROR CODE

    //-------------ONBOARDING--------------------
    void OnboardingStartEvent();
    void OnboardingEmailSubmittedEvent();
    void OnboardingPasswordSubmittedEvent();
    void OnboardingPinSubmittedEvent();
    void OnboardingAccountCreatedEvent();
    void OnboardingAccountCreatedErrorEvent(long errorCode);

    //-------------CHILD PROFILE CREATION-------------
    void childProfileStartEvent();
    void childProfileNameEvent();
    void childProdileNameErrorEvent();
    void childProfileDOBEvent();
    void childProfileDOBErrorEvent(); // NO DOB CHECKING IS COMPLETED
    void childProfileOomeeEvent(int oomeeNumber);
    void childProfileCreatedSuccessEvent(int oomeeNumber); // HOW TO GET THE OOMEE NUMBER?
    void childProfileCreatedErrorEvent(long errorCode);

    //-------------HUB ACTIONS-------------------
    void hubTapOomeeEvent(int oomeeNumber, std::string oomeeAction);
    void navSelectionEvent(std::string hubOrTop, int buttonNumber);
    void openContentEvent(std::string Title,std::string Description, std::string Type, std::string contentID);
    void closeContentEvent();

    //------------- PREVIEW ACTIONS ---------------
    void previewContentClickedEvent(std::string Title,std::string Description, std::string Type);
    void previewPopupCancelledEvent();

    //---------------MEDIA ACTIONS -----------------
    void mediaQualityEvent(std::string quality);
    void mediaProgressEvent(int percentComplete);
    void mediaPausedEvent();
    void mediaEndEvent(int SecondsMediaPlayed);

    //---------------OTHER ACTION------------------
    void genericButtonPressEvent(std::string buttonName);
    void messageBoxShowEvent(std::string messageTitle);

};
