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
}
    
void AnalyticsSingleton::setPortraitOrientation()
{
    mixPanelRegisterSuperProperties("orientation", "portrait");
}
    
void AnalyticsSingleton::setLandscapeOrientation()
{
    mixPanelRegisterSuperProperties("orientation", "landscape");
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
    std::string eventID = "accountCreated";
    
    std::map<std::string, std::string> mixPanelProperties;
    mixPanelProperties["Method"] = "App";
    
    mixPanelSendEventWithStoredProperties(eventID, mixPanelProperties);
    appsFlyerSendEvent(eventID, mixPanelProperties);
}

void AnalyticsSingleton::OnboardingAccountCreatedErrorEvent(long errorCode)
{
    std::string eventID = "accountCreatedError";
    
    std::map<std::string, std::string> mixPanelProperties;
    mixPanelProperties["ErrorCode"] = cocos2d::StringUtils::format("%ld", errorCode);
    
    mixPanelSendEventWithStoredProperties(eventID, mixPanelProperties);
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
    std::string eventID = "childProfileOomee";
    
    std::map<std::string, std::string> mixPanelProperties;
    mixPanelProperties["SelectedOomee"] = ConfigStorage::getInstance()->getHumanReadableNameForOomee(oomeeNumber);
    
    mixPanelSendEventWithStoredProperties(eventID, mixPanelProperties);
}

void AnalyticsSingleton::childProfileCreatedSuccessEvent(int oomeeNumber)
{
    std::string eventID = "childProfileCreatedSuccess";
    
    std::map<std::string, std::string> mixPanelProperties;
    mixPanelProperties["Method"] = "App";
    mixPanelProperties["SelectedOomee"] = ConfigStorage::getInstance()->getHumanReadableNameForOomee(oomeeNumber);
    
    mixPanelSendEventWithStoredProperties(eventID, mixPanelProperties);
}

void AnalyticsSingleton::childProfileCreatedErrorEvent(long errorCode)
{
    std::string eventID = "childProfileCreatedError";
    
    std::map<std::string, std::string> mixPanelProperties;
    mixPanelProperties["ErrorCode"] = cocos2d::StringUtils::format("%ld", errorCode);
    
    mixPanelSendEventWithStoredProperties(eventID, mixPanelProperties);
}
    
void AnalyticsSingleton::childProfileUpdateErrorEvent(long errorCode)
{
    std::string eventID = "childProfileUpdateError";
    
    std::map<std::string, std::string> mixPanelProperties;
    mixPanelProperties["ErrorCode"] = cocos2d::StringUtils::format("%ld", errorCode);
    
    mixPanelSendEventWithStoredProperties(eventID, mixPanelProperties);
}

//-------------HUB ACTIONS-------------------
void AnalyticsSingleton::hubTapOomeeEvent(int oomeeNumber, std::string oomeeAction)
{
    std::string eventID = "tapOomee";
    
    std::map<std::string, std::string> mixPanelProperties;
    mixPanelProperties["SelectedOomee"] = ConfigStorage::getInstance()->getHumanReadableNameForOomee(oomeeNumber);
    mixPanelProperties["OomeeAnimation"] = oomeeAction;
    
    mixPanelSendEventWithStoredProperties(eventID, mixPanelProperties);
}

void AnalyticsSingleton::navSelectionEvent(std::string hubOrTop, int buttonNumber)
{
    std::string eventID = "contentNavSelection";
    
    std::map<std::string, std::string> mixPanelProperties;
    mixPanelProperties["Type"] = ConfigStorage::getInstance()->getNameForMenuItem(buttonNumber);
    mixPanelProperties["Method"] = hubOrTop;
    
    mixPanelSendEventWithStoredProperties(eventID, mixPanelProperties);
}

//---------------- CONTENT ITEM ACTIONS------------------------
    
void AnalyticsSingleton::contentItemSelectedEvent(std::string Title,std::string Description, std::string Type, std::string contentID, int rowNumber, int elementNumber, std::string elementShape)
{
    SessionIdManager::getInstance()->resetBackgroundTimeInContent();
    
    std::string eventID = "contentItemSelectedEvent";
    
    std::map<std::string, std::string> mixPanelProperties;
    mixPanelProperties["Title"] = Title;
    mixPanelProperties["Description"] = Description;
    mixPanelProperties["Type"] = Type;
    mixPanelProperties["ContentID"] = contentID;
    mixPanelProperties["rowNumber"] = cocos2d::StringUtils::format("%d", rowNumber);
    mixPanelProperties["elementNumber"] = cocos2d::StringUtils::format("%d", elementNumber);
    mixPanelProperties["elementShape"] = elementShape;
    
    storedContentItemProperties = mixPanelProperties;
    
    mixPanelSendEventWithStoredProperties(eventID, mixPanelProperties);
    appsFlyerSendEvent(eventID, mixPanelProperties);
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
    
    std::string eventID = "contentItemClosed";
    
    storedContentItemProperties["SecondsInContent"] = cocos2d::StringUtils::format("%s%.f",NUMBER_IDENTIFIER, secondsOpened);
    
    mixPanelSendEventWithStoredProperties(eventID, storedContentItemProperties);
    
}

