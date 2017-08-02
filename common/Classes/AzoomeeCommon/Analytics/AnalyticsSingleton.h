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
    void mixPanelSendEventNative(const std::string& eventID, const std::map<std::string, std::string>& map);
    
    void mixPanelSendEventWithStoredProperties(const std::string& eventID);
    void mixPanelSendEventWithStoredProperties(const std::string& eventID, const std::map<std::string, std::string>& map);
    
    void mixPanelRegisterSuperProperties(const std::string& key, const std::string& property);
    void mixPanelRegisterIdentity(const std::string& parentID, const std::map<std::string, std::string>& map);
    
    void appsFlyerSendEvent(const std::string& eventID);
    void appsFlyerSendEvent(const std::string& eventID, const std::map<std::string, std::string>& map);
    
    std::map<std::string, std::string> storedContentItemProperties;
    std::map<std::string, std::string> storedGeneralProperties;
    
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
    void setNumberOfChatFriends(int noOfChildren);
    void setChatFriendIsParent(bool chatFriendIsParent);

    void registerChildID(std::string noOfChatFriends);
    void registerChildGenderAndAge(int childNumber);

    void logoutChildEvent();
    void logoutParentEvent();

    //-------------Startup--------------------

    void firstLaunchEvent();

    void signInSuccessEvent();
    void signInFailEvent(int errorCode); // NEED TO UNDERSTAND HOW TO PASS THE ERROR CODE

    //-------------ONBOARDING--------------------
    void OnboardingStartEvent();
    void OnboardingPasswordSubmittedEvent();
    void OnboardingPinSubmittedEvent();
    void OnboardingAccountCreatedEvent();
    void OnboardingAccountCreatedErrorEvent(long errorCode);

    //-------------CHILD PROFILE CREATION-------------
    void childProfileStartEvent();
    void childProdileNameErrorEvent();
    void childProfileDOBEvent();
    void childProfileDOBErrorEvent(); // NO DOB CHECKING IS COMPLETED
    void childProfileOomeeEvent(int oomeeNumber);
    void childProfileCreatedSuccessEvent(int oomeeNumber);
    void childProfileCreatedErrorEvent(long errorCode);
    void childProfileUpdateErrorEvent(long errorCode);

    //-------------HUB ACTIONS-------------------
    void hubTapOomeeEvent(int oomeeNumber, std::string oomeeAction);
    void navSelectionEvent(std::string hubOrTop, int buttonNumber);
    
    //-------------CONTENTITEM EVENTS----------
    void contentItemSelectedEvent(std::string Title,std::string Description, std::string Type, std::string contentID, int rowNumber, int elementNumber, std::string elementShape);
    void contentItemProcessingStartedEvent();
    void contentItemProcessingErrorEvent();
    void contentItemIncompatibleEvent();
    void contentItemWebviewStartedEvent();
    void contentItemClosedEvent();
    void updateContentItemDetails(std::map<std::string, std::string> contentItemDetails);

    //------------- PREVIEW ACTIONS ---------------
    void previewContentClickedEvent(std::string Title,std::string Description, std::string Type);
    void previewPopupCancelledEvent();

    //---------------MEDIA ACTIONS -----------------
    void mediaPlayerQualityEvent(std::string quality);
    void mediaPlayerProgressEvent(int percentComplete);
    void mediaPlayerPausedEvent();
    void mediaPlayerEndEvent(int SecondsMediaPlayed);
    void mediaPlayerFirstFrameEvent(std::string loadTimeMS);
    void mediaPlayerNewPlaylistItemSetEvent(int itemNumber);
    void mediaPlayerPlaylistCompletedEvent();
    void mediaPlayerVideoPlayEvent();
    void mediaPlayerVideoCompletedEvent();

    //---------------OTHER ACTION------------------
    void genericButtonPressEvent(std::string buttonName);
    void messageBoxShowEvent(std::string messageTitle, long errorCode);
    void localisedStringErrorEvent(std::string stringRequested, std::string languageUsed);
    void introVideoTimedOutError(std::string errorMessage);
    void enteredBackgroundEvent();
    void enteredForegroundEvent();
    void sessionIdHasChanged(std::string oldSessionId);
    void httpRequestFailed(std::string requestTag, long responseCode, std::string qid);
    void moveToSceneEvent(std::string newScene);
    void settingsOnlineSafetySlideChangeEvent(int NewSlideNumber);
    
    //---------------IAP ACTIONS------------------
    void displayIAPUpsaleEvent(std::string fromLocation);
    void iapSubscriptionSuccessEvent();
    void iapSubscriptionErrorEvent(std::string errorDescription);
    void iapSubscriptionFailedEvent(const std::string& failureDetails);
    void iapUserDataFailedEvent();
    void iapSubscriptionDoublePurchaseEvent();
    void iapBackEndRequestFailedEvent(long errorCode);
    void iapAppleAutoRenewSubscriptionEvent();
    
    //---------------DEEPLINKING ACTIONS------------------
    void deepLinkingDetailsSetEvent();
    void deepLinkingMoveToEvent(std::string moveTo);
    void deepLinkingContentEvent();
    
    //-----------------CHAT INVITATION EVENTS------------------
    void settingsConfirmationTabNotificationShown();
    void settingsConfirmationTabNotificationError(long errorCode);
    void settingsKidInviteRequestSuccess();
    void settingsKidInviteRequestError(long errorCode);
    void settingsPendingFriendRequestsRefreshError(long errorCode);
    void settingsConfirmationRejectedSuccess();
    void settingsConfirmationApprovedSuccess();
    void settingsConfirmationError(long errorCode);
    void chatKeyboardEvent(bool isOnScreen);
    void chatSelectedSticker(std::string stickerURL);
    void chatSelectedStickerTab(std::string categoryID);
    void chatIncomingMessageEvent(std::string messageType);
    void chatOutgoingMessageEvent(std::string messageType);
    void unreadMessagesNotificationReceived();
    void chatReportedEvent();
    void chatResetReportedEvent();

};
  
}

#endif
