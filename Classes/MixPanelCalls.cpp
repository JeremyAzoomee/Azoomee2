#include "MixPanelCalls.h"
#include "StringStorage.h"


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

//-------------- SUPER PROPERTIES

//When capturing account status, if possible capture date created, type etc.
void mixPanel_registerSuperProperties(std::string ParentID, std::string AccountStatus, std::string ChildID, std::string ChildGender,std::string ChildDob)
{
    std::string KEY_ParentID = "parentID";
    std::string KEY_AccountStatus = "accountStatus";
    std::string KEY_ChildID = "childID";
    std::string KEY_ChildGender = "sex";
    std::string KEY_ChildDOB = "dob";
    std::string KEY_SWVersion = "appVersion";
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    
    std::map<std::string, std::string> mixPanelProperties;
    
    mixPanelProperties[KEY_ParentID] = ParentID;
    mixPanelProperties[KEY_AccountStatus] = AccountStatus;
    mixPanelProperties[KEY_ChildID] = ChildID;
    mixPanelProperties[KEY_ChildGender] = ChildGender;
    mixPanelProperties[KEY_ChildDOB] = ChildDob;
    mixPanelProperties[KEY_SWVersion] = APP_VERSION_NUMBER;
    
    mixPanel_registerSuperProperties_iOS(mixPanelProperties);
    
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    cocos2d::JniMethodInfo methodInfo;
    
    if (! cocos2d::JniHelper::getStaticMethodInfo(methodInfo, "org/cocos2dx/cpp/AppActivity", "sendMixPanelSuperProperties", "(Ljava/lang/String;)V"))
    {
        return;
    }
    
   std::string propertiesJSONString = cocos2d::StringUtils::format("{\"%s\":\"%s\",\"%s\":\"%s\",\"%s\":\"%s\",\"%s\":\"%s\",\"%s\":\"%s\",\"%s\":\"%s\"}", KEY_ParentID.c_str(),ParentID.c_str(),KEY_AccountStatus.c_str(),AccountStatus.c_str(),KEY_ChildID.c_str(),ChildID.c_str(),KEY_ChildGender.c_str(),ChildGender.c_str(),KEY_ChildDOB.c_str(),ChildDob.c_str(),KEY_SWVersion.c_str(),APP_VERSION_NUMBER.c_str());
    
    jstring jstringJSONProperties= methodInfo.env->NewStringUTF(propertiesJSONString.c_str());
    
    methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID,jstringJSONProperties);
    
    methodInfo.env->DeleteLocalRef(methodInfo.classID);
    
#endif
}

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

void mixPanel_registerParentID(std::string ParentID)
{
    mixPanel_OSSpecificSuperPropertiesCall("ParentID",ParentID);
}

void mixPanel_registerChildID(std::string ChildID)
{
    mixPanel_OSSpecificSuperPropertiesCall("ChildID",ChildID);
}

void mixPanel_registerAzoomeeEmail(std::string emailAddress)
{
    if (emailAddress.find("azoomee") != std::string::npos) {
        //.. found.
    }
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
void mixPanel_emailSubmittedEvent()
{
    createOSSpecficCall("emailSubmitted");
}

void mixPanel_passwordSubmittedEvent()
{
    createOSSpecficCall("passwordSubmitted");
}

void mixPanel_pinSubmittedEvent()
{
    createOSSpecficCall("pinSubmitted");
}

void mixPanel_accountCreatedEvent()
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

void mixPanel_accountCreatedErrorEvent(long errorCode)
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
    createOSSpecficCall("childProfileNameError");
}

void mixPanel_childProfileDOBErrorEvent()
{
    //There are no errors defined at present for this function.
}

void mixPanel_childProfileOomeeEvent()
{
    createOSSpecficCall("childProfileNameError");
}

void mixPanel_childProfileCreatedSuccessEvent()
{
    std::string eventID = "childProfileCreatedSuccess";
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    std::map<std::string, std::string> mixPanelProperties;
    
    mixPanelProperties["Method"] = "App";
    
    mixPanelSendiOSEvent(mixPanelProperties, eventID);
    
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    mixPanel_androidJNIHelper(eventID,"{\"Method\":\"App\"}");
    
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
