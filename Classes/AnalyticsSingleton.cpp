#include "AnalyticsSingleton.h"
#include "StringMgr.h"
#include "TextInputChecker.h"
#include "ConfigStorage.h"
#include "Analytics_android.h"


#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    #include "Analytics_ios.h"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    #include "Analytics_android.h"
#endif

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

//----------------MIX PANEL---------------

void AnalyticsSingleton::mixPanel_createOSSpecficCall(std::string eventID)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    
    mixPanelSendEvent_ios(eventID);
    
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    mixPanelSendEvent_android(eventID);
    
#endif
}

void AnalyticsSingleton::mixPanel_createOSSpecficCall(std::string eventID, std::map<std::string, std::string> map)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    
    mixPanelSendEvent_ios(eventID, map);
    
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    mixPanelSendEvent_android(eventID, map);
    
#endif
}

void AnalyticsSingleton::mixPanel_OSSpecificSuperPropertiesCall(std::string Key, std::string Property)
{
    std::map<std::string, std::string> mixPanelProperties;
    mixPanelProperties[Key] = Property;
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    
    mixPanel_registerSuperProperties_ios(mixPanelProperties);
    
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    mixPanel_registerSuperProperties_android(mixPanelProperties);
    
#endif
}

//-------------- APPSFLYER -----------------------

void AnalyticsSingleton::appsflyer_createOSSpecficCall(std::string eventID)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    
    appsFlyerSendEvent_ios(eventID);
    
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    appsFlyerSendEvent_android(eventID);
    
#endif
}

void AnalyticsSingleton::appsflyer_createOSSpecficCall(std::string eventID, std::map<std::string, std::string> map)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    
    appsFlyerSendEvent_ios(eventID, map);
    
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    appsFlyerSendEvent_android(eventID, map);
    
#endif
}

//-------------- SUPER PROPERTIES---------------------
void AnalyticsSingleton::registerAppVersion()
{
    mixPanel_OSSpecificSuperPropertiesCall("appVersion",APP_VERSION_NUMBER);
}

void AnalyticsSingleton::registerParentID(std::string ParentID)
{
    mixPanel_OSSpecificSuperPropertiesCall("parentID",ParentID);
}

void AnalyticsSingleton::registerNoOfChildren(int noOfChildren)
{
    mixPanel_OSSpecificSuperPropertiesCall("noOfChildren",cocos2d::StringUtils::format("%s%d",NUMBER_IDENTIFIER, noOfChildren));
}

void AnalyticsSingleton::registerAzoomeeEmail(std::string emailAddress)
{
    std::string azoomeEmail = "NO";
    
    if (hasAzoomeeEmailAddress(emailAddress))
        azoomeEmail = "YES";
    
    mixPanel_OSSpecificSuperPropertiesCall("azoomeeEmail",azoomeEmail);
}

void AnalyticsSingleton::registerAccountStatus(std::string Status)
{
    mixPanel_OSSpecificSuperPropertiesCall("accountStatus",Status);
}

void AnalyticsSingleton::registerChildID(std::string ChildID)
{
    mixPanel_OSSpecificSuperPropertiesCall("childID",ChildID);
}

void AnalyticsSingleton::registerChildGender(std::string ChildGender)
{
    mixPanel_OSSpecificSuperPropertiesCall("sex",ChildGender);
}

void AnalyticsSingleton::registerChildDOB(std::string ChildDOB)
{
    mixPanel_OSSpecificSuperPropertiesCall("dob",ChildDOB);
}

void AnalyticsSingleton::logoutChildEvent()
{
    mixPanel_OSSpecificSuperPropertiesCall("childID","");
    mixPanel_OSSpecificSuperPropertiesCall("sex","");
    mixPanel_OSSpecificSuperPropertiesCall("dob","");
}

void AnalyticsSingleton::logoutParentEvent()
{
    logoutChildEvent();
    mixPanel_OSSpecificSuperPropertiesCall("accountStatus","");
    mixPanel_OSSpecificSuperPropertiesCall("parentID","");
    mixPanel_OSSpecificSuperPropertiesCall("azoomeeEmail","");
}

//-------------Startup--------------------

void AnalyticsSingleton::firstLaunchEvent()
{
    mixPanel_createOSSpecficCall("firstLaunch");
    appsflyer_createOSSpecficCall("firstLaunch");
}

