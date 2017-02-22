#include "AnalyticsSingleton.h"
#include "StringMgr.h"
#include "TextInputChecker.h"
#include "ConfigStorage.h"


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

//---------------- PRIVATE FUNCTIONS -----------------

void AnalyticsSingleton::createOSSpecficCall(std::string eventID)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    
    mixPanelSendEvent_ios(eventID);
    
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    mixPanelSendEvent_android(eventID);
    
#endif
}

//-------------- SUPER PROPERTIES---------------------

void AnalyticsSingleton::mixPanel_OSSpecificSuperPropertiesCall(std::string Key, std::string Property)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    
    std::map<std::string, std::string> mixPanelProperties;
    mixPanelProperties[Key] = Property;
    
    mixPanel_registerSuperProperties_ios(mixPanelProperties);
    
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    cocos2d::JniMethodInfo methodInfo;
    
    if (! cocos2d::JniHelper::getStaticMethodInfo(methodInfo, "org/cocos2dx/cpp/AppActivity", "sendMixPanelSuperProperties", "(Ljava/lang/String;)V"))
    {
        return;
    }
    
    std::string propertiesJSONString = cocos2d::StringUtils::format("{\"%s\":\"%s\"}", Key.c_str(),Property.c_str());
    
    jstring jstringJSONProperties= methodInfo.env->NewStringUTF(propertiesJSONString.c_str());
    
    methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID,jstringJSONProperties);
    
    methodInfo.env->DeleteLocalRef(methodInfo.classID);
    
#endif
}

void AnalyticsSingleton::mixPanel_registerAppVersion()
{
    mixPanel_OSSpecificSuperPropertiesCall("appVersion",APP_VERSION_NUMBER);
}

void AnalyticsSingleton::mixPanel_registerParentID(std::string ParentID)
{
    mixPanel_OSSpecificSuperPropertiesCall("parentID",ParentID);
}

void AnalyticsSingleton::mixPanel_registerNoOfChildren(int noOfChildren)
{
    mixPanel_OSSpecificSuperPropertiesCall("noOfChildren",cocos2d::StringUtils::format("%d", noOfChildren));
}

void AnalyticsSingleton::mixPanel_registerAzoomeeEmail(std::string emailAddress)
{
    std::string azoomeEmail = "NO";
    
    if (hasAzoomeeEmailAddress(emailAddress))
        azoomeEmail = "YES";
    
    mixPanel_OSSpecificSuperPropertiesCall("azoomeeEmail",azoomeEmail);
}

void AnalyticsSingleton::mixPanel_registerAccountStatus(std::string Status)
{
    mixPanel_OSSpecificSuperPropertiesCall("accountStatus",Status);
}

void AnalyticsSingleton::mixPanel_registerChildID(std::string ChildID)
{
    mixPanel_OSSpecificSuperPropertiesCall("childID",ChildID);
}

void AnalyticsSingleton::mixPanel_registerChildGender(std::string ChildGender)
{
    mixPanel_OSSpecificSuperPropertiesCall("sex",ChildGender);
}

void AnalyticsSingleton::mixPanel_registerChildDOB(std::string ChildDOB)
{
    mixPanel_OSSpecificSuperPropertiesCall("dob",ChildDOB);
}

void AnalyticsSingleton::mixPanel_logoutChild()
{
    mixPanel_OSSpecificSuperPropertiesCall("childID","");
    mixPanel_OSSpecificSuperPropertiesCall("sex","");
    mixPanel_OSSpecificSuperPropertiesCall("dob","");
}

void AnalyticsSingleton::mixPanel_logoutParent()
{
    mixPanel_logoutChild();
    mixPanel_OSSpecificSuperPropertiesCall("accountStatus","");
    mixPanel_OSSpecificSuperPropertiesCall("parentID","");
    mixPanel_OSSpecificSuperPropertiesCall("azoomeeEmail","");
}

//-------------Startup--------------------

void AnalyticsSingleton::mixPanel_fistLaunchEvent()
{
    createOSSpecficCall("firstLaunch");
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    
    appsFlyerSendEvent_ios("firstLaunch");
    
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    appsFlyer_androidJNIHelper("firstLaunch", "");
    
#endif
}

// -------------- SIGN IN FUNCTIONS -----------------
void AnalyticsSingleton::mixPanel_signInSuccessEvent()
{
    createOSSpecficCall("parentAppLoginSuccess");
}

void AnalyticsSingleton::mixPanel_signInFailEvent(int errorCode)
{
    createOSSpecficCall("parentAppLoginFailure");
}

//-------------ONBOARDING--------------------
void AnalyticsSingleton::mixPanel_OnboardingStartEvent()
{
    createOSSpecficCall("startCreateAccount");
}

