#include "MixPanelSingleton.h"
#include "StringStorage.h"
#include "TextInputChecker.h"
#include "ConfigStorage.h"


#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    #include "MixPanel_ios.h"
#endif

static MixPanelSingleton *_sharedConfigStorage = NULL;

MixPanelSingleton* MixPanelSingleton::getInstance()
{
    if (! _sharedConfigStorage)
    {
        _sharedConfigStorage = new MixPanelSingleton();
        _sharedConfigStorage->init();
    }
    
    return _sharedConfigStorage;
}

MixPanelSingleton::~MixPanelSingleton(void)
{
}

bool MixPanelSingleton::init(void)
{
    return true;
}

//---------------- PRIVATE FUNCTIONS -----------------

void MixPanelSingleton::createOSSpecficCall(std::string eventID)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    
    mixPanelSendiOSEvent(eventID);
    
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    mixPanel_androidJNIHelper(eventID,"");
    
#endif
}

void MixPanelSingleton::mixPanel_androidJNIHelper(std::string eventID, std::string propertiesJSONString)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    cocos2d::JniMethodInfo methodInfo;
    
    if (! cocos2d::JniHelper::getStaticMethodInfo(methodInfo, "org/cocos2dx/cpp/AppActivity", "sendMixPanelWithEventID", "(Ljava/lang/String;Ljava/lang/String;)V"))
    {
        return;
    }
    
    jstring jstringEventID= methodInfo.env->NewStringUTF(eventID.c_str());
    jstring jstringJSONProperties= methodInfo.env->NewStringUTF(propertiesJSONString.c_str());
    
    methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, jstringEventID,jstringJSONProperties);
    
    methodInfo.env->DeleteLocalRef(methodInfo.classID);
    
#endif
}

void MixPanelSingleton::appsFlyer_androidJNIHelper(std::string eventID, std::string propertiesJSONString)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    cocos2d::JniMethodInfo methodInfo;
    
    if (! cocos2d::JniHelper::getStaticMethodInfo(methodInfo, "org/cocos2dx/cpp/AppActivity", "sendAppsFlyerEvent", "(Ljava/lang/String;Ljava/lang/String;)V"))
    {
        return;
    }
    
    jstring jstringEventID= methodInfo.env->NewStringUTF(eventID.c_str());
    jstring jstringJSONProperties= methodInfo.env->NewStringUTF(propertiesJSONString.c_str());
    
    methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, jstringEventID,jstringJSONProperties);
    
    methodInfo.env->DeleteLocalRef(methodInfo.classID);
    
#endif
}

//-------------- SUPER PROPERTIES---------------------

void MixPanelSingleton::mixPanel_OSSpecificSuperPropertiesCall(std::string Key, std::string Property)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    
    std::map<std::string, std::string> mixPanelProperties;
    mixPanelProperties[Key] = Property;
    
    mixPanel_registerSuperProperties_iOS(mixPanelProperties);
    
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

void MixPanelSingleton::mixPanel_registerAppVersion()
{
    mixPanel_OSSpecificSuperPropertiesCall("appVersion",APP_VERSION_NUMBER);
}

void MixPanelSingleton::mixPanel_registerParentID(std::string ParentID)
{
    mixPanel_OSSpecificSuperPropertiesCall("parentID",ParentID);
}

void MixPanelSingleton::mixPanel_registerNoOfChildren(int noOfChildren)
{
    mixPanel_OSSpecificSuperPropertiesCall("noOfChildren",cocos2d::StringUtils::format("%d", noOfChildren));
}

void MixPanelSingleton::mixPanel_registerAzoomeeEmail(std::string emailAddress)
{
    std::string azoomeEmail = "NO";
    
    if (hasAzoomeeEmailAddress(emailAddress))
        azoomeEmail = "YES";
    
    mixPanel_OSSpecificSuperPropertiesCall("azoomeeEmail",azoomeEmail);
}

void MixPanelSingleton::mixPanel_registerAccountStatus(std::string Status)
{
    mixPanel_OSSpecificSuperPropertiesCall("accountStatus",Status);
}

void MixPanelSingleton::mixPanel_registerChildID(std::string ChildID)
{
    mixPanel_OSSpecificSuperPropertiesCall("childID",ChildID);
}

void MixPanelSingleton::mixPanel_registerChildGender(std::string ChildGender)
{
    mixPanel_OSSpecificSuperPropertiesCall("sex",ChildGender);
}

void MixPanelSingleton::mixPanel_registerChildDOB(std::string ChildDOB)
{
    mixPanel_OSSpecificSuperPropertiesCall("dob",ChildDOB);
}

