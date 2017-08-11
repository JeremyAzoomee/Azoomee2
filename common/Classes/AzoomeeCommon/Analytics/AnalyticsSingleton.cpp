#include "AnalyticsSingleton.h"
#include "../Utils/StringMgr.h"
#include "../Input/TextInputChecker.h"
#include "../Data/Parent/ParentDataProvider.h"
#include "../Utils/StringFunctions.h"
#include "../Strings.h"
#include "../Utils/SessionIdManager.h"
#include "../Crashlytics/CrashlyticsConfig.h"

namespace Azoomee
{

static AnalyticsSingleton *_sharedAnalyticsSingleton = NULL;

AnalyticsSingleton* AnalyticsSingleton::getInstance()
{
    if (! _sharedAnalyticsSingleton)
    {
        _sharedAnalyticsSingleton = new AnalyticsSingleton();
        _sharedAnalyticsSingleton->init();
    }
    
    return _sharedAnalyticsSingleton;
}

AnalyticsSingleton::~AnalyticsSingleton(void)
{
}

bool AnalyticsSingleton::init(void)
{
    return true;
}
    
void AnalyticsSingleton::mixPanelSendEventWithStoredProperties(const std::string& eventID)
{
    mixPanelSendEventNative(eventID, storedGeneralProperties);
}

void AnalyticsSingleton::mixPanelSendEventWithStoredProperties(const std::string& eventID, const std::map<std::string, std::string>& map)
{
    std::map<std::string, std::string> mapToBeSent;
    mapToBeSent.insert(map.begin(), map.end());
    mapToBeSent.insert(storedGeneralProperties.begin(), storedGeneralProperties.end());
    
    mixPanelSendEventNative(eventID, mapToBeSent);
}
  
void AnalyticsSingleton::mixPanelRegisterSuperProperties(const std::string& key, const std::string& property)
{
    storedGeneralProperties[key] = property;
}

//-------------- SUPER PROPERTIES---------------------

void AnalyticsSingleton::registerAppVersion()
{
    mixPanelRegisterSuperProperties("appVersion",ConfigStorage::getInstance()->getVersionNumberToDisplay());
}

void AnalyticsSingleton::registerParentID(std::string ParentID)
{
    storedGeneralProperties["parentID"] = ParentID;
    mixPanelRegisterIdentity(ParentID,storedGeneralProperties);
}

void AnalyticsSingleton::registerNoOfChildren(int noOfChildren)
{
    mixPanelRegisterSuperProperties("noOfChildren",cocos2d::StringUtils::format("%s%d",NUMBER_IDENTIFIER, noOfChildren));
}

void AnalyticsSingleton::registerAzoomeeEmail(std::string emailAddress)
{
    std::string azoomeEmail = "NO";
    
    if (hasAzoomeeEmailAddress(emailAddress))
        azoomeEmail = "YES";
    
    mixPanelRegisterSuperProperties("azoomeeEmail",azoomeEmail);
    setCrashlyticsKeyWithString("azoomeeEmail", azoomeEmail);
}

void AnalyticsSingleton::registerAccountStatus(std::string Status)
{
    mixPanelRegisterSuperProperties("accountStatus",Status);
}

void AnalyticsSingleton::registerBillingStatus(std::string Status)
{
    mixPanelRegisterSuperProperties("billingStatus",Status);
}

void AnalyticsSingleton::registerIAPOS(std::string OS_String)
{
    OSManufacturer = OS_String;
    mixPanelRegisterSuperProperties("iAP_OS",OS_String);
}

void AnalyticsSingleton::registerBillingProvider(std::string provider)
{
    mixPanelRegisterSuperProperties("billingProvider",provider);
}

void AnalyticsSingleton::registerChildID(std::string ChildID)
{
    mixPanelRegisterSuperProperties("childID",ChildID);
}

void AnalyticsSingleton::registerChildGenderAndAge(int childNumber)
{
    mixPanelRegisterSuperProperties("sex",ParentDataProvider::getInstance()->getSexForAnAvailableChildren(childNumber));
    
    int childAge = ageFromDOBString(ParentDataProvider::getInstance()->getDOBForAnAvailableChildren(childNumber));
    
    mixPanelRegisterSuperProperties("age",cocos2d::StringUtils::format("%s%d",NUMBER_IDENTIFIER, childAge));
}
    
void AnalyticsSingleton::registerSessionId(std::string sessionId)
{
    mixPanelRegisterSuperProperties("sessionId", sessionId);
    setCrashlyticsKeyWithString("sessionId", sessionId);
}
    
void AnalyticsSingleton::registerCurrentScene(std::string currentScene)
{
    mixPanelRegisterSuperProperties("currentScene", currentScene);
    setCrashlyticsKeyWithString("currentScene", currentScene);
    
    moveToSceneEvent(currentScene);
}
    
void AnalyticsSingleton::setPortraitOrientation()
{
    mixPanelRegisterSuperProperties("orientation", "portrait");
}
    
void AnalyticsSingleton::setLandscapeOrientation()
{
    mixPanelRegisterSuperProperties("orientation", "landscape");
}
    
void AnalyticsSingleton::setNumberOfChatFriends(int noOfChatFriends)
{
    mixPanelRegisterSuperProperties("noOfChatFriends",cocos2d::StringUtils::format("%s%d",NUMBER_IDENTIFIER, noOfChatFriends));
}

void AnalyticsSingleton::setChatFriendIsParent(bool chatFriendIsParent)
{
    std::string friendIsParentString = "NO";
    
    if (chatFriendIsParent)
        friendIsParentString = "YES";
    
    mixPanelRegisterSuperProperties("chatFriendIsParent",friendIsParentString);
}

//-------------logout events-----------------

void AnalyticsSingleton::logoutChildEvent()
{
    mixPanelRegisterSuperProperties("childID","");
    mixPanelRegisterSuperProperties("sex","");
    mixPanelRegisterSuperProperties("age","");
}

void AnalyticsSingleton::logoutParentEvent()
{
    logoutChildEvent();
    mixPanelRegisterSuperProperties("accountStatus","");
    mixPanelRegisterSuperProperties("parentID","");
    mixPanelRegisterSuperProperties("azoomeeEmail","");
    mixPanelRegisterSuperProperties("billingProvider","");
    mixPanelRegisterSuperProperties("billingStatus","");
}

//-------------Startup--------------------

void AnalyticsSingleton::firstLaunchEvent()
{
    if(!(cocos2d::UserDefault::getInstance()->getBoolForKey("firstTimeLaunchEventSent", false) || cocos2d::UserDefault::getInstance()->getBoolForKey("FirstSlideShowSeen", false)))
    {
        cocos2d::UserDefault::getInstance()->setBoolForKey("firstTimeLaunchEventSent", true);
        mixPanelSendEventWithStoredProperties("firstLaunch");
        appsFlyerSendEvent("firstLaunch");
    }
}

// -------------- SIGN IN FUNCTIONS -----------------
void AnalyticsSingleton::signInSuccessEvent()
{
    mixPanelSendEventWithStoredProperties("parentAppLoginSuccess");
}

void AnalyticsSingleton::signInFailEvent(int errorCode)
{
    mixPanelSendEventWithStoredProperties("parentAppLoginFailure");
}

//-------------ONBOARDING--------------------
void AnalyticsSingleton::OnboardingStartEvent()
{
    mixPanelSendEventWithStoredProperties("startCreateAccount");
}

void AnalyticsSingleton::OnboardingAccountCreatedEvent()
{
    std::map<std::string, std::string> mixPanelProperties;
    mixPanelProperties["Method"] = "App";
    
    mixPanelSendEventWithStoredProperties("accountCreated", mixPanelProperties);
    appsFlyerSendEvent("accountCreated", mixPanelProperties);
}

void AnalyticsSingleton::OnboardingAccountCreatedErrorEvent(long errorCode)
{
    std::map<std::string, std::string> mixPanelProperties;
    mixPanelProperties["ErrorCode"] = cocos2d::StringUtils::format("%ld", errorCode);
    
    mixPanelSendEventWithStoredProperties("accountCreatedError", mixPanelProperties);
}

//-------------CHILD PROFILE CREATION-------------
void AnalyticsSingleton::childProfileStartEvent()
{
    mixPanelSendEventWithStoredProperties("childProfileCreateStart");
}

void AnalyticsSingleton::childProdileNameErrorEvent()
{
    mixPanelSendEventWithStoredProperties("childProfileNameError");
}

void AnalyticsSingleton::childProfileDOBErrorEvent()
{
    //There are no errors defined at present for this function.
    //mixPanelSendEvent("childProfileDOBError");
}

void AnalyticsSingleton::childProfileOomeeEvent(int oomeeNumber)
{
    std::map<std::string, std::string> mixPanelProperties;
    mixPanelProperties["SelectedOomee"] = ConfigStorage::getInstance()->getHumanReadableNameForOomee(oomeeNumber);
    
    mixPanelSendEventWithStoredProperties("childProfileOomee", mixPanelProperties);
}

void AnalyticsSingleton::childProfileCreatedSuccessEvent(int oomeeNumber)
{
    std::map<std::string, std::string> mixPanelProperties;
    mixPanelProperties["Method"] = "App";
    mixPanelProperties["SelectedOomee"] = ConfigStorage::getInstance()->getHumanReadableNameForOomee(oomeeNumber);
    
    mixPanelSendEventWithStoredProperties("childProfileCreatedSuccess", mixPanelProperties);
}

void AnalyticsSingleton::childProfileCreatedErrorEvent(long errorCode)
{
    std::map<std::string, std::string> mixPanelProperties;
    mixPanelProperties["ErrorCode"] = cocos2d::StringUtils::format("%ld", errorCode);
    
    mixPanelSendEventWithStoredProperties("childProfileCreatedError", mixPanelProperties);
}
    
void AnalyticsSingleton::childProfileUpdateErrorEvent(long errorCode)
{
    std::map<std::string, std::string> mixPanelProperties;
    mixPanelProperties["ErrorCode"] = cocos2d::StringUtils::format("%ld", errorCode);
    
    mixPanelSendEventWithStoredProperties("childProfileUpdateError", mixPanelProperties);
}

//-------------HUB ACTIONS-------------------
void AnalyticsSingleton::hubTapOomeeEvent(int oomeeNumber, std::string oomeeAction)
{
    std::map<std::string, std::string> mixPanelProperties;
    mixPanelProperties["SelectedOomee"] = ConfigStorage::getInstance()->getHumanReadableNameForOomee(oomeeNumber);
    mixPanelProperties["OomeeAnimation"] = oomeeAction;
    
    mixPanelSendEventWithStoredProperties("tapOomee", mixPanelProperties);
}

void AnalyticsSingleton::navSelectionEvent(std::string hubOrTop, int buttonNumber)
{
    std::map<std::string, std::string> mixPanelProperties;
    mixPanelProperties["Type"] = ConfigStorage::getInstance()->getNameForMenuItem(buttonNumber);
    mixPanelProperties["Method"] = hubOrTop;
    
    mixPanelSendEventWithStoredProperties("contentNavSelection", mixPanelProperties);
}

//---------------- CONTENT ITEM ACTIONS------------------------
    
void AnalyticsSingleton::contentItemSelectedEvent(std::string Title,std::string Description, std::string Type, std::string contentID, int rowNumber, int elementNumber, std::string elementShape)
{
    SessionIdManager::getInstance()->resetBackgroundTimeInContent();

    std::map<std::string, std::string> mixPanelProperties;
    mixPanelProperties["Title"] = Title;
    mixPanelProperties["Description"] = Description;
    mixPanelProperties["Type"] = Type;
    mixPanelProperties["ContentID"] = contentID;
    mixPanelProperties["rowNumber"] = cocos2d::StringUtils::format("%d", rowNumber);
    mixPanelProperties["elementNumber"] = cocos2d::StringUtils::format("%d", elementNumber);
    mixPanelProperties["elementShape"] = elementShape;
    
    storedContentItemProperties = mixPanelProperties;
    
    mixPanelSendEventWithStoredProperties("contentItemSelectedEvent", mixPanelProperties);
    appsFlyerSendEvent("contentItemSelectedEvent", mixPanelProperties);
}
    
void AnalyticsSingleton::updateContentItemDetails(std::map<std::string, std::string> contentItemDetails)
{
    if (contentItemDetails.find("id") == contentItemDetails.end()) return;
    
    std::map<std::string, std::string> mixPanelProperties;
    mixPanelProperties["Title"] = contentItemDetails["title"];
    mixPanelProperties["Description"] = contentItemDetails["description"];
    mixPanelProperties["Type"] = contentItemDetails["type"];
    mixPanelProperties["ContentID"] = contentItemDetails["id"];
    mixPanelProperties["rowNumber"] = storedContentItemProperties["rowNumber"];
    mixPanelProperties["elementNumber"] = contentItemDetails["elementNumber"];
    mixPanelProperties["elementShape"] = contentItemDetails["elementShape"];
    
    storedContentItemProperties = mixPanelProperties;
}
    
void AnalyticsSingleton::contentItemProcessingStartedEvent()
{
    mixPanelSendEventWithStoredProperties("contentItemProcessingStarted", storedContentItemProperties);
}
    
void AnalyticsSingleton::contentItemProcessingErrorEvent()
{
    mixPanelSendEventWithStoredProperties("contentItemProcessingError", storedContentItemProperties);
}
    
void AnalyticsSingleton::contentItemIncompatibleEvent()
{
    mixPanelSendEventWithStoredProperties("contentItemIncompatible", storedContentItemProperties);
}
    
void AnalyticsSingleton::contentItemWebviewStartedEvent()
{
    time(&timeOpenedContent);
    mixPanelSendEventWithStoredProperties("contentItemWebviewStarted", storedContentItemProperties);
}

void AnalyticsSingleton::contentItemClosedEvent()
{
    time_t now;
    time(&now);
    double secondsOpened = difftime(now,timeOpenedContent);
    
    secondsOpened -= SessionIdManager::getInstance()->getBackgroundTimeInContent();
    
    storedContentItemProperties["SecondsInContent"] = cocos2d::StringUtils::format("%s%.f",NUMBER_IDENTIFIER, secondsOpened);
    
    mixPanelSendEventWithStoredProperties("contentItemClosed", storedContentItemProperties);
    
}

//------------- PREVIEW ACTIONS ---------------
void AnalyticsSingleton::previewContentClickedEvent(std::string Title, std::string Description, std::string Type)
{
    std::map<std::string, std::string> mixPanelProperties;
    mixPanelProperties["Title"] = Title;
    mixPanelProperties["Description"] = Description;
    mixPanelProperties["Type"] = Type;
    
    mixPanelSendEventWithStoredProperties("previewContentItemClicked", mixPanelProperties);
}

void AnalyticsSingleton::previewPopupCancelledEvent()
{
    mixPanelSendEventWithStoredProperties("previewEmailPopUpDismissed");
}

//---------------MEDIA ACTIONS -----------------
void AnalyticsSingleton::mediaPlayerQualityEvent(std::string quality)
{
    std::map<std::string, std::string> mixPanelProperties = storedContentItemProperties;
    
    mixPanelProperties["Quality"] = quality;
    
    mixPanelSendEventWithStoredProperties("mediaQuality", mixPanelProperties);
}

void AnalyticsSingleton::mediaPlayerProgressEvent(int percentComplete)
{
    std::map<std::string, std::string> mixPanelProperties = storedContentItemProperties;
    mixPanelProperties["Progress"] = cocos2d::StringUtils::format("%d", percentComplete);
    
    mixPanelSendEventWithStoredProperties("mediaProgress", mixPanelProperties);
}

void AnalyticsSingleton::mediaPlayerPausedEvent()
{
    mixPanelSendEventWithStoredProperties("mediaPause", storedContentItemProperties);
}

void AnalyticsSingleton::mediaPlayerEndEvent(int SecondsMediaPlayed)
{
    std::map<std::string, std::string> mixPanelProperties = storedContentItemProperties;
    
    mixPanelProperties["SecondsMediaPlayed"] = cocos2d::StringUtils::format("%s%d",NUMBER_IDENTIFIER, SecondsMediaPlayed);

    mixPanelSendEventWithStoredProperties("mediaEnd", mixPanelProperties);
}

void AnalyticsSingleton::mediaPlayerFirstFrameEvent(std::string loadTimeMS)
{
    std::map<std::string, std::string> mixPanelProperties = storedContentItemProperties;
    mixPanelProperties["LoadTime"] = cocos2d::StringUtils::format("%s%s",NUMBER_IDENTIFIER, loadTimeMS.c_str());
    
    mixPanelSendEventWithStoredProperties("mediaFirstFrame", mixPanelProperties);
}
    
void AnalyticsSingleton::mediaPlayerNewPlaylistItemSetEvent(int itemNumber)
{
    std::map<std::string, std::string> mixPanelProperties = storedContentItemProperties;
    mixPanelProperties["playlistElementNumber"] = cocos2d::StringUtils::format("%d", itemNumber);

    mixPanelSendEventWithStoredProperties("mediaNewPlaylistItem", mixPanelProperties);
}
    
void AnalyticsSingleton::mediaPlayerVideoPlayEvent()
{
    mixPanelSendEventWithStoredProperties("mediaVideoPlay", storedContentItemProperties);
}
    
void AnalyticsSingleton::mediaPlayerVideoCompletedEvent()
{
    mixPanelSendEventWithStoredProperties("mediaVideoCompleted", storedContentItemProperties);
}
    
void AnalyticsSingleton::mediaPlayerPlaylistCompletedEvent()
{
    mixPanelSendEventWithStoredProperties("mediaPlaylistCompleted", storedContentItemProperties);
}

//---------------OTHER ACTION------------------
void AnalyticsSingleton::genericButtonPressEvent(std::string buttonName)
{
    std::map<std::string, std::string> mixPanelProperties;
    mixPanelProperties["ButtonName"] = buttonName;
    
    mixPanelSendEventWithStoredProperties("tapButton", mixPanelProperties);
}

void AnalyticsSingleton::messageBoxShowEvent(std::string messageTitle, long errorCode)
{
    std::map<std::string, std::string> mixPanelProperties;
    mixPanelProperties["MessageTitle"] = messageTitle;
    mixPanelProperties["ErrorCode"] = cocos2d::StringUtils::format("%ld", errorCode);
    
    mixPanelSendEventWithStoredProperties("messageBoxDisplayed", mixPanelProperties);
}

void AnalyticsSingleton::localisedStringErrorEvent(std::string stringRequested, std::string languageUsed)
{
    std::map<std::string, std::string> mixPanelProperties;
    mixPanelProperties["StringRequestFor"] = stringRequested;
    mixPanelProperties["WithLanguage"] = languageUsed;
    
    mixPanelSendEventWithStoredProperties("localisedStringError", mixPanelProperties);
}

void AnalyticsSingleton::introVideoTimedOutError(std::string errorMessage)
{
    std::map<std::string, std::string> mixPanelProperties;
    mixPanelProperties["ErrorMessage"] = errorMessage;
    
    mixPanelSendEventWithStoredProperties("introVideoTimedOutError", mixPanelProperties);
}
    
void AnalyticsSingleton::enteredBackgroundEvent()
{
    mixPanelSendEventWithStoredProperties("enteredBackgroundEvent");
}
void AnalyticsSingleton::enteredForegroundEvent()
{
    mixPanelSendEventWithStoredProperties("enteredForegroundEvent");
}
    
void AnalyticsSingleton::sessionIdHasChanged(std::string oldSessionId)
{
    std::map<std::string, std::string> mixPanelProperties;
    mixPanelProperties["oldSessionId"] = oldSessionId;
    
    mixPanelSendEventWithStoredProperties("sessionIdHasChanged", mixPanelProperties);
}
    
void AnalyticsSingleton::httpRequestFailed(std::string requestTag, long responseCode, std::string qid)
{
    std::map<std::string, std::string> mixPanelProperties;
    mixPanelProperties["requestTag"] = requestTag;
    mixPanelProperties["responseCode"] = cocos2d::StringUtils::format("%ld", responseCode);;
    mixPanelProperties["qid"] = qid;
    
    mixPanelSendEventWithStoredProperties("httpRequestFailed", mixPanelProperties);
}
    
void AnalyticsSingleton::moveToSceneEvent(std::string newScene)
{
    std::map<std::string, std::string> mixPanelProperties;
    mixPanelProperties["newScene"] = newScene;
    
    mixPanelSendEventWithStoredProperties("moveToSceneEvent", mixPanelProperties);
}
    
void AnalyticsSingleton::settingsOnlineSafetySlideChangeEvent(int NewSlideNumber)
{
    std::map<std::string, std::string> mixPanelProperties;
    mixPanelProperties["NewSlideNumber"] = cocos2d::StringUtils::format("%d", NewSlideNumber);
    
    mixPanelSendEventWithStoredProperties("settingsOnlineSafetySlideChange", mixPanelProperties);
}

//---------------IAP ACTIONS------------------
  
void AnalyticsSingleton::displayIAPUpsaleEvent(std::string fromLocation)
{
    std::map<std::string, std::string> mixPanelProperties;
    mixPanelProperties["DisplayedFrom"] = fromLocation;
    
    mixPanelSendEventWithStoredProperties("displayIAPUpsale", mixPanelProperties);
}

void AnalyticsSingleton::iapSubscriptionSuccessEvent()
{
    mixPanelSendEventWithStoredProperties("iapSubscriptionSuccess");
    
    std::map<std::string, std::string> mixPanelProperties;
    mixPanelProperties["iAP_OS"] = OSManufacturer;
    
    appsFlyerSendEvent("iapSubscriptionSuccess", mixPanelProperties);
}

void AnalyticsSingleton::iapSubscriptionErrorEvent(std::string errorDescription)
{
    std::map<std::string, std::string> mixPanelProperties;
    mixPanelProperties["ErrorType"] = errorDescription;
    
    mixPanelSendEventWithStoredProperties("iapSubscriptionError", mixPanelProperties);
}

void AnalyticsSingleton::iapSubscriptionFailedEvent(const std::string& failureDetails)
{
    std::map<std::string, std::string> mixPanelProperties = { {"FailureDetails", failureDetails} };
    mixPanelSendEventWithStoredProperties("iapSubscriptionFailed", mixPanelProperties);
}

void AnalyticsSingleton::iapUserDataFailedEvent()
{
    mixPanelSendEventWithStoredProperties("iapUserDataFailed");
}

void AnalyticsSingleton::iapSubscriptionDoublePurchaseEvent()
{
    mixPanelSendEventWithStoredProperties("iapSubscriptionDoublePurchase");
}

void AnalyticsSingleton::iapBackEndRequestFailedEvent(long errorCode)
{
    std::map<std::string, std::string> mixPanelProperties;
    mixPanelProperties["ErrorCode"] = cocos2d::StringUtils::format("%ld", errorCode);
    
    mixPanelSendEventWithStoredProperties("iapBackendRequestFailedError", mixPanelProperties);
}

void AnalyticsSingleton::iapAppleAutoRenewSubscriptionEvent()
{
    mixPanelSendEventWithStoredProperties("iapAppleAutoRenewSubscription");
}
    
//---------------DEEPLINKING ACTIONS------------------
void AnalyticsSingleton::deepLinkingDetailsSetEvent()
{
    mixPanelSendEventWithStoredProperties("deepLinkingDetailsSet");
}
    
void AnalyticsSingleton::deepLinkingMoveToEvent(std::string moveTo)
{
    std::map<std::string, std::string> mixPanelProperties;
    mixPanelProperties["MoveTo"] = moveTo;
    
    mixPanelSendEventWithStoredProperties("deepLinkingMoveToEvent", mixPanelProperties);
}
    
void AnalyticsSingleton::deepLinkingContentEvent()
{
    mixPanelSendEventWithStoredProperties("deepLinkingContentEvent");
}
    
