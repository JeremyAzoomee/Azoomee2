#include "DeepLinkingSingleton.h"
#include <AzoomeeCommon/Utils/StringFunctions.h>
#include "BackEndCaller.h"
#include "WebViewSelector.h"
#include "GameDataManager.h"
#include "NavigationLayer.h"
#include "HQDataProvider.h"
#include "HQHistoryManager.h"
#include <AzoomeeCommon/Data/Child/ChildDataProvider.h>
#include <AzoomeeCommon/Data/Parent/ParentDataParser.h>
#include "IAPUpsaleLayer.h"
#include "OnboardingScene.h"
#include <AzoomeeCommon/UI/ModalMessages.h>

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
    deepLinkActionWaiting = setHostandPath(UriString);
    
    if(Director::getInstance()->getRunningScene())
        actionDeepLink();
}

void DeepLinkingSingleton::resetDeepLink()
{
    deepLinkActionWaiting = false;
    Host = "";
    Path = "";
}

bool DeepLinkingSingleton::setHostandPath(std::string UriString)
{
    std::vector<std::string> SplitByAzoomeVector = splitStringToVector(stringToLower(UriString), "azoomee://");
    
    if(SplitByAzoomeVector.size() == 0 || SplitByAzoomeVector.size() > 2)
        return false;
    
    std::string UriStringWhole = SplitByAzoomeVector.at(0);
    
    if(SplitByAzoomeVector.size() == 2)
        std::string UriStringWhole = SplitByAzoomeVector.at(1);
    
    std::vector<std::string>  SplitByForwardSlash = splitStringToVector(UriStringWhole, "/");
    
    if(SplitByForwardSlash.size() != 2)
        return false;
    
    Host = SplitByForwardSlash.at(0);
    Path = SplitByForwardSlash.at(1);
    
    return true;
}

bool DeepLinkingSingleton::actionDeepLink()
{
    if(Host == "" | Path == "" | !deepLinkActionWaiting)
        return false;
    
    if(Host == "content" && ChildDataProvider::getInstance()->getIsChildLoggedIn())
    {
        ModalMessages::getInstance()->startLoading();
        deepLinkActionWaiting = false;
        BackEndCaller::getInstance()->getContentItemDetails("deepLinkContentRequest", Path);
        return true;
    }
    else if(Host == "moveto")
    {
        if(Path == "signup" && !ChildDataProvider::getInstance()->getIsChildLoggedIn() && !ParentDataParser::getInstance()->hasParentLoginDataInUserDefaults())
        {
            auto onboardingScene = OnboardingScene::createScene(0);
            Director::getInstance()->replaceScene(onboardingScene);
            resetDeepLink();
            return true;
        }
    }
    
    return false;
}

void DeepLinkingSingleton::contentDetailsResponse(std::string responseBody)
{
    rapidjson::Document contentData;
    contentData.Parse(responseBody.c_str());
    
    //ERROR CHECK RESPONSE
    if (contentData.HasParseError()) return;
    if(!contentData.HasMember("type") || !contentData.HasMember("uri") || !contentData.HasMember("entitled")) return;
    if(contentData["type"].IsNull() || contentData["uri"].IsNull() || contentData["entitled"].IsNull()) return;
    
    if(contentData["entitled"].GetBool())
        completeContentAction(contentData["type"].GetString(), contentData["uri"].GetString());
    else
    {
        ModalMessages::getInstance()->stopLoading();
        resetDeepLink();
        MessageBox::createWith(ERROR_CODE_DEEPLINK_NOT_ENTITLED, nullptr);
    }
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
        
        ModalMessages::getInstance()->stopLoading();

        HQDataProvider::getInstance()->getDataForGroupHQ(uri);
        HQHistoryManager::getInstance()->setGroupHQSourceId(Path);
        
        auto funcCallAction = CallFunc::create([=](){
            HQDataProvider::getInstance()->getDataForGroupHQ(uri);
            HQHistoryManager::getInstance()->setGroupHQSourceId(Path);
        });
        
        Director::getInstance()->getRunningScene()->runAction(Sequence::create(DelayTime::create(0.5), funcCallAction, NULL));
    }
    
    resetDeepLink();
}

//Delegate Functions
void DeepLinkingSingleton::MessageBoxButtonPressed(std::string messageBoxTitle,std::string buttonTitle)
{
    IAPUpsaleLayer::create();
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