// -------------- SIGN IN FUNCTIONS -----------------
void AnalyticsSingleton::signInSuccessEvent()
{
    mixPanel_createOSSpecficCall("parentAppLoginSuccess");
}

void AnalyticsSingleton::signInFailEvent(int errorCode)
{
    mixPanel_createOSSpecficCall("parentAppLoginFailure");
}

//-------------ONBOARDING--------------------
void AnalyticsSingleton::OnboardingStartEvent()
{
    mixPanel_createOSSpecficCall("startCreateAccount");
}

void AnalyticsSingleton::OnboardingEmailSubmittedEvent(std::string emailAddress)
{
    mixPanel_createOSSpecficCall("emailSubmitted");
    
    registerAzoomeeEmail(emailAddress);
}

void AnalyticsSingleton::OnboardingPasswordSubmittedEvent()
{
    mixPanel_createOSSpecficCall("passwordSubmitted");
}

void AnalyticsSingleton::OnboardingPinSubmittedEvent()
{
    mixPanel_createOSSpecficCall("pinSubmitted");
}

void AnalyticsSingleton::OnboardingAccountCreatedEvent()
{
    std::string eventID = "accountCreated";
    
    std::map<std::string, std::string> mixPanelProperties;
    mixPanelProperties["Method"] = "App";
    
    mixPanel_createOSSpecficCall(eventID, mixPanelProperties);
    appsflyer_createOSSpecficCall(eventID, mixPanelProperties);
}

void AnalyticsSingleton::OnboardingAccountCreatedErrorEvent(long errorCode)
{
    std::string eventID = "accountCreatedError";
    
    std::map<std::string, std::string> mixPanelProperties;
    mixPanelProperties["ErrorCode"] = cocos2d::StringUtils::format("%ld", errorCode);
    
    mixPanel_createOSSpecficCall(eventID, mixPanelProperties);
}

//-------------CHILD PROFILE CREATION-------------
void AnalyticsSingleton::childProfileStartEvent()
{
    mixPanel_createOSSpecficCall("childProfileCreateStart");
}

void AnalyticsSingleton::childProfileNameEvent()
{
    mixPanel_createOSSpecficCall("childProfileName");
}

void AnalyticsSingleton::childProdileNameErrorEvent()
{
    mixPanel_createOSSpecficCall("childProfileNameError");
}

void AnalyticsSingleton::childProfileDOBEvent()
{
    mixPanel_createOSSpecficCall("childProfileDOB");
}

void AnalyticsSingleton::childProfileDOBErrorEvent()
{
    //There are no errors defined at present for this function.
    //createOSSpecficCall("childProfileDOBError");
}

void AnalyticsSingleton::childProfileOomeeEvent(int oomeeNumber)
{
    std::string eventID = "childProfileOomee";
    
    std::map<std::string, std::string> mixPanelProperties;
    mixPanelProperties["SelectedOomee"] = ConfigStorage::getInstance()->getHumanReadableNameForOomee(oomeeNumber);
    
    mixPanel_createOSSpecficCall(eventID, mixPanelProperties);
}

void AnalyticsSingleton::childProfileCreatedSuccessEvent(int oomeeNumber)
{
    std::string eventID = "childProfileCreatedSuccess";
    
    std::map<std::string, std::string> mixPanelProperties;
    mixPanelProperties["Method"] = "App";
    mixPanelProperties["SelectedOomee"] = ConfigStorage::getInstance()->getHumanReadableNameForOomee(oomeeNumber);
    
    mixPanel_createOSSpecficCall(eventID, mixPanelProperties);
}

void AnalyticsSingleton::childProfileCreatedErrorEvent(long errorCode)
{
    std::string eventID = "childProfileCreatedError";
    
    std::map<std::string, std::string> mixPanelProperties;
    mixPanelProperties["ErrorCode"] = cocos2d::StringUtils::format("%ld", errorCode);
    
    mixPanel_createOSSpecficCall(eventID, mixPanelProperties);
}

//-------------HUB ACTIONS-------------------
void AnalyticsSingleton::hubTapOomeeEvent(int oomeeNumber, std::string oomeeAction)
{
    std::string eventID = "tapOomee";
    
    std::map<std::string, std::string> mixPanelProperties;
    mixPanelProperties["SelectedOomee"] = ConfigStorage::getInstance()->getHumanReadableNameForOomee(oomeeNumber);
    mixPanelProperties["OomeeAnimation"] = oomeeAction;
    
    mixPanel_createOSSpecficCall(eventID, mixPanelProperties);
}