    //-----------------CHAT INVITATION EVENTS------------------
    void AnalyticsSingleton::settingsConfirmationTabNotificationShown()
    {
        mixPanelSendEventWithStoredProperties("settingsConfirmationTabNotificationShown");
    }
    
    void AnalyticsSingleton::settingsConfirmationTabNotificationError(long errorCode)
    {
        std::map<std::string, std::string> mixPanelProperties;
        mixPanelProperties["ErrorCode"] = cocos2d::StringUtils::format("%ld", errorCode);
        
        mixPanelSendEventWithStoredProperties("settingsConfirmationTabNotificationError", mixPanelProperties);
    }
    
    void AnalyticsSingleton::settingsKidInviteRequestSuccess()
    {
        mixPanelSendEventWithStoredProperties("settingsKidInviteRequestSuccess");
    }
    
    void AnalyticsSingleton::settingsKidInviteRequestError(long errorCode)
    {
        std::map<std::string, std::string> mixPanelProperties;
        mixPanelProperties["ErrorCode"] = cocos2d::StringUtils::format("%ld", errorCode);
        
        mixPanelSendEventWithStoredProperties("settingsKidInviteRequestError", mixPanelProperties);
    }
    
    void AnalyticsSingleton::settingsPendingFriendRequestsRefreshError(long errorCode)
    {
        std::map<std::string, std::string> mixPanelProperties;
        mixPanelProperties["ErrorCode"] = cocos2d::StringUtils::format("%ld", errorCode);
        
        mixPanelSendEventWithStoredProperties("settingsPendingFriendRequestsRefreshError", mixPanelProperties);
    }
    
