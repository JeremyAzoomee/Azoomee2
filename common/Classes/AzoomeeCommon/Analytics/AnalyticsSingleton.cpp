#include "AnalyticsSingleton.h"
#include "../Utils/StringMgr.h"
#include "../Input/TextInputChecker.h"
#include "../Data/ConfigStorage.h"
#include "../Data/Parent/ParentDataProvider.h"
#include "../Utils/StringFunctions.h"
#include "../Strings.h"
#include "../Utils/SessionIdManager.h"


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
  std::map<std::string, std::string> mixPanelProperties;
  mixPanelProperties[key] = property;
  mixPanelRegisterSuperProperties( mixPanelProperties );
}

//-------------- SUPER PROPERTIES---------------------

void AnalyticsSingleton::registerAppVersion()
{
    mixPanelRegisterSuperProperties("appVersion",ConfigStorage::getInstance()->getVersionNumberToDisplay());
}

void AnalyticsSingleton::registerParentID(std::string ParentID)
{
    std::map<std::string, std::string> mixPanelProperties;
    mixPanelProperties["parentID"] = ParentID;
    
    mixPanelRegisterSuperProperties("parentID",ParentID);
    mixPanelRegisterIdentity(ParentID,mixPanelProperties);
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
}
    
void AnalyticsSingleton::registerCurrentScene(std::string currentScene)
{
    mixPanelRegisterSuperProperties("currentScene", currentScene);
}

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
}

//-------------Startup--------------------

void AnalyticsSingleton::firstLaunchEvent()
{
    mixPanelSendEvent("firstLaunch");
    appsFlyerSendEvent("firstLaunch");
}

// -------------- SIGN IN FUNCTIONS -----------------
void AnalyticsSingleton::signInSuccessEvent()
{
    mixPanelSendEvent("parentAppLoginSuccess");
}

void AnalyticsSingleton::signInFailEvent(int errorCode)
{
    mixPanelSendEvent("parentAppLoginFailure");
}

//-------------ONBOARDING--------------------
void AnalyticsSingleton::OnboardingStartEvent()
{
    mixPanelSendEvent("startCreateAccount");
}

void AnalyticsSingleton::OnboardingEmailSubmittedEvent(std::string emailAddress)
{
    mixPanelSendEvent("emailSubmitted");
    
    registerAzoomeeEmail(emailAddress);
}

void AnalyticsSingleton::OnboardingPasswordSubmittedEvent()
{
    mixPanelSendEvent("passwordSubmitted");
}

void AnalyticsSingleton::OnboardingPinSubmittedEvent()
{
    mixPanelSendEvent("pinSubmitted");
}

void AnalyticsSingleton::OnboardingAccountCreatedEvent()
{
    std::string eventID = "accountCreated";
    
    std::map<std::string, std::string> mixPanelProperties;
    mixPanelProperties["Method"] = "App";
    
    mixPanelSendEvent(eventID, mixPanelProperties);
    appsFlyerSendEvent(eventID, mixPanelProperties);
}

void AnalyticsSingleton::OnboardingAccountCreatedErrorEvent(long errorCode)
{
    std::string eventID = "accountCreatedError";
    
    std::map<std::string, std::string> mixPanelProperties;
    mixPanelProperties["ErrorCode"] = cocos2d::StringUtils::format("%ld", errorCode);
    
    mixPanelSendEvent(eventID, mixPanelProperties);
}

//-------------CHILD PROFILE CREATION-------------
void AnalyticsSingleton::childProfileStartEvent()
{
    mixPanelSendEvent("childProfileCreateStart");
}

void AnalyticsSingleton::childProfileNameEvent()
{
    mixPanelSendEvent("childProfileName");
}

void AnalyticsSingleton::childProdileNameErrorEvent()
{
    mixPanelSendEvent("childProfileNameError");
}

void AnalyticsSingleton::childProfileDOBEvent()
{
    mixPanelSendEvent("childProfileDOB");
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
    
    mixPanelSendEvent(eventID, mixPanelProperties);
}

