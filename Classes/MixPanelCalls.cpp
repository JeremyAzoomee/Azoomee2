#include "MixPanelCalls.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    #include "MixPanel_ios.h"
#endif

void mixPanel_androidJNIHelper(std::string eventID, std::string propertiesJSONString)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    cocos2d::JniMethodInfo methodInfo;
    
    if (! cocos2d::JniHelper::getStaticMethodInfo(methodInfo, "org/cocos2dx/cpp/AppActivity", "sendMixPanelWithEventID", "(Ljava/lang/String;Ljava/lang/String;)V"))
    {
        return;
    }
    
    jstring jstringEventID= methodInfo.env->NewStringUTF(eventID);
    jstring jstringJSONProperties= methodInfo.env->NewStringUTF(propertiesJSONString);
    
    methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, jstringEventID,jstringJSONProperties);
    
    methodInfo.env->DeleteLocalRef(methodInfo.classID);
    
#endif
}

void mixPanel_fistLaunchEvent()
{
    std::string eventID = "firstLaunch";
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    
    mixPanelSendiOSEvent(eventID);
    
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    mixPanel_androidJNIHelper(eventID,"");
    
#endif
}

// -------------- SIGN IN FUNCTIONS -----------------
void mixPanel_signInSuccessEvent()
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
}

void mixPanel_signInFailEvent(int errorCode)
{
    std::string eventID = "signInSuccess";
    
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        std::map<std::string, std::string> mixPanelProperties;
        
        mixPanelProperties["Method"] = "App";
        mixPanelSendiOSEvent(mixPanelProperties, eventID);
    
    #elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        
        
    #endif
    
}