void MixPanelSingleton::mixPanel_logoutChild()
{
    mixPanel_OSSpecificSuperPropertiesCall("childID","");
    mixPanel_OSSpecificSuperPropertiesCall("sex","");
    mixPanel_OSSpecificSuperPropertiesCall("dob","");
}

void MixPanelSingleton::mixPanel_logoutParent()
{
    mixPanel_logoutChild();
    mixPanel_OSSpecificSuperPropertiesCall("accountStatus","");
    mixPanel_OSSpecificSuperPropertiesCall("parentID","");
    mixPanel_OSSpecificSuperPropertiesCall("azoomeeEmail","");
}

//-------------Startup--------------------

void MixPanelSingleton::mixPanel_fistLaunchEvent()
{
    createOSSpecficCall("firstLaunch");
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    
    appsFlyerSendiOSEvent("firstLaunch");
    
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    appsFlyer_androidJNIHelper("firstLaunch", "");
    
#endif
}

// -------------- SIGN IN FUNCTIONS -----------------
void MixPanelSingleton::mixPanel_signInSuccessEvent()
{
    createOSSpecficCall("parentAppLoginSuccess");
}

void MixPanelSingleton::mixPanel_signInFailEvent(int errorCode)
{
    createOSSpecficCall("parentAppLoginFailure");
}

//-------------ONBOARDING--------------------
void MixPanelSingleton::mixPanel_OnboardingStartEvent()
{
    createOSSpecficCall("startCreateAccount");
}

void MixPanelSingleton::mixPanel_OnboardingEmailSubmittedEvent()
{
    createOSSpecficCall("emailSubmitted");
}

void MixPanelSingleton::mixPanel_OnboardingPasswordSubmittedEvent()
{
    createOSSpecficCall("passwordSubmitted");
}

void MixPanelSingleton::mixPanel_OnboardingPinSubmittedEvent()
{
    createOSSpecficCall("pinSubmitted");
}

void MixPanelSingleton::mixPanel_OnboardingAccountCreatedEvent()
{
    std::string eventID = "accountCreated";
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    std::map<std::string, std::string> mixPanelProperties;
    
    mixPanelProperties["Method"] = "App";
    
    mixPanelSendiOSEvent(mixPanelProperties, eventID);
    
    appsFlyerSendiOSEvent(eventID);
    
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    mixPanel_androidJNIHelper(eventID,"{\"Method\":\"App\"}");
    
    appsFlyer_androidJNIHelper(eventID, "{\"Method\":\"App\"}");
    
#endif
}

void MixPanelSingleton::mixPanel_OnboardingAccountCreatedErrorEvent(long errorCode)
{
    std::string eventID = "accountCreatedError";
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    std::map<std::string, std::string> mixPanelProperties;
    
    mixPanelProperties["ErrorCode"] = cocos2d::StringUtils::format("%ld", errorCode);
    
    mixPanelSendiOSEvent(mixPanelProperties, eventID);
    
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    mixPanel_androidJNIHelper(eventID,cocos2d::StringUtils::format("{\"ErrorCode\":\"%ld\"}", errorCode));
    
#endif
}

//-------------CHILD PROFILE CREATION-------------
void MixPanelSingleton::mixPanel_childProfileStartEvent()
{
    createOSSpecficCall("childProfileCreateStart");
}

void MixPanelSingleton::mixPanel_childProfileNameEvent()
{
    createOSSpecficCall("childProfileName");
}

void MixPanelSingleton::mixPanel_childProdileNameErrorEvent()
{
    createOSSpecficCall("childProfileNameError");
}

void MixPanelSingleton::mixPanel_childProfileDOBEvent()
{
    createOSSpecficCall("childProfileDOB");
}

void MixPanelSingleton::mixPanel_childProfileDOBErrorEvent()
{
    //There are no errors defined at present for this function.
    //createOSSpecficCall("childProfileDOBError");
}

void MixPanelSingleton::mixPanel_childProfileOomeeEvent(int oomeeNumber)
{
    std::string eventID = "childProfileOomee";
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    std::map<std::string, std::string> mixPanelProperties;
    
    mixPanelProperties["SelectedOomee"] = ConfigStorage::getInstance()->getOomeeColour(oomeeNumber);
    
    mixPanelSendiOSEvent(mixPanelProperties, eventID);
    
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    mixPanel_androidJNIHelper(eventID,cocos2d::StringUtils::format("{\"selectedOomee\":\"%s\"}",ConfigStorage::getInstance()->getOomeeColour(oomeeNumber).c_str()));
    
#endif
}

