#include "DeepLinkingSingleton.h"
#include <AzoomeeCommon/Utils/StringFunctions.h>
#include "BackEndCaller.h"
#include "WebViewSelector.h"
#include "GameDataManager.h"
#include "NavigationLayer.h"
#include "HQDataProvider.h"
#include "HQHistoryManager.h"
#include <AzoomeeCommon/Data/Child/ChildDataProvider.h>

USING_NS_CC;
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
    resetDeepLink();
    return true;
}

void DeepLinkingSingleton::setDeepLink(std::string UriString)
{
    CCLOG("DEEPLINK URI:%s",UriString.c_str());
    
    resetDeepLink();
    setHostandPath(UriString);
    actionDeepLink();
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
    
    if(Host == "content" && ChildDataProvider::getInstance()->getIsChildLoggedIn())
        BackEndCaller::getInstance()->getContentItemDetails("deepLinkContentRequest", Path);
}

void DeepLinkingSingleton::contentDetailsResponse(std::string responseBody)
{
    rapidjson::Document contentData;
    contentData.Parse(responseBody.c_str());
    
    //ERROR CHECK RESPONSE
    if (contentData.HasParseError()) return;
    if(!contentData.HasMember("type") || !contentData.HasMember("uri") || !contentData.HasMember("entitled")) return;
    if(contentData["type"].IsNull() || contentData["uri"].IsNull() || contentData["entitled"].IsNull()) return;
    
    if(contentData["entitled"].GetBool());
        completeContentAction(contentData["type"].GetString(), contentData["uri"].GetString());
    
}

void DeepLinkingSingleton::completeContentAction(std::string type,std::string uri)
{
    if(type == "GAME")
    {
        GameDataManager::getInstance()->startProcessingGame(uri, Path);
    }
    else if(type == "VIDEO" || type == "AUDIO")
    {
        auto webViewSelector = WebViewSelector::create();
        webViewSelector->loadWebView(uri.c_str());
    }
    else if(type == "AUDIOGROUP")
    {
        NavigationLayer *navigationLayer = (NavigationLayer *)Director::getInstance()->getRunningScene()->getChildByName("baseLayer")->getChildByName("NavigationLayer");
        navigationLayer->startLoadingGroupHQ(uri);
        
        HQDataProvider::getInstance()->getDataForGroupHQ(uri);
        HQHistoryManager::getInstance()->setGroupHQSourceId(Path);

    }
    else if(type == "GROUP")
    {
        NavigationLayer *navigationLayer = (NavigationLayer *)Director::getInstance()->getRunningScene()->getChildByName("baseLayer")->getChildByName("NavigationLayer");
        navigationLayer->startLoadingGroupHQ(uri);

        HQDataProvider::getInstance()->getDataForGroupHQ(uri);
        HQHistoryManager::getInstance()->setGroupHQSourceId(Path);

    }
    
    resetDeepLink();
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