void AnalyticsSingleton::mixPanel_OnboardingEmailSubmittedEvent()
{
    createOSSpecficCall("emailSubmitted");
}

void AnalyticsSingleton::mixPanel_OnboardingPasswordSubmittedEvent()
{
    createOSSpecficCall("passwordSubmitted");
}

void AnalyticsSingleton::mixPanel_OnboardingPinSubmittedEvent()
{
    createOSSpecficCall("pinSubmitted");
}

void AnalyticsSingleton::mixPanel_OnboardingAccountCreatedEvent()
{
    std::string eventID = "accountCreated";
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    std::map<std::string, std::string> mixPanelProperties;
    
    mixPanelProperties["Method"] = "App";
    
    mixPanelSendEvent_ios(mixPanelProperties, eventID);
    
    appsFlyerSendEvent_ios(eventID);
    
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    mixPanel_androidJNIHelper(eventID,"{\"Method\":\"App\"}");
    
    appsFlyer_androidJNIHelper(eventID, "{\"Method\":\"App\"}");
    
#endif
}

void AnalyticsSingleton::mixPanel_OnboardingAccountCreatedErrorEvent(long errorCode)
{
    std::string eventID = "accountCreatedError";
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    std::map<std::string, std::string> mixPanelProperties;
    
    mixPanelProperties["ErrorCode"] = cocos2d::StringUtils::format("%ld", errorCode);
    
    mixPanelSendEvent_ios(mixPanelProperties, eventID);
    
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    mixPanel_androidJNIHelper(eventID,cocos2d::StringUtils::format("{\"ErrorCode\":\"%ld\"}", errorCode));
    
#endif
}

//-------------CHILD PROFILE CREATION-------------
void AnalyticsSingleton::mixPanel_childProfileStartEvent()
{
    createOSSpecficCall("childProfileCreateStart");
}

void AnalyticsSingleton::mixPanel_childProfileNameEvent()
{
    createOSSpecficCall("childProfileName");
}

void AnalyticsSingleton::mixPanel_childProdileNameErrorEvent()
{
    createOSSpecficCall("childProfileNameError");
}

void AnalyticsSingleton::mixPanel_childProfileDOBEvent()
{
    createOSSpecficCall("childProfileDOB");
}

void AnalyticsSingleton::mixPanel_childProfileDOBErrorEvent()
{
    //There are no errors defined at present for this function.
    //createOSSpecficCall("childProfileDOBError");
}

void AnalyticsSingleton::mixPanel_childProfileOomeeEvent(int oomeeNumber)
{
    std::string eventID = "childProfileOomee";
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    std::map<std::string, std::string> mixPanelProperties;
    
    mixPanelProperties["SelectedOomee"] = ConfigStorage::getInstance()->getOomeeColour(oomeeNumber);
    
    mixPanelSendEvent_ios(mixPanelProperties, eventID);
    
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    mixPanel_androidJNIHelper(eventID,cocos2d::StringUtils::format("{\"selectedOomee\":\"%s\"}",ConfigStorage::getInstance()->getOomeeColour(oomeeNumber).c_str()));
    
#endif
}

void AnalyticsSingleton::mixPanel_childProfileCreatedSuccessEvent(int oomeeNumber)
{
    std::string eventID = "childProfileCreatedSuccess";
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    std::map<std::string, std::string> mixPanelProperties;
    
    mixPanelProperties["Method"] = "App";
    mixPanelProperties["SelectedOomee"] = ConfigStorage::getInstance()->getOomeeColour(oomeeNumber);
    
    mixPanelSendEvent_ios(mixPanelProperties, eventID);
    
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    mixPanel_androidJNIHelper(eventID,cocos2d::StringUtils::format("{\"Method\":\"App\",\"selectedOomee\":\"%s\"}",ConfigStorage::getInstance()->getOomeeColour(oomeeNumber).c_str()));
    
#endif
}

void AnalyticsSingleton::mixPanel_childProfileCreatedErrorEvent(long errorCode)
{
    std::string eventID = "childProfileCreatedError";
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    std::map<std::string, std::string> mixPanelProperties;
    
    mixPanelProperties["ErrorCode"] = cocos2d::StringUtils::format("%ld", errorCode);
    
    mixPanelSendEvent_ios(mixPanelProperties, eventID);
    
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    mixPanel_androidJNIHelper(eventID,cocos2d::StringUtils::format("{\"ErrorCode\":\"%ld\"}", errorCode));
    
#endif
}