void AnalyticsSingleton::childProfileCreatedSuccessEvent(int oomeeNumber)
{
    std::string eventID = "childProfileCreatedSuccess";
    
    std::map<std::string, std::string> mixPanelProperties;
    mixPanelProperties["Method"] = "App";
    mixPanelProperties["SelectedOomee"] = ConfigStorage::getInstance()->getHumanReadableNameForOomee(oomeeNumber);
    
    mixPanelSendEvent(eventID, mixPanelProperties);
}

void AnalyticsSingleton::childProfileCreatedErrorEvent(long errorCode)
{
    std::string eventID = "childProfileCreatedError";
    
    std::map<std::string, std::string> mixPanelProperties;
    mixPanelProperties["ErrorCode"] = cocos2d::StringUtils::format("%ld", errorCode);
    
    mixPanelSendEvent(eventID, mixPanelProperties);
}

//-------------HUB ACTIONS-------------------
void AnalyticsSingleton::hubTapOomeeEvent(int oomeeNumber, std::string oomeeAction)
{
    std::string eventID = "tapOomee";
    
    std::map<std::string, std::string> mixPanelProperties;
    mixPanelProperties["SelectedOomee"] = ConfigStorage::getInstance()->getHumanReadableNameForOomee(oomeeNumber);
    mixPanelProperties["OomeeAnimation"] = oomeeAction;
    
    mixPanelSendEvent(eventID, mixPanelProperties);
}

void AnalyticsSingleton::navSelectionEvent(std::string hubOrTop, int buttonNumber)
{
    std::string eventID = "contentNavSelection";
    
    std::map<std::string, std::string> mixPanelProperties;
    mixPanelProperties["Type"] = ConfigStorage::getInstance()->getNameForMenuItem(buttonNumber);
    mixPanelProperties["Method"] = hubOrTop;
    
    mixPanelSendEvent(eventID, mixPanelProperties);
}

    void AnalyticsSingleton::openContentEvent(std::string Title,std::string Description, std::string Type, std::string contentID, int rowNumber, int elementNumber, std::string elementShape)
{
    SessionIdManager::getInstance()->resetBackgroundTimeInContent();
    
    time(&timeOpenedContent);
    storedTitle = Title;
    storedDescription = Description;
    storedType = Type;
    storedContentID = contentID;
    storedRowNumber = rowNumber;
    storedElementNumber = elementNumber;
    storedElementShape = elementShape;
    
    std::string eventID = "openContent";
    
    std::map<std::string, std::string> mixPanelProperties;
    mixPanelProperties["Title"] = Title;
    mixPanelProperties["Description"] = Description;
    mixPanelProperties["Type"] = Type;
    mixPanelProperties["ContentID"] = contentID;
    mixPanelProperties["rowNumber"] = cocos2d::StringUtils::format("%d", rowNumber);
    mixPanelProperties["elementNumber"] = cocos2d::StringUtils::format("%d", elementNumber);
    mixPanelProperties["elementShape"] = elementShape;
    
    
    mixPanelSendEvent(eventID, mixPanelProperties);
    appsFlyerSendEvent(eventID, mixPanelProperties);
}

void AnalyticsSingleton::closeContentEvent()
{
    time_t now;
    time(&now);
    double secondsOpened = difftime(now,timeOpenedContent);
    
    cocos2d::log("CLOSE EVENT CONTENT TIME: %f", secondsOpened);
    cocos2d::log("CLOSE EVENT BACKGROUND TIME: %lu", SessionIdManager::getInstance()->getBackgroundTimeInContent());
    
    secondsOpened -= SessionIdManager::getInstance()->getBackgroundTimeInContent();
    
    std::string eventID = "closedContent";
    
    std::map<std::string, std::string> mixPanelProperties;
    
    mixPanelProperties["Title"] = {storedTitle,0};
    mixPanelProperties["Description"] = storedDescription;
    mixPanelProperties["Type"] = storedType;
    mixPanelProperties["ContentID"] = storedContentID;
    mixPanelProperties["SecondsInContent"] = cocos2d::StringUtils::format("%s%.f",NUMBER_IDENTIFIER, secondsOpened);
    mixPanelProperties["rowNumber"] = cocos2d::StringUtils::format("%d", storedRowNumber);
    mixPanelProperties["elementNumber"] = cocos2d::StringUtils::format("%d", storedElementNumber);
    mixPanelProperties["elementShape"] = storedElementShape;
    
    
    mixPanelSendEvent(eventID, mixPanelProperties);
}

