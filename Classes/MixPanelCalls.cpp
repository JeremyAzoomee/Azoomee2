#include "MixPanelCalls.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    #include "MixPanel_ios.h"
#endif


// -------------- SIGN IN FUNCTIONS -----------------
void mixPanel_signInSuccessEvent()
{
    std::map<std::string, std::string> mixPanelProperties;
    
    mixPanelProperties["Method"] = "App";
    
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    
    mixPanelSendiOSEvent(mixPanelProperties, "CliveMP_TEST_IOS");
    
    #elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    cocos2d::JniMethodInfo t;
    
    // we need env to do things so get it from here. (cocos2d-x does not export getEnv function)
    if (cocos2d::JniHelper::getStaticMethodInfo(t, "org/cocos2dx/cpp/AppActivity", "sendMixPanelWithEventID", "(Ljava/lang/String;Lorg/cocos2dx/cpp/mixPanel;)V"))
    {
        // get class id of mixPanel class
        jclass j_cMixPanel = t.env->FindClass("org/cocos2dx/cpp/mixPanel");
        
        //  is special for constructor
        jmethodID j_mMixPanel = t.env->GetMethodID(j_cMixPanel, "", "(IILjava/lang/String;)V");
        
        // create a jstring from c-string
        jstring j_sEventID = t.env->NewStringUTF("CliveMP_TEST_ANDROID");
        
        // create a MyVector object
        jobject j_oMixPanel = t.env->NewObject(j_cMixPanel, j_mMixPanel);
        
        jmethodID jSendPropertiesMethod = t.env->GetMethodID(j_cMixPanel, "addToMixPanelProperties", "(Ljava/lang/String;Ljava/lang/String;)V");
        
        t.env->CallStaticVoidMethod(j_cMixPanel, jSendPropertiesMethod, "Key", "detail");
        
        // now we can send
        t.env->CallStaticVoidMethod(t.classID, t.methodID, j_sEventID, j_oMixPanel);
        
        // remember to remove objects we allocated
        t.env->DeleteLocalRef(j_sMessage);
        t.env->DeleteLocalRef(j_oMyVector);
        
        t.env->DeleteLocalRef(j_cMyVector);
        t.env->DeleteLocalRef(t.classID);
        
    #endif
}

void mixPanel_signInFailEvent(int errorCode)
{
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        
        
    #elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        
        
    #endif
    
}