void MixPanelSingleton::mixPanel_childProfileCreatedSuccessEvent(int oomeeNumber)
{
    std::string eventID = "childProfileCreatedSuccess";
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    std::map<std::string, std::string> mixPanelProperties;
    
    mixPanelProperties["Method"] = "App";
    mixPanelProperties["SelectedOomee"] = ConfigStorage::getInstance()->getOomeeColour(oomeeNumber);
    
    mixPanelSendiOSEvent(mixPanelProperties, eventID);
    
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    mixPanel_androidJNIHelper(eventID,cocos2d::StringUtils::format("{\"Method\":\"App\",\"selectedOomee\":\"%s\"}",ConfigStorage::getInstance()->getOomeeColour(oomeeNumber).c_str()));
    
#endif
}

void MixPanelSingleton::mixPanel_childProfileCreatedErrorEvent(long errorCode)
{
    std::string eventID = "childProfileCreatedError";
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    std::map<std::string, std::string> mixPanelProperties;
    
    mixPanelProperties["ErrorCode"] = cocos2d::StringUtils::format("%ld", errorCode);
    
    mixPanelSendiOSEvent(mixPanelProperties, eventID);
    
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    mixPanel_androidJNIHelper(eventID,cocos2d::StringUtils::format("{\"ErrorCode\":\"%ld\"}", errorCode));
    
#endif
}

//-------------HUB ACTIONS-------------------
void MixPanelSingleton::mixPanel_hubTapOomee(int oomeeNumber, std::string oomeeAction)
{
    std::string eventID = "tapOomee";

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    std::map<std::string, std::string> mixPanelProperties;
    
    mixPanelProperties["SelectedOomee"] = ConfigStorage::getInstance()->getOomeeColour(oomeeNumber);
    mixPanelProperties["OomeeAnimation"] = oomeeAction;
    
    mixPanelSendiOSEvent(mixPanelProperties, eventID);
    
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    mixPanel_androidJNIHelper(eventID,cocos2d::StringUtils::format("{\"SelectedOomee\":\"%s\",\"OomeeAnimation\":\"%s\"}",ConfigStorage::getInstance()->getOomeeColour(oomeeNumber).c_str(),oomeeAction.c_str()));
    
#endif
}

void MixPanelSingleton::mixPanel_navSelectionEvent(std::string hubOrTop, int buttonNumber)
{
    std::string eventID = "contentNavSelection";
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    std::map<std::string, std::string> mixPanelProperties;
    
    mixPanelProperties["ErrorCode"] = ConfigStorage::getInstance()->getNameForMenuItem(buttonNumber);
    mixPanelProperties["Method"] = hubOrTop;
    
    mixPanelSendiOSEvent(mixPanelProperties, eventID);
    
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    mixPanel_androidJNIHelper(eventID,cocos2d::StringUtils::format("{\"ErrorCode\":\"%s\",\"Method\":\"%s\"}", ConfigStorage::getInstance()->getNameForMenuItem(buttonNumber).c_str(),hubOrTop.c_str()));
    
#endif
}

void MixPanelSingleton::mixPanel_openContentEvent(std::string Title,std::string Description, std::string Type, std::string contentID)
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
    
    mixPanelSendiOSEvent(mixPanelProperties, eventID);
    
    appsFlyerSendiOSEvent(mixPanelProperties, eventID);
    
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    mixPanel_androidJNIHelper(eventID,cocos2d::StringUtils::format("{\"Title\":\"%s\",\"Description\":\"%s\",\"Type\":\"%s\",\"ContentID\":\"%s\"}", Title.c_str(),Description.c_str(),Type.c_str(),contentID.c_str()));
    
    appsFlyer_androidJNIHelper(eventID, cocos2d::StringUtils::format("{\"Title\":\"%s\",\"Description\":\"%s\",\"Type\":\"%s\",\"ContentID\":\"%s\"}", Title.c_str(),Description.c_str(),Type.c_str(),contentID.c_str()));
    
#endif
}

void MixPanelSingleton::mixPanel_closeContentEvent()
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
    
    mixPanelSendiOSEvent(mixPanelProperties, eventID);
    
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    mixPanel_androidJNIHelper(eventID,cocos2d::StringUtils::format("{\"Title\":\"%s\",\"Description\":\"%s\",\"Type\":\"%s\",\"ContentID\":\"%s\",\"SecondsInContent\":\"%.f\"}", storedTitle.c_str(),storedDescription.c_str(),storedType.c_str(),storedContentID.c_str(), secondsOpened));
    
#endif

}

//------------- PREVIEW ACTIONS ---------------
void MixPanelSingleton::mixPanel_previewContentClickedEvent(std::string Title, std::string Description, std::string Type)
{
    std::string eventID = "previewContentItemClicked";
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    std::map<std::string, std::string> mixPanelProperties;
    
    mixPanelProperties["Title"] = Title;
    mixPanelProperties["Description"] = Description;
    mixPanelProperties["Type"] = Type;
    
    mixPanelSendiOSEvent(mixPanelProperties, eventID);
    
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    mixPanel_androidJNIHelper(eventID,cocos2d::StringUtils::format("{\"Title\":\"%s\",\"Description\":\"%s\",\"Type\":\"%s\"}", Title.c_str(),Description.c_str(),Type.c_str()));
    
#endif
}