//------------- PREVIEW ACTIONS ---------------
void AnalyticsSingleton::previewContentClickedEvent(std::string Title, std::string Description, std::string Type)
{
    std::string eventID = "previewContentItemClicked";
    
    std::map<std::string, std::string> mixPanelProperties;
    mixPanelProperties["Title"] = Title;
    mixPanelProperties["Description"] = Description;
    mixPanelProperties["Type"] = Type;
    
    mixPanelSendEventWithStoredProperties(eventID, mixPanelProperties);
}

void AnalyticsSingleton::previewPopupCancelledEvent()
{
    mixPanelSendEventWithStoredProperties("previewEmailPopUpDismissed");
}

//---------------MEDIA ACTIONS -----------------
void AnalyticsSingleton::mediaPlayerQualityEvent(std::string quality)
{
    std::string eventID = "mediaQuality";
    
    std::map<std::string, std::string> mixPanelProperties = storedContentItemProperties;
    
    mixPanelProperties["Quality"] = quality;
    
    mixPanelSendEventWithStoredProperties(eventID, mixPanelProperties);
}

void AnalyticsSingleton::mediaPlayerProgressEvent(int percentComplete)
{
    std::string eventID = "mediaProgress";
    
    std::map<std::string, std::string> mixPanelProperties = storedContentItemProperties;
    mixPanelProperties["Progress"] = cocos2d::StringUtils::format("%d", percentComplete);
    
    mixPanelSendEventWithStoredProperties(eventID, mixPanelProperties);
}

void AnalyticsSingleton::mediaPlayerPausedEvent()
{
    mixPanelSendEventWithStoredProperties("mediaPause", storedContentItemProperties);
}

void AnalyticsSingleton::mediaPlayerEndEvent(int SecondsMediaPlayed)
{
    std::string eventID = "mediaEnd";
    
    std::map<std::string, std::string> mixPanelProperties = storedContentItemProperties;
    
    mixPanelProperties["SecondsMediaPlayed"] = cocos2d::StringUtils::format("%s%d",NUMBER_IDENTIFIER, SecondsMediaPlayed);

    mixPanelSendEventWithStoredProperties(eventID, mixPanelProperties);
}

void AnalyticsSingleton::mediaPlayerFirstFrameEvent(std::string loadTimeMS)
{
    std::string eventID = "mediaFirstFrame";
    
    std::map<std::string, std::string> mixPanelProperties = storedContentItemProperties;
    mixPanelProperties["LoadTime"] = cocos2d::StringUtils::format("%s%s",NUMBER_IDENTIFIER, loadTimeMS.c_str());
    
    mixPanelSendEventWithStoredProperties(eventID, mixPanelProperties);
}
    
void AnalyticsSingleton::mediaPlayerNewPlaylistItemSetEvent(int itemNumber)
{
    std::map<std::string, std::string> mixPanelProperties = storedContentItemProperties;
    mixPanelProperties["playlistElementNumber"] = cocos2d::StringUtils::format("%d", itemNumber);
    
    std::string eventID = "mediaNewPlaylistItem";
    mixPanelSendEventWithStoredProperties(eventID, mixPanelProperties);
}
    
void AnalyticsSingleton::mediaPlayerVideoPlayEvent()
{
    std::string eventID = "mediaVideoPlay";
    mixPanelSendEventWithStoredProperties(eventID, storedContentItemProperties);
}
    
void AnalyticsSingleton::mediaPlayerVideoCompletedEvent()
{
    std::string eventID = "mediaVideoCompleted";
    mixPanelSendEventWithStoredProperties(eventID, storedContentItemProperties);
}
    
void AnalyticsSingleton::mediaPlayerPlaylistCompletedEvent()
{
    std::string eventID = "mediaPlaylistCompleted";
    mixPanelSendEventWithStoredProperties(eventID, storedContentItemProperties);
}