//-------------HUB ACTIONS-------------------
void AnalyticsSingleton::mixPanel_hubTapOomee(int oomeeNumber, std::string oomeeAction)
{
    std::string eventID = "tapOomee";

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    std::map<std::string, std::string> mixPanelProperties;
    
    mixPanelProperties["SelectedOomee"] = ConfigStorage::getInstance()->getOomeeColour(oomeeNumber);
    mixPanelProperties["OomeeAnimation"] = oomeeAction;
    
    mixPanelSendEvent_ios(mixPanelProperties, eventID);
    
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    mixPanel_androidJNIHelper(eventID,cocos2d::StringUtils::format("{\"SelectedOomee\":\"%s\",\"OomeeAnimation\":\"%s\"}",ConfigStorage::getInstance()->getOomeeColour(oomeeNumber).c_str(),oomeeAction.c_str()));
    
#endif
}

void AnalyticsSingleton::mixPanel_navSelectionEvent(std::string hubOrTop, int buttonNumber)
{
    std::string eventID = "contentNavSelection";
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    std::map<std::string, std::string> mixPanelProperties;
    
    mixPanelProperties["ErrorCode"] = ConfigStorage::getInstance()->getNameForMenuItem(buttonNumber);
    mixPanelProperties["Method"] = hubOrTop;
    
    mixPanelSendEvent_ios(mixPanelProperties, eventID);
    
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    mixPanel_androidJNIHelper(eventID,cocos2d::StringUtils::format("{\"ErrorCode\":\"%s\",\"Method\":\"%s\"}", ConfigStorage::getInstance()->getNameForMenuItem(buttonNumber).c_str(),hubOrTop.c_str()));
    
#endif
}

void AnalyticsSingleton::mixPanel_openContentEvent(std::string Title,std::string Description, std::string Type, std::string contentID)
{
    time(&timeOpenedContent);
    storedTitle = Title;
    storedDescription = Description;
    storedType = Type;
    storedContentID = contentID;
    
    std::string eventID = "openContent";
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    std::map<std::string, std::string> mixPanelProperties;
    
    mixPanelProperties["Title"] = Title;
    mixPanelProperties["Description"] = Description;
    mixPanelProperties["Type"] = Type;
    mixPanelProperties["ContentID"] = contentID;
    
    mixPanelSendEvent_ios(mixPanelProperties, eventID);
    
    appsFlyerSendEvent_ios(mixPanelProperties, eventID);
    
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    mixPanel_androidJNIHelper(eventID,cocos2d::StringUtils::format("{\"Title\":\"%s\",\"Description\":\"%s\",\"Type\":\"%s\",\"ContentID\":\"%s\"}", Title.c_str(),Description.c_str(),Type.c_str(),contentID.c_str()));
    
    appsFlyer_androidJNIHelper(eventID, cocos2d::StringUtils::format("{\"Title\":\"%s\",\"Description\":\"%s\",\"Type\":\"%s\",\"ContentID\":\"%s\"}", Title.c_str(),Description.c_str(),Type.c_str(),contentID.c_str()));
    
#endif
}

void AnalyticsSingleton::mixPanel_closeContentEvent()
{
    time_t now;
    time(&now);
    double secondsOpened = difftime(now,timeOpenedContent);
    
    std::string eventID = "closedContent";
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    std::map<std::string, std::string> mixPanelProperties;
    
    mixPanelProperties["Title"] = storedTitle;
    mixPanelProperties["Description"] = storedDescription;
    mixPanelProperties["Type"] = storedType;
    mixPanelProperties["ContentID"] = storedContentID;
    mixPanelProperties["SecondsInContent"] = cocos2d::StringUtils::format("%.f", secondsOpened);
    
    mixPanelSendEvent_ios(mixPanelProperties, eventID);
    
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    mixPanel_androidJNIHelper(eventID,cocos2d::StringUtils::format("{\"Title\":\"%s\",\"Description\":\"%s\",\"Type\":\"%s\",\"ContentID\":\"%s\",\"SecondsInContent\":\"%.f\"}", storedTitle.c_str(),storedDescription.c_str(),storedType.c_str(),storedContentID.c_str(), secondsOpened));
    
#endif

}

//------------- PREVIEW ACTIONS ---------------
void AnalyticsSingleton::mixPanel_previewContentClickedEvent(std::string Title, std::string Description, std::string Type)
{
    std::string eventID = "previewContentItemClicked";
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    std::map<std::string, std::string> mixPanelProperties;
    
    mixPanelProperties["Title"] = Title;
    mixPanelProperties["Description"] = Description;
    mixPanelProperties["Type"] = Type;
    
    mixPanelSendEvent_ios(mixPanelProperties, eventID);
    
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    mixPanel_androidJNIHelper(eventID,cocos2d::StringUtils::format("{\"Title\":\"%s\",\"Description\":\"%s\",\"Type\":\"%s\"}", Title.c_str(),Description.c_str(),Type.c_str()));
    
#endif
}

