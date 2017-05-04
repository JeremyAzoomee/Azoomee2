#include "DeepLinkingSingleton.h"
#include <AzoomeeCommon/Utils/StringFunctions.h>

using namespace Azoomee;
    
static DeepLinkingSingleton *_sharedDeepLinkingSingleton = NULL;

DeepLinkingSingleton* DeepLinkingSingleton::getInstance()
{
    if (! _sharedDeepLinkingSingleton)
    {
        _sharedDeepLinkingSingleton = new DeepLinkingSingleton();
        _sharedDeepLinkingSingleton->init();
    }
    
    return _sharedDeepLinkingSingleton;
}

DeepLinkingSingleton::~DeepLinkingSingleton(void)
{
}

bool DeepLinkingSingleton::init(void)
{
    return true;
}

void DeepLinkingSingleton::setDeepLink(std::string UriString)
{
    CCLOG("DEEPLINK URI:%s",UriString.c_str());
    
    setHostandPath(UriString);
    
    

}

void DeepLinkingSingleton::resetDeepLink()
{
    Host = "";
    Path = "";
}

void DeepLinkingSingleton::setHostandPath(std::string UriString)
{
    std::vector<std::string> SplitByAzoomeVector = splitStringToVector(stringToLower(UriString), "azoomee://");
    
    if(SplitByAzoomeVector.size() == 0 || SplitByAzoomeVector.size() > 2)
    {
        resetDeepLink();
        return;
    }
    
    std::string UriStringWhole = SplitByAzoomeVector.at(0);
    
    if(SplitByAzoomeVector.size() == 2)
        std::string UriStringWhole = SplitByAzoomeVector.at(1);
    
    std::vector<std::string>  SplitByForwardSlash = splitStringToVector(UriStringWhole, "/");
    
    if(SplitByForwardSlash.size() != 2)
    {
        resetDeepLink();
        return;
    }
    
    Host = SplitByForwardSlash.at(0);
    Path = SplitByForwardSlash.at(1);
}

void DeepLinkingSingleton::actionDeepLink()
{
    if(Host == "" | Path == "")
        return;
    
    if(Host == "content")
    {
        //backendcaller to get the latest content details based on path.
    }
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

extern "C"
{
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_DeepLink_sendDeepLinkToCPP(JNIEnv* env, jobject thiz, jstring URIString);
};

JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_DeepLink_sendDeepLinkToCPP(JNIEnv* env, jobject thiz, jstring URIString)
{
    using namespace Azoomee;
    #include "DeepLinkingSingleton.h"
    
    const char* cURIString = env->GetStringUTFChars(URIString, NULL);
    
    DeepLinkingSingleton::getInstance()->setDeepLink(std::string(cURIString));
    
}

#endif