void MixPanelSingleton::mixPanel_previewPopupCancelledEvent()
{
    createOSSpecficCall("previewEmailPopUpDismissed");
}

//---------------MEDIA ACTIONS -----------------
void MixPanelSingleton::mixPanel_mediaQuality(std::string quality)
{
    std::string eventID = "mediaQuality";
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    std::map<std::string, std::string> mixPanelProperties;
    
    mixPanelProperties["Title"] = storedTitle;
    mixPanelProperties["Description"] = storedDescription;
    mixPanelProperties["Type"] = storedType;
    mixPanelProperties["ContentID"] = storedContentID;
    mixPanelProperties["Quality"] = cocos2d::StringUtils::format("%s", quality.c_str());
    
    mixPanelSendiOSEvent(mixPanelProperties, eventID);
    
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    mixPanel_androidJNIHelper(eventID,cocos2d::StringUtils::format("{\"Title\":\"%s\",\"Description\":\"%s\",\"Type\":\"%s\",\"ContentID\":\"%s\",\"Quality\":\"%s\"}", storedTitle.c_str(),storedDescription.c_str(),storedType.c_str(),storedContentID.c_str(), quality.c_str()));
    
#endif
}

void MixPanelSingleton::mixPanel_mediaProgress(int percentComplete)
{
    std::string eventID = "mediaProgress";
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    std::map<std::string, std::string> mixPanelProperties;
    
    mixPanelProperties["Title"] = storedTitle;
    mixPanelProperties["Description"] = storedDescription;
    mixPanelProperties["Type"] = storedType;
    mixPanelProperties["ContentID"] = storedContentID;
    mixPanelProperties["Progress"] = cocos2d::StringUtils::format("%d", percentComplete);
    
    mixPanelSendiOSEvent(mixPanelProperties, eventID);
    
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    mixPanel_androidJNIHelper(eventID,cocos2d::StringUtils::format("{\"Title\":\"%s\",\"Description\":\"%s\",\"Type\":\"%s\",\"ContentID\":\"%s\",\"Progress\":\"%d\"}", storedTitle.c_str(),storedDescription.c_str(),storedType.c_str(),storedContentID.c_str(), percentComplete));
    
#endif
}

void MixPanelSingleton::mixPanel_mediaPausedEvent()
{
    createOSSpecficCall("mediaPause");
}

void MixPanelSingleton::mixPanel_mediaEnd(int SecondsMediaPlayed)
{
    std::string eventID = "mediaEnd";

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    std::map<std::string, std::string> mixPanelProperties;
    
    mixPanelProperties["Title"] = storedTitle;
    mixPanelProperties["Description"] = storedDescription;
    mixPanelProperties["Type"] = storedType;
    mixPanelProperties["ContentID"] = storedContentID;
    mixPanelProperties["SecondsMediaPlayed"] = cocos2d::StringUtils::format("%d", SecondsMediaPlayed);
    
    mixPanelSendiOSEvent(mixPanelProperties, eventID);
    
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    mixPanel_androidJNIHelper(eventID,cocos2d::StringUtils::format("{\"Title\":\"%s\",\"Description\":\"%s\",\"Type\":\"%s\",\"ContentID\":\"%s\",\"SecondsMediaPlayed\":\"%d\"}", storedTitle.c_str(),storedDescription.c_str(),storedType.c_str(),storedContentID.c_str(),SecondsMediaPlayed));
    
#endif

}

//---------------OTHER ACTION------------------
void MixPanelSingleton::mixPanel_genericButtonPress(std::string buttonName)
{
    std::string eventID = "tapButton";
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    std::map<std::string, std::string> mixPanelProperties;
    
    mixPanelProperties["ButtonName"] = buttonName;
    
    mixPanelSendiOSEvent(mixPanelProperties, eventID);
    
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    mixPanel_androidJNIHelper(eventID,cocos2d::StringUtils::format("{\"ButtonName\":\"%s\"}", buttonName.c_str()));
    
#endif
}

void MixPanelSingleton::mixPanel_messageBoxShow(std::string messageTitle)
{
    std::string eventID = "messageBoxDisplayed";
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    std::map<std::string, std::string> mixPanelProperties;
    
    mixPanelProperties["MessageTitle"] = messageTitle;
    
    mixPanelSendiOSEvent(mixPanelProperties, eventID);
    
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    mixPanel_androidJNIHelper(eventID,cocos2d::StringUtils::format("{\"MessageTitle\":\"%s\"}", messageTitle.c_str()));
    
#endif
}