void AnalyticsSingleton::navSelectionEvent(std::string hubOrTop, int buttonNumber)
{
    std::string eventID = "contentNavSelection";
    
    std::map<std::string, std::string> mixPanelProperties;
    mixPanelProperties["Type"] = ConfigStorage::getInstance()->getNameForMenuItem(buttonNumber);
    mixPanelProperties["Method"] = hubOrTop;
    
    mixPanel_createOSSpecficCall(eventID, mixPanelProperties);
}

void AnalyticsSingleton::openContentEvent(std::string Title,std::string Description, std::string Type, std::string contentID)
{
    time(&timeOpenedContent);
    storedTitle = Title;
    storedDescription = Description;
    storedType = Type;
    storedContentID = contentID;
    
    std::string eventID = "openContent";
    
    std::map<std::string, std::string> mixPanelProperties;
    mixPanelProperties["Title"] = Title;
    mixPanelProperties["Description"] = Description;
    mixPanelProperties["Type"] = Type;
    mixPanelProperties["ContentID"] = contentID;
    
    mixPanel_createOSSpecficCall(eventID, mixPanelProperties);
    appsflyer_createOSSpecficCall(eventID, mixPanelProperties);
}

void AnalyticsSingleton::closeContentEvent()
{
    time_t now;
    time(&now);
    double secondsOpened = difftime(now,timeOpenedContent);
    
    std::string eventID = "closedContent";
    
    std::map<std::string, std::string> mixPanelProperties;
    
    mixPanelProperties["Title"] = {storedTitle,0};
    mixPanelProperties["Description"] = storedDescription;
    mixPanelProperties["Type"] = storedType;
    mixPanelProperties["ContentID"] = storedContentID;
    mixPanelProperties["SecondsInContent"] = cocos2d::StringUtils::format("%s%.f",NUMBER_IDENTIFIER, secondsOpened);
    
    mixPanel_createOSSpecficCall(eventID, mixPanelProperties);
}

//------------- PREVIEW ACTIONS ---------------
void AnalyticsSingleton::previewContentClickedEvent(std::string Title, std::string Description, std::string Type)
{
    std::string eventID = "previewContentItemClicked";
    
    std::map<std::string, std::string> mixPanelProperties;
    mixPanelProperties["Title"] = Title;
    mixPanelProperties["Description"] = Description;
    mixPanelProperties["Type"] = Type;
    
    mixPanel_createOSSpecficCall(eventID, mixPanelProperties);
}

void AnalyticsSingleton::previewPopupCancelledEvent()
{
    mixPanel_createOSSpecficCall("previewEmailPopUpDismissed");
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
    
    mixPanel_createOSSpecficCall(eventID, mixPanelProperties);
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
    
    mixPanel_createOSSpecficCall(eventID, mixPanelProperties);
}

void AnalyticsSingleton::mediaPausedEvent()
{
    mixPanel_createOSSpecficCall("mediaPause");
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

    mixPanel_createOSSpecficCall(eventID, mixPanelProperties);
}

//---------------OTHER ACTION------------------
void AnalyticsSingleton::genericButtonPressEvent(std::string buttonName)
{
    std::string eventID = "tapButton";
    
    std::map<std::string, std::string> mixPanelProperties;
    mixPanelProperties["ButtonName"] = buttonName;
    
    mixPanel_createOSSpecficCall(eventID, mixPanelProperties);
}

void AnalyticsSingleton::messageBoxShowEvent(std::string messageTitle)
{
    std::string eventID = "messageBoxDisplayed";
    
    std::map<std::string, std::string> mixPanelProperties;
    mixPanelProperties["MessageTitle"] = messageTitle;
    
    mixPanel_createOSSpecficCall(eventID, mixPanelProperties);
}

void AnalyticsSingleton::localisedStringErrorEvent(std::string stringRequested, std::string languageUsed)
{
    std::string eventID = "localisedStringError";
    
    std::map<std::string, std::string> mixPanelProperties;
    mixPanelProperties["StringRequestFor"] = stringRequested;
    mixPanelProperties["WithLanguage"] = languageUsed;
    
    mixPanel_createOSSpecficCall(eventID, mixPanelProperties);
}