    void AnalyticsSingleton::settingsConfirmationRejectedSuccess()
    {
        mixPanelSendEventWithStoredProperties("settingsConfirmationRejectedSuccess");
    }
    
    void AnalyticsSingleton::settingsConfirmationApprovedSuccess()
    {
        mixPanelSendEventWithStoredProperties("settingsConfirmationApprovedSuccess");
    }
    
    void AnalyticsSingleton::settingsConfirmationError(long errorCode)
    {
        std::map<std::string, std::string> mixPanelProperties;
        mixPanelProperties["ErrorCode"] = cocos2d::StringUtils::format("%ld", errorCode);
        
        mixPanelSendEventWithStoredProperties("settingsConfirmationError", mixPanelProperties);
    }
    
    void AnalyticsSingleton::chatKeyboardEvent(bool isOnScreen)
    {
        std::string isOnScreenString = "NO";
        
        if (isOnScreen)
            isOnScreenString = "YES";

        std::map<std::string, std::string> mixPanelProperties;
        mixPanelProperties["keyboardOnScreen"] = isOnScreenString;
        
        mixPanelSendEventWithStoredProperties("chatKeyboardOnScreenEvent", mixPanelProperties);
    }
    
    void AnalyticsSingleton::chatSelectedSticker(std::string stickerURL)
    {
        std::map<std::string, std::string> mixPanelProperties;
        mixPanelProperties["stickerURL"] = stickerURL;
        
        mixPanelSendEventWithStoredProperties("chatSelectedSticker", mixPanelProperties);
    }
    
