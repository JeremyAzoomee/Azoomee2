#include "MixPanelCalls.h"
#include "StringStorage.h"
#include "TextInputChecker.h"
#include "ConfigStorage.h"


#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    #include "MixPanel_ios.h"
#endif

void createOSSpecficCall(std::string eventID)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    
    mixPanelSendiOSEvent(eventID);
    
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    mixPanel_androidJNIHelper(eventID,"");
    
#endif
}

void mixPanel_androidJNIHelper(std::string eventID, std::string propertiesJSONString)
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

//-------------- SUPER PROPERTIES---------------------

void mixPanel_OSSpecificSuperPropertiesCall(std::string Key, std::string Property)
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

void mixPanel_registerAppVersion()
{
    mixPanel_OSSpecificSuperPropertiesCall("appVersion",APP_VERSION_NUMBER);
}

void mixPanel_registerParentID(std::string ParentID)
{
    mixPanel_OSSpecificSuperPropertiesCall("parentID",ParentID);
}

void mixPanel_registerNoOfChildren(int noOfChildren)
{
    mixPanel_OSSpecificSuperPropertiesCall("noOfChildren",cocos2d::StringUtils::format("%d", noOfChildren));
}

void mixPanel_registerAzoomeeEmail(std::string emailAddress)
{
    std::string azoomeEmail = "NO";
    
    if (hasAzoomeeEmailAddress(emailAddress))
        azoomeEmail = "YES";
    
    mixPanel_OSSpecificSuperPropertiesCall("azoomeeEmail",azoomeEmail);
}

void mixPanel_registerAccountStatus(std::string Status)
{
    mixPanel_OSSpecificSuperPropertiesCall("accountStatus",Status);
}

void mixPanel_registerChildID(std::string ChildID)
{
    mixPanel_OSSpecificSuperPropertiesCall("childID",ChildID);
}

void mixPanel_registerChildGender(std::string ChildGender)
{
    mixPanel_OSSpecificSuperPropertiesCall("sex",ChildGender);
}

void mixPanel_registerChildDOB(std::string ChildDOB)
{
    mixPanel_OSSpecificSuperPropertiesCall("dob",ChildDOB);
}

void mixPanel_logoutChild()
{
    mixPanel_OSSpecificSuperPropertiesCall("childID","");
    mixPanel_OSSpecificSuperPropertiesCall("sex","");
    mixPanel_OSSpecificSuperPropertiesCall("dob","");
}

void mixPanel_logoutParent()
{
    mixPanel_logoutChild();
    mixPanel_OSSpecificSuperPropertiesCall("accountStatus","");
    mixPanel_OSSpecificSuperPropertiesCall("parentID","");
    mixPanel_OSSpecificSuperPropertiesCall("azoomeeEmail","");
}

//-------------Startup--------------------

void mixPanel_fistLaunchEvent()
{
    createOSSpecficCall("firstLaunch");
}

// -------------- SIGN IN FUNCTIONS -----------------
void mixPanel_signInSuccessEvent()
{
    createOSSpecficCall("parentAppLoginSuccess");
}

void mixPanel_signInFailEvent(int errorCode)
{
    createOSSpecficCall("parentAppLoginFailure");
}

//-------------ONBOARDING--------------------
void mixPanel_OnboardingStartEvent()
{
    createOSSpecficCall("startCreateAccount");
}

void mixPanel_OnboardingEmailSubmittedEvent()
{
    createOSSpecficCall("emailSubmitted");
}

void mixPanel_OnboardingPasswordSubmittedEvent()
{
    createOSSpecficCall("passwordSubmitted");
}

void mixPanel_OnboardingPinSubmittedEvent()
{
    createOSSpecficCall("pinSubmitted");
}

void mixPanel_OnboardingAccountCreatedEvent()
{
    std::string eventID = "accountCreated";
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    std::map<std::string, std::string> mixPanelProperties;
    
    mixPanelProperties["Method"] = "App";
    
    mixPanelSendiOSEvent(mixPanelProperties, eventID);
    
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    mixPanel_androidJNIHelper(eventID,"{\"Method\":\"App\"}");
    
#endif
}

void mixPanel_OnboardingAccountCreatedErrorEvent(long errorCode)
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
void mixPanel_childProfileStartEvent()
{
    createOSSpecficCall("childProfileCreateStart");
}

void mixPanel_childProfileNameEvent()
{
    createOSSpecficCall("childProfileName");
}

void mixPanel_childProdileNameErrorEvent()
{
    createOSSpecficCall("childProfileNameError");
}

void mixPanel_childProfileDOBEvent()
{
    createOSSpecficCall("childProfileDOB");
}

void mixPanel_childProfileDOBErrorEvent()
{
    //There are no errors defined at present for this function.
    //createOSSpecficCall("childProfileDOBError");
}

void mixPanel_childProfileOomeeEvent(int oomeeNumber)
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

void mixPanel_childProfileCreatedSuccessEvent(int oomeeNumber)
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

void mixPanel_childProfileCreatedErrorEvent(long errorCode)
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
void mixPanel_hubTapOomee(int oomeeNumber, std::string oomeeAction)
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

void mixPanel_navSelectionEvent(std::string hubOrTop, int buttonNumber)
{
    std::string eventID = "contentNavSelection";
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    std::map<std::string, std::string> mixPanelProperties;
    
    mixPanelProperties["ErrorCode"] = ConfigStorage::getInstance()->getNameForMenuItem(buttonNumber);
    
    mixPanelSendiOSEvent(mixPanelProperties, eventID);
    
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    mixPanel_androidJNIHelper(eventID,cocos2d::StringUtils::format("{\"ErrorCode\":\"%s\"}", ConfigStorage::getInstance()->getNameForMenuItem(buttonNumber).c_str()));
    
#endif
}

//------------- PREVIEW ACTIONS ---------------
void mixPanel_previewContentClickedEvent(std::string Title, std::string Description, std::string Type)
{
    
}

void mixPanel_previewPopupCancelledEvent()
{
    createOSSpecficCall("previewEmailPopUpDismissed");
}



/*void mixPanel_signInSuccessEvent()
 {
 std::map<std::string, std::string> mixPanelProperties;
 
 mixPanelProperties["Method"] = "App";
 
 #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
 
 mixPanelSendiOSEvent(mixPanelProperties, "CliveMP_TEST_IOS");
 
 #elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
 
 cocos2d::JniMethodInfo methodInfo;
 
 if (! cocos2d::JniHelper::getStaticMethodInfo(methodInfo, "org/cocos2dx/cpp/AppActivity", "sendMixPanelWithEventID", "(Ljava/lang/String;Ljava/lang/String;)V"))
 {
 return;
 }
 
 jstring jstringJSONProperties= methodInfo.env->NewStringUTF("{\"KEY\":\"Property\"}");
 jstring jstringEventID= methodInfo.env->NewStringUTF("CliveMP_TEST_ANDROID");
 
 CCLOG("Mix Panel Android Text");
 
 methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, jstringEventID,jstringJSONProperties);
 
 methodInfo.env->DeleteLocalRef(methodInfo.classID);
 
 #endif
 }*/