//---------------OTHER ACTION------------------
void AnalyticsSingleton::genericButtonPressEvent(std::string buttonName)
{
    std::string eventID = "tapButton";
    
    std::map<std::string, std::string> mixPanelProperties;
    mixPanelProperties["ButtonName"] = buttonName;
    
    mixPanelSendEventWithStoredProperties(eventID, mixPanelProperties);
}

void AnalyticsSingleton::messageBoxShowEvent(std::string messageTitle, long errorCode)
{
    std::string eventID = "messageBoxDisplayed";
    
    std::map<std::string, std::string> mixPanelProperties;
    mixPanelProperties["MessageTitle"] = messageTitle;
    mixPanelProperties["ErrorCode"] = cocos2d::StringUtils::format("%ld", errorCode);
    
    mixPanelSendEventWithStoredProperties(eventID, mixPanelProperties);
}

void AnalyticsSingleton::localisedStringErrorEvent(std::string stringRequested, std::string languageUsed)
{
    std::string eventID = "localisedStringError";
    
    std::map<std::string, std::string> mixPanelProperties;
    mixPanelProperties["StringRequestFor"] = stringRequested;
    mixPanelProperties["WithLanguage"] = languageUsed;
    
    mixPanelSendEventWithStoredProperties(eventID, mixPanelProperties);
}

void AnalyticsSingleton::introVideoTimedOutError(std::string errorMessage)
{
    std::string eventID = "introVideoTimedOutError";
    
    std::map<std::string, std::string> mixPanelProperties;
    mixPanelProperties["ErrorMessage"] = errorMessage;
    
    mixPanelSendEventWithStoredProperties(eventID, mixPanelProperties);
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
    std::string eventID = "sessionIdHasChanged";
    
    std::map<std::string, std::string> mixPanelProperties;
    mixPanelProperties["oldSessionId"] = oldSessionId;
    
    mixPanelSendEventWithStoredProperties(eventID, mixPanelProperties);
}
    
void AnalyticsSingleton::httpRequestFailed(std::string requestTag, long responseCode, std::string qid)
{
    std::string eventID = "httpRequestFailed";
    
    std::map<std::string, std::string> mixPanelProperties;
    mixPanelProperties["requestTag"] = requestTag;
    mixPanelProperties["responseCode"] = cocos2d::StringUtils::format("%ld", responseCode);;
    mixPanelProperties["qid"] = qid;
    
    mixPanelSendEventWithStoredProperties(eventID, mixPanelProperties);
}

//---------------IAP ACTIONS------------------
  
void AnalyticsSingleton::displayIAPUpsaleEvent(std::string fromLocation)
{
    std::string eventID = "displayIAPUpsale";
    
    std::map<std::string, std::string> mixPanelProperties;
    mixPanelProperties["DisplayedFrom"] = fromLocation;
    
    mixPanelSendEventWithStoredProperties(eventID, mixPanelProperties);
}

void AnalyticsSingleton::iapSubscriptionSuccessEvent()
{
    std::string eventID = "iapSubscriptionSuccess";
    
    mixPanelSendEventWithStoredProperties(eventID);
    
    std::map<std::string, std::string> mixPanelProperties;
    mixPanelProperties["iAP_OS"] = OSManufacturer;
    
    appsFlyerSendEvent(eventID, mixPanelProperties);
    
    
}

void AnalyticsSingleton::iapSubscriptionErrorEvent(std::string errorDescription)
{
    std::string eventID = "iapSubscriptionError";
    
    std::map<std::string, std::string> mixPanelProperties;
    mixPanelProperties["ErrorType"] = errorDescription;
    
    mixPanelSendEventWithStoredProperties(eventID, mixPanelProperties);
}

void AnalyticsSingleton::iapSubscriptionFailedEvent()
{
    mixPanelSendEventWithStoredProperties("iapSubscriptionFailed");
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
    std::string eventID = "iapBackendRequestFailedError";
    
    std::map<std::string, std::string> mixPanelProperties;
    mixPanelProperties["ErrorCode"] = cocos2d::StringUtils::format("%ld", errorCode);
    
    mixPanelSendEventWithStoredProperties(eventID, mixPanelProperties);
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
    std::string eventID = "deepLinkingMoveToEvent";
    
    std::map<std::string, std::string> mixPanelProperties;
    mixPanelProperties["MoveTo"] = moveTo;
    
    mixPanelSendEventWithStoredProperties(eventID, mixPanelProperties);
}
    
void AnalyticsSingleton::deepLinkingContentEvent()
{
    mixPanelSendEventWithStoredProperties("deepLinkingContentEvent");
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

}