    void AnalyticsSingleton::chatSelectedStickerTab(std::string categoryID)
    {
        std::map<std::string, std::string> mixPanelProperties;
        mixPanelProperties["stickerCategoryID"] = categoryID;
        
        mixPanelSendEventWithStoredProperties("chatSelectedStickerTab", mixPanelProperties);
    }
    
    void AnalyticsSingleton::chatIncomingMessageEvent(std::string messageType)
    {
        std::map<std::string, std::string> mixPanelProperties;
        mixPanelProperties["messageType"] = messageType;
        
        mixPanelSendEventWithStoredProperties("chatIncomingMessageEvent", mixPanelProperties);
    }
    
    void AnalyticsSingleton::chatOutgoingMessageEvent(std::string messageType)
    {
        std::map<std::string, std::string> mixPanelProperties;
        mixPanelProperties["messageType"] = messageType;
        
        mixPanelSendEventWithStoredProperties("chatOutgoingMessageEvent", mixPanelProperties);
    }
    
    void AnalyticsSingleton::unreadMessagesNotificationReceived()
    {
        mixPanelSendEventWithStoredProperties("unreadMessagesNotificationReceived");
    }
    
    void AnalyticsSingleton::chatReportedEvent()
    {
        mixPanelSendEventWithStoredProperties("chatReportedEvent");
    }
    
    void AnalyticsSingleton::chatResetReportedEvent()
    {
        mixPanelSendEventWithStoredProperties("chatResetReportedEvent");
    }
    
    
}