void AnalyticsSingleton::mixPanel_previewPopupCancelledEvent()
{
    createOSSpecficCall("previewEmailPopUpDismissed");
}

//---------------MEDIA ACTIONS -----------------
void AnalyticsSingleton::mixPanel_mediaQuality(std::string quality)
{
    std::string eventID = "mediaQuality";
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    std::map<std::string, std::string> mixPanelProperties;
    
    mixPanelProperties["Title"] = storedTitle;
    mixPanelProperties["Description"] = storedDescription;
    mixPanelProperties["Type"] = storedType;
    mixPanelProperties["ContentID"] = storedContentID;
    mixPanelProperties["Quality"] = cocos2d::StringUtils::format("%s", quality.c_str());
    
    mixPanelSendEvent_ios(mixPanelProperties, eventID);
    
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    mixPanel_androidJNIHelper(eventID,cocos2d::StringUtils::format("{\"Title\":\"%s\",\"Description\":\"%s\",\"Type\":\"%s\",\"ContentID\":\"%s\",\"Quality\":\"%s\"}", storedTitle.c_str(),storedDescription.c_str(),storedType.c_str(),storedContentID.c_str(), quality.c_str()));
    
#endif
}

void AnalyticsSingleton::mixPanel_mediaProgress(int percentComplete)
{
    std::string eventID = "mediaProgress";
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    std::map<std::string, std::string> mixPanelProperties;
    
    mixPanelProperties["Title"] = storedTitle;
    mixPanelProperties["Description"] = storedDescription;
    mixPanelProperties["Type"] = storedType;
    mixPanelProperties["ContentID"] = storedContentID;
    mixPanelProperties["Progress"] = cocos2d::StringUtils::format("%d", percentComplete);
    
    mixPanelSendEvent_ios(mixPanelProperties, eventID);
    
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    mixPanel_androidJNIHelper(eventID,cocos2d::StringUtils::format("{\"Title\":\"%s\",\"Description\":\"%s\",\"Type\":\"%s\",\"ContentID\":\"%s\",\"Progress\":\"%d\"}", storedTitle.c_str(),storedDescription.c_str(),storedType.c_str(),storedContentID.c_str(), percentComplete));
    
#endif
}

void AnalyticsSingleton::mixPanel_mediaPausedEvent()
{
    createOSSpecficCall("mediaPause");
}

void AnalyticsSingleton::mixPanel_mediaEnd(int SecondsMediaPlayed)
{
    std::string eventID = "mediaEnd";

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    std::map<std::string, std::string> mixPanelProperties;
    
    mixPanelProperties["Title"] = storedTitle;
    mixPanelProperties["Description"] = storedDescription;
    mixPanelProperties["Type"] = storedType;
    mixPanelProperties["ContentID"] = storedContentID;
    mixPanelProperties["SecondsMediaPlayed"] = cocos2d::StringUtils::format("%d", SecondsMediaPlayed);
    
    mixPanelSendEvent_ios(mixPanelProperties, eventID);
    
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    mixPanel_androidJNIHelper(eventID,cocos2d::StringUtils::format("{\"Title\":\"%s\",\"Description\":\"%s\",\"Type\":\"%s\",\"ContentID\":\"%s\",\"SecondsMediaPlayed\":\"%d\"}", storedTitle.c_str(),storedDescription.c_str(),storedType.c_str(),storedContentID.c_str(),SecondsMediaPlayed));
    
#endif

}

//---------------OTHER ACTION------------------
void AnalyticsSingleton::mixPanel_genericButtonPress(std::string buttonName)
{
    std::string eventID = "tapButton";
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    std::map<std::string, std::string> mixPanelProperties;
    
    mixPanelProperties["ButtonName"] = buttonName;
    
    mixPanelSendEvent_ios(mixPanelProperties, eventID);
    
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    mixPanel_androidJNIHelper(eventID,cocos2d::StringUtils::format("{\"ButtonName\":\"%s\"}", buttonName.c_str()));
    
#endif
}

void AnalyticsSingleton::mixPanel_messageBoxShow(std::string messageTitle)
{
    std::string eventID = "messageBoxDisplayed";
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    std::map<std::string, std::string> mixPanelProperties;
    
    mixPanelProperties["MessageTitle"] = messageTitle;
    
    mixPanelSendEvent_ios(mixPanelProperties, eventID);
    
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    mixPanel_androidJNIHelper(eventID,cocos2d::StringUtils::format("{\"MessageTitle\":\"%s\"}", messageTitle.c_str()));
    
#endif
}
