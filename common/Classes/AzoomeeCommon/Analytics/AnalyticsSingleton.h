#ifndef AzoomeeCommon_AnalyticsSingleton_h
#define AzoomeeCommon_AnalyticsSingleton_h

#include <cocos/cocos2d.h>
#include "../Data/ConfigStorage.h"
#include <time.h>
#include "../Data/HQDataObject/HQContentItemObject.h"
#include "AnalyticsProperties.h"
#include "../Data/Parent/BillingData.h"
#include "../Data/Child/Child.h"

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
	
	void mixPanelUpdatePeopleProfileData(const std::map<std::string, std::string>& profileData);
	
	void mixPanelRegisterAlias(const std::string& newId);
	
    void appsFlyerSendEvent(const std::string& eventID);
    void appsFlyerSendEvent(const std::string& eventID, const std::map<std::string, std::string>& map);
    
    AnalyticsProperties* _analyticsProperties = nullptr;
    
public:
    static AnalyticsSingleton* getInstance(void);
    virtual ~AnalyticsSingleton();
    bool init(void);

    //---------------SUPER PROPERTIES---------
    
    void registerAppVersion();
    void registerAnonymousIp(const std::string& anonymousIp);
    void registerIdentifier(const std::string& parentId);
    void registerNoOfChildren(int noOfChildren);
    void registerAzoomeeEmail(std::string emailAddress);
    void registerAccountStatus(std::string Status);
    void registerBillingStatus(std::string Status);
    void registerIAPOS(std::string OS_String);
    void registerBillingProvider(std::string provider);
    void registerSessionId(std::string sessionId);
    void registerCurrentScene(const std::string& currentScene);
    void setLandscapeOrientation();
    void setPortraitOrientation();
    void setNumberOfChatFriends(int noOfChildren);
    void setChatFriendIsParent(bool chatFriendIsParent);
    void setIsUserAnonymous(bool isUserAnonymous);
	void registerAlias(const std::string& newId);
	void registerBillingData(const BillingDataRef& billingData);
	void registerLanguageCode(const std::string& languageCode);
	
    void registerChildGenderAndAge(const ChildRef& child);

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
    void childProfileCreatedSuccessEvent();
    void childProfileCreatedErrorEvent(long errorCode);
    void childProfileUpdateErrorEvent(long errorCode);

    void childProfileCreatedEvent(int age);
    
    //-------------HUB ACTIONS-------------------
    void navSelectionEvent(std::string hubOrTop, const std::string& buttonName);
    
    //-------------CONTENTITEM EVENTS----------
    void contentItemSelectedEvent(const std::string& Type);
    void contentItemSelectedEvent(const std::string& Type, const std::string& elementShape);
    void contentItemSelectedEvent(const HQContentItemObjectRef &contentItem, int rowNumber, int elementNumber, const std::string& elementShape);
    void contentItemSelectedOutsideCarouselEvent(const HQContentItemObjectRef &contentItem);
    void contentItemProcessingStartedEvent();
    void contentItemProcessingErrorEvent();
    void contentItemIncompatibleEvent();
    void contentItemWebviewStartedEvent();
    void contentItemClosedEvent();
    void updateContentItemDetails(const HQContentItemObjectRef &contentItem);
    void lockedContentItemInRecentlyPlayedEvent(const HQContentItemObjectRef& contentItem);

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
    void moveToSceneEvent(const std::string& previousScene);
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
    void chatOpenSharedContentEvent(const std::string& contentId);
    
    //---------------CTA EVENTS----------------------------------
    void ctaButtonPressed(const std::string &buttonId, const std::string &title = "");
    void ctaWindowAppeared(const std::string &groupId, const std::string &nodeId);
    void registerCTASource(const std::string& buttonId, const std::string& contentId, const std::string& mediaType, const std::string& recommendedContentId = "");
    
    //--------------ME HQ Events--------------------------------
    void favouriteContentItem(const std::string& contentItemId);
    void unFavouriteContentItem(const std::string& contentItemId);
    void shareContentItemButtonPressed(const std::string& contentItemId);
    
    //-------------OomeeMaker Events----------------------------
    void makeAvatarSuccess(const std::string& origin);
    void shareOomee();
    void editOomee();
    void deleteOomee();
    void newOomee();
    void saveOomee(const std::string& oomeeDataString);
    void reportNumberOfOomees(int numOomees);
    void accessorySelectedEvent(const std::string& method);
    
    //-------------Art app events-------------------------------
    void stickerSelectedEvent(const std::string& stickerFilename);
	
	//-------------I18n events----------------------------------
	void languageChangedEvent(const std::string& languageCode);
	
	//-------------Vodacom events-------------------------------
	void vodacomOnboardingFlowStartedEvent();
	void vodacomOnboardingFlowExitEvent();
	void vodacomOnboardingFlowMoveToScreen(const std::string& newScreen);
	void vodacomOnboardingVoucherAdded(const std::string& voucherCode);
	
};
  
}

#endif
