#ifndef AzoomeeCommon_AnalyticsSingleton_h
#define AzoomeeCommon_AnalyticsSingleton_h

#include <cocos/cocos2d.h>
#include "../Data/ConfigStorage.h"
#include <time.h>

#define NUMBER_IDENTIFIER "NUMBER|"


namespace Azoomee
{
  
class AnalyticsSingleton : public cocos2d::Ref
{
    
private:
    void mixPanelSendEvent(const std::string& eventID);
    void mixPanelSendEvent(const std::string& eventID, const std::map<std::string, std::string>& map);
    void mixPanelRegisterSuperProperties(const std::string& key, const std::string& property);
    void mixPanelRegisterSuperProperties(const std::map<std::string, std::string>& map);
    void mixPanelRegisterIdentity(const std::string& parentID, const std::map<std::string, std::string>& map);
    
    void appsFlyerSendEvent(const std::string& eventID);
    void appsFlyerSendEvent(const std::string& eventID, const std::map<std::string, std::string>& map);
    
    std::string storedTitle;
    std::string storedDescription;
    std::string storedType;
    std::string storedContentID;
    int storedRowNumber;
    int storedElementNumber;
    std::string storedElementShape;
    
    std::string OSManufacturer;
    
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
    void registerAccountStatus(std::string Status);
    void registerBillingStatus(std::string Status);
    void registerIAPOS(std::string OS_String);
    void registerBillingProvider(std::string provider);
    void registerSessionId(std::string sessionId);
    void registerCurrentScene(std::string currentScene);
    void setLandscapeOrientation();
    void setPortraitOrientation();


    void registerChildID(std::string ChildID);
    void registerChildGenderAndAge(int childNumber);

    void logoutChildEvent();
    void logoutParentEvent();

    //-------------Startup--------------------

    void firstLaunchEvent();

    void signInSuccessEvent();
    void signInFailEvent(int errorCode); // NEED TO UNDERSTAND HOW TO PASS THE ERROR CODE

    //-------------ONBOARDING--------------------
    void OnboardingStartEvent();
    void OnboardingEmailSubmittedEvent(std::string emailAddress);
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
    void childProfileCreatedSuccessEvent(int oomeeNumber);
    void childProfileCreatedErrorEvent(long errorCode);

    //-------------HUB ACTIONS-------------------
    void hubTapOomeeEvent(int oomeeNumber, std::string oomeeAction);
    void navSelectionEvent(std::string hubOrTop, int buttonNumber);
    void openContentEvent(std::string Title,std::string Description, std::string Type, std::string contentID, int rowNumber, int elementNumber, std::string elementShape);
    void closeContentEvent();

    //------------- PREVIEW ACTIONS ---------------
    void previewContentClickedEvent(std::string Title,std::string Description, std::string Type);
    void previewPopupCancelledEvent();

    //---------------MEDIA ACTIONS -----------------
    void mediaQualityEvent(std::string quality);
    void mediaProgressEvent(int percentComplete);
    void mediaPausedEvent();
    void mediaEndEvent(int SecondsMediaPlayed);
    void mediaPlayerFirstFrameEvent(std::string loadTimeMS);

    //---------------OTHER ACTION------------------
    void genericButtonPressEvent(std::string buttonName);
    void messageBoxShowEvent(std::string messageTitle);
    void localisedStringErrorEvent(std::string stringRequested, std::string languageUsed);
    void introVideoTimedOutError(std::string errorMessage);
    void enteredBackgroundEvent();
    void enteredForegroundEvent();
    void sessionIdHasChanged(std::string oldSessionId);
    void httpRequestFailed(std::string requestTag, long responseCode, std::string qid);
    
    //---------------IAP ACTIONS------------------
    void displayIAPUpsaleEvent(std::string fromLocation);
    void iapSubscriptionSuccessEvent();
    void iapSubscriptionErrorEvent(std::string errorDescription);
    void iapSubscriptionFailedEvent();
    void iapUserDataFailedEvent();
    void iapSubscriptionDoublePurchaseEvent();
    void iapBackEndRequestFailedEvent(long errorCode);
    void iapAppleAutoRenewSubscriptionEvent();

};
  
}

#endif
