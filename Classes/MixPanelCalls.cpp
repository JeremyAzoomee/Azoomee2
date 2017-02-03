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
    
    mixPanelSendiOSEvent(mixPanelProperties, "CliveMP_TEST");
    
    #elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        
        
    #endif
}

void mixPanel_signInFailEvent(int errorCode)
{
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        
        
    #elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        
        
    #endif
    
}