//------------- PREVIEW ACTIONS ---------------
void AnalyticsSingleton::previewContentClickedEvent(std::string Title, std::string Description, std::string Type)
{
    std::string eventID = "previewContentItemClicked";
    
    std::map<std::string, std::string> mixPanelProperties;
    mixPanelProperties["Title"] = Title;
    mixPanelProperties["Description"] = Description;
    mixPanelProperties["Type"] = Type;
    
    mixPanelSendEvent(eventID, mixPanelProperties);
}

void AnalyticsSingleton::previewPopupCancelledEvent()
{
    mixPanelSendEvent("previewEmailPopUpDismissed");
}

//---------------MEDIA ACTIONS -----------------
void AnalyticsSingleton::mediaQualityEvent(std::string quality)
{
    std::string eventID = "mediaQuality";
    
    std::map<std::string, std::string> mixPanelProperties;
    
    mixPanelProperties["Title"] = storedTitle;
    mixPanelProperties["Description"] = storedDescription;
    mixPanelProperties["Type"] = storedType;
    mixPanelProperties["ContentID"] = storedContentID;
    mixPanelProperties["Quality"] = quality;
    
    mixPanelSendEvent(eventID, mixPanelProperties);
}

void AnalyticsSingleton::mediaProgressEvent(int percentComplete)
{
    std::string eventID = "mediaProgress";
    
    std::map<std::string, std::string> mixPanelProperties;
    
    mixPanelProperties["Title"] = storedTitle;
    mixPanelProperties["Description"] = storedDescription;
    mixPanelProperties["Type"] = storedType;
    mixPanelProperties["ContentID"] = storedContentID;
    mixPanelProperties["Progress"] = cocos2d::StringUtils::format("%d", percentComplete);
    
    mixPanelSendEvent(eventID, mixPanelProperties);
}

void AnalyticsSingleton::mediaPausedEvent()
{
    mixPanelSendEvent("mediaPause");
}

void AnalyticsSingleton::mediaEndEvent(int SecondsMediaPlayed)
{
    std::string eventID = "mediaEnd";
    
    std::map<std::string, std::string> mixPanelProperties;
    mixPanelProperties["Title"] = storedTitle;
    mixPanelProperties["Description"] = storedDescription;
    mixPanelProperties["Type"] = storedType;
    mixPanelProperties["ContentID"] = storedContentID;
    mixPanelProperties["SecondsMediaPlayed"] = cocos2d::StringUtils::format("%s%d",NUMBER_IDENTIFIER, SecondsMediaPlayed);

    mixPanelSendEvent(eventID, mixPanelProperties);
}

void AnalyticsSingleton::mediaPlayerFirstFrameEvent(std::string loadTimeMS)
{
    std::string eventID = "mediaFirstFrame";
    
    std::map<std::string, std::string> mixPanelProperties;
    mixPanelProperties["LoadTime"] = cocos2d::StringUtils::format("%s%s",NUMBER_IDENTIFIER, loadTimeMS.c_str());
    
    mixPanelSendEvent(eventID, mixPanelProperties);
}

//---------------OTHER ACTION------------------
void AnalyticsSingleton::genericButtonPressEvent(std::string buttonName)
{
    std::string eventID = "tapButton";
    
    std::map<std::string, std::string> mixPanelProperties;
    mixPanelProperties["ButtonName"] = buttonName;
    
    mixPanelSendEvent(eventID, mixPanelProperties);
}

void AnalyticsSingleton::messageBoxShowEvent(std::string messageTitle)
{
    std::string eventID = "messageBoxDisplayed";
    
    std::map<std::string, std::string> mixPanelProperties;
    mixPanelProperties["MessageTitle"] = messageTitle;
    
    mixPanelSendEvent(eventID, mixPanelProperties);
}

