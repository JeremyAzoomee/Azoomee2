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
#include "VideoPlaylistManager.h"

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

void DeepLinkingSingleton::setDeepLink(std::string uriString)
{
    CCLOG("DEEPLINK URI:%s",uriString.c_str());
    
    resetDeepLink();
    deepLinkActionWaiting = setHostAndPath(uriString);
    
    if(Director::getInstance()->getRunningScene())
        actionDeepLink();
}

void DeepLinkingSingleton::resetDeepLink()
{
    deepLinkActionWaiting = false;
    host = "";
    path = "";
}

bool DeepLinkingSingleton::setHostAndPath(std::string uriString)
{
    std::vector<std::string> splitByAzoomeVector = splitStringToVector(stringToLower(uriString), "azoomee://");
    
    if(splitByAzoomeVector.size() == 0 || splitByAzoomeVector.size() > 2)
        return false;
    
    std::string uriStringWhole = splitByAzoomeVector.at(0);
    
    if(splitByAzoomeVector.size() == 2)
        uriStringWhole = splitByAzoomeVector.at(1);
    
    std::vector<std::string>  splitByForwardSlash = splitStringToVector(uriStringWhole, "/");
    
    if(splitByForwardSlash.size() != 2)
        return false;
    
    host = splitByForwardSlash.at(0);
    path = splitByForwardSlash.at(1);
    
    return true;
}

bool DeepLinkingSingleton::actionDeepLink()
{
    if(host == "" || path == "" || !deepLinkActionWaiting)
        return false;
    
    if(host == "content" && ChildDataProvider::getInstance()->getIsChildLoggedIn())
    {
        ModalMessages::getInstance()->startLoading();
        deepLinkActionWaiting = false;
        BackEndCaller::getInstance()->getElectricDreamsContent("deepLinkContentRequest", path);
        return true;
    }
    else if(host == "moveto")
    {
        if(path == "signup" && !ChildDataProvider::getInstance()->getIsChildLoggedIn() && !ParentDataParser::getInstance()->hasParentLoginDataInUserDefaults())
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
        MessageBox::createWith(ERROR_CODE_DEEPLINK_NOT_ENTITLED, this);
    }
}

void DeepLinkingSingleton::completeContentAction(std::string type,std::string uri)
{
    if(type == "GAME")
    {
        GameDataManager::getInstance()->startProcessingGame(uri, path);
    }
    else if(type == "VIDEO" || type == "AUDIO")
    {
        VideoPlaylistManager::getInstance()->setPlaylist("");
        auto webViewSelector = WebViewSelector::create();
        webViewSelector->loadWebView(uri.c_str());
    }
    else if(type == "AUDIOGROUP" || type == "GROUP")
    {
        ModalMessages::getInstance()->stopLoading();
        
        auto baseLayer = Director::getInstance()->getRunningScene()->getChildByName("baseLayer");
        if(baseLayer)
        {
            NavigationLayer *navigationLayer = (NavigationLayer *)baseLayer->getChildByName("NavigationLayer");
            
            if(navigationLayer)
            {
                navigationLayer->startLoadingGroupHQ(uri);

                HQDataProvider::getInstance()->getDataForGroupHQ(uri);
                HQHistoryManager::getInstance()->setGroupHQSourceId(path);
                
                auto funcCallAction = CallFunc::create([=](){
                    HQDataProvider::getInstance()->getDataForGroupHQ(uri);
                    HQHistoryManager::getInstance()->setGroupHQSourceId(path);
                });
                
                Director::getInstance()->getRunningScene()->runAction(Sequence::create(DelayTime::create(0.5), funcCallAction, NULL));
            }
        }
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
    //using namespace Azoomee;
    //#include "DeepLinkingSingleton.h"
    
    const char* cURIString = env->GetStringUTFChars(URIString, NULL);
    
    DeepLinkingSingleton::getInstance()->setDeepLink(std::string(cURIString));
    
}

#endif