void AnalyticsSingleton::localisedStringErrorEvent(std::string stringRequested, std::string languageUsed)
{
    std::string eventID = "localisedStringError";
    
    std::map<std::string, std::string> mixPanelProperties;
    mixPanelProperties["StringRequestFor"] = stringRequested;
    mixPanelProperties["WithLanguage"] = languageUsed;
    
    mixPanelSendEvent(eventID, mixPanelProperties);
}

void AnalyticsSingleton::introVideoTimedOutError(std::string errorMessage)
{
    std::string eventID = "introVideoTimedOutError";
    
    std::map<std::string, std::string> mixPanelProperties;
    mixPanelProperties["ErrorMessage"] = errorMessage;
    
    mixPanelSendEvent(eventID, mixPanelProperties);
}
    
void AnalyticsSingleton::enteredBackgroundEvent()
{
    mixPanelSendEvent("enteredBackgroundEvent");
}
void AnalyticsSingleton::enteredForegroundEvent()
{
    mixPanelSendEvent("enteredForegroundEvent");
}
    
void AnalyticsSingleton::sessionIdHasChanged(std::string oldSessionId)
{
    std::string eventID = "sessionIdHasChanged";
    
    std::map<std::string, std::string> mixPanelProperties;
    mixPanelProperties["oldSessionId"] = oldSessionId;
    
    mixPanelSendEvent(eventID, mixPanelProperties);
}
    
void AnalyticsSingleton::httpRequestFailed(std::string requestTag, int responseCode, std::string qid)
{
    std::string eventID = "httpRequestFailed";
    
    std::map<std::string, std::string> mixPanelProperties;
    mixPanelProperties["requestTag"] = requestTag;
    mixPanelProperties["responseCode"] = cocos2d::StringUtils::format("%d", responseCode);;
    mixPanelProperties["qid"] = qid;
    
    mixPanelSendEvent(eventID, mixPanelProperties);
}

//---------------IAP ACTIONS------------------
  
void AnalyticsSingleton::displayIAPUpsaleEvent(std::string fromLocation)
{
    std::string eventID = "displayIAPUpsale";
    
    std::map<std::string, std::string> mixPanelProperties;
    mixPanelProperties["DisplayedFrom"] = fromLocation;
    
    mixPanelSendEvent(eventID, mixPanelProperties);
}

void AnalyticsSingleton::iapSubscriptionSuccessEvent()
{
    std::string eventID = "iapSubscriptionSuccess";
    
    mixPanelSendEvent(eventID);
    
    std::map<std::string, std::string> mixPanelProperties;
    mixPanelProperties["iAP_OS"] = OSManufacturer;
    
    appsFlyerSendEvent(eventID, mixPanelProperties);
    
    
}

void AnalyticsSingleton::iapSubscriptionErrorEvent(std::string errorDescription)
{
    std::string eventID = "iapSubscriptionError";
    
    std::map<std::string, std::string> mixPanelProperties;
    mixPanelProperties["ErrorType"] = errorDescription;
    
    mixPanelSendEvent(eventID, mixPanelProperties);
}

void AnalyticsSingleton::iapSubscriptionFailedEvent()
{
    mixPanelSendEvent("iapSubscriptionFailed");
}

void AnalyticsSingleton::iapUserDataFailedEvent()
{
    mixPanelSendEvent("iapUserDataFailed");
}

void AnalyticsSingleton::iapSubscriptionDoublePurchaseEvent()
{
    mixPanelSendEvent("iapSubscriptionDoublePurchase");
}

void AnalyticsSingleton::iapBackEndRequestFailedEvent(long errorCode)
{
    std::string eventID = "iapBackendRequestFailedError";
    
    std::map<std::string, std::string> mixPanelProperties;
    mixPanelProperties["ErrorCode"] = cocos2d::StringUtils::format("%ld", errorCode);
    
    mixPanelSendEvent(eventID, mixPanelProperties);
}

void AnalyticsSingleton::iapAppleAutoRenewSubscriptionEvent()
{
    mixPanelSendEvent("iapAppleAutoRenewSubscription");
}
    
}

