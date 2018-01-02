#include "DeepLinkingSingleton.h"
#include <AzoomeeCommon/Utils/StringFunctions.h>
#include "BackEndCaller.h"
#include "WebViewSelector.h"
#include "GameDataManager.h"
#include "HQDataProvider.h"
#include "HQHistoryManager.h"
#include <AzoomeeCommon/Data/Child/ChildDataProvider.h>
#include <AzoomeeCommon/Data/Parent/ParentDataParser.h>
#include "IAPUpsaleLayer.h"
#include <AzoomeeCommon/UI/ModalMessages.h>
#include "VideoPlaylistManager.h"
#include "SceneManagerScene.h"
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include "ContentHistoryManager.h"
#include <AzoomeeCommon/Data/Json.h>

using namespace cocos2d;
using namespace Azoomee;

NS_AZOOMEE_BEGIN

const std::string DeepLinkingSingleton::kPostContentDeeplinkStr = "azoomee://post-content/";

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

void DeepLinkingSingleton::setDeepLink(const std::string& uriString)
{
    CCLOG("DEEPLINK URI:%s",uriString.c_str());
    
    resetDeepLink();
    deepLinkActionWaiting = setHostAndPath(uriString);
    
    if(Director::getInstance()->getRunningScene())
    {
        actionDeepLink();
    }
}

void DeepLinkingSingleton::resetDeepLink()
{
    deepLinkActionWaiting = false;
    host = "";
    path = "";
}

bool DeepLinkingSingleton::setHostAndPath(const std::string& uriString)
{
    const std::vector<std::string>& splitByAzoomeVector = splitStringToVector(stringToLower(uriString), "azoomee://");
    
    if(splitByAzoomeVector.size() == 0 || splitByAzoomeVector.size() > 2)
    {
        return false;
    }
    
    std::string uriStringWhole = splitByAzoomeVector.at(0);
    
    if(splitByAzoomeVector.size() == 2)
    {
        uriStringWhole = splitByAzoomeVector.at(1);
    }
    
    const std::vector<std::string>&  splitByForwardSlash = splitStringToVector(uriStringWhole, "/");
    
    if(splitByForwardSlash.size() != 2)
    {
        return false;
    }
    
    host = splitByForwardSlash.at(0);
    path = splitByForwardSlash.at(1);
    
    AnalyticsSingleton::getInstance()->deepLinkingDetailsSetEvent();
    
    return true;
}

bool DeepLinkingSingleton::actionDeepLink()
{
    if(host == "" || path == "" || !deepLinkActionWaiting)
    {
        return false;
    }
    
    if(host == "content" && ChildDataProvider::getInstance()->getIsChildLoggedIn())
    {
        AnalyticsSingleton::getInstance()->deepLinkingContentEvent();
        
        ModalMessages::getInstance()->startLoading();
        deepLinkActionWaiting = false;
        BackEndCaller::getInstance()->getElectricDreamsContent("deepLinkContentRequest", path);
        return true;
    }
    else if(host == "post-content")
    {
        const HQContentItemObjectRef& item = HQDataProvider::getInstance()->getItemDataForSpecificItem(path);
        if(item)
        {
            AnalyticsSingleton::getInstance()->contentItemSelectedEvent(item, -1, -1, "0,0");
            completeContentAction(item);
        }
        
    }
    else if(host == "moveto")
    {
        if(path == "signup" && !ChildDataProvider::getInstance()->getIsChildLoggedIn() && !ParentDataParser::getInstance()->hasParentLoginDataInUserDefaults())
        {
            AnalyticsSingleton::getInstance()->deepLinkingMoveToEvent(path);
            
            Director::getInstance()->replaceScene(SceneManagerScene::createScene(Onboarding));
            
            resetDeepLink();
            return true;
        }
        
        if(!ChildDataProvider::getInstance()->getIsChildLoggedIn())
        {
            return false;
        }
        
        if(path == "chat")
        {
            AnalyticsSingleton::getInstance()->deepLinkingMoveToEvent(path);
            
            Director::getInstance()->replaceScene(SceneManagerScene::createScene(ChatEntryPointScene));
            
            resetDeepLink();
            return true;
        }
        if(path == "artstudio")
        {
            AnalyticsSingleton::getInstance()->deepLinkingMoveToEvent(path);
            
            Director::getInstance()->replaceScene(SceneManagerScene::createScene(ArtAppEntryPointScene));
            
            resetDeepLink();
            return true;
        }
        if(path == "games")
        {
            moveToHQ(ConfigStorage::HubTargetTagNumber::GAME_HQ);
            return true;
        }
        if(path == "audio")
        {
            moveToHQ(ConfigStorage::HubTargetTagNumber::AUDIO_HQ);
            return true;
        }
        if(path == "videos")
        {
            moveToHQ(ConfigStorage::HubTargetTagNumber::VIDEO_HQ);
            return true;
        }
    }
    
    return false;
}

void DeepLinkingSingleton::moveToHQ(ConfigStorage::HubTargetTagNumber hqName)
{
    AnalyticsSingleton::getInstance()->deepLinkingMoveToEvent(path);
    
    auto baseLayer = Director::getInstance()->getRunningScene()->getChildByName("baseLayer");
    if(baseLayer)
    {
        NavigationLayer *navigationLayer = (NavigationLayer *)baseLayer->getChildByName("NavigationLayer");
        
        if(navigationLayer)
        {
            navigationLayer->changeToScene(hqName, 0.1);
        }
    }
    
    resetDeepLink();
}

void DeepLinkingSingleton::contentDetailsResponse(const std::string& responseBody)
{
    rapidjson::Document contentData;
    contentData.Parse(responseBody.c_str());
    
    cocos2d::log("DATA RESPONSE: %s", responseBody.c_str());
    
    //ERROR CHECK RESPONSE
    if (contentData.HasParseError())
    {
        return;
    }
    
    if(contentData["entitled"].GetBool())
    {
        HQContentItemObjectRef contentItem = HQContentItemObject::create();
        contentItem->setTitle(getStringFromJson("title", contentData));
        contentItem->setDescription(getStringFromJson("description", contentData));
        contentItem->setType(getStringFromJson("type", contentData));
        contentItem->setUri(getStringFromJson("uri", contentData));
        contentItem->setContentItemId(path);
        
        AnalyticsSingleton::getInstance()->contentItemSelectedEvent(contentItem, -1, -1, "0,0");
        
        completeContentAction(contentItem);
    }
    else
    {
        ModalMessages::getInstance()->stopLoading();
        resetDeepLink();
        MessageBox::createWith(ERROR_CODE_DEEPLINK_NOT_ENTITLED, this);
    }
}

std::string DeepLinkingSingleton::getDataForKeyFromJSON(const std::string& jsonString, const std::string& key)
{
    rapidjson::Document jsonData;
    jsonData.Parse(jsonString.c_str());
    
    return getStringFromJson(key, jsonData);
}

void DeepLinkingSingleton::completeContentAction(const HQContentItemObjectRef &contentItem)
{
    if(!contentItem->isEntitled())
    {
        return;
    }
    
    if(contentItem->getType() == ConfigStorage::kContentTypeGame)
    {
        ContentHistoryManager::getInstance()->setLastOppenedContent(contentItem);
        GameDataManager::getInstance()->startProcessingGame(contentItem);
    }
    else if(contentItem->getType()  == ConfigStorage::kContentTypeVideo || contentItem->getType()  == ConfigStorage::kContentTypeAudio)
    {
        ContentHistoryManager::getInstance()->setLastOppenedContent(contentItem);
        VideoPlaylistManager::getInstance()->clearPlaylist();
        auto webViewSelector = WebViewSelector::create();
        webViewSelector->loadWebView(contentItem->getUri(),Orientation::Landscape);
    }
    else if(contentItem->getType()  == ConfigStorage::kContentTypeAudioGroup || contentItem->getType()  == ConfigStorage::kContentTypeGroup)
    {
        ModalMessages::getInstance()->stopLoading();
        
        auto baseLayer = Director::getInstance()->getRunningScene()->getChildByName("baseLayer");
        if(baseLayer)
        {
            NavigationLayer *navigationLayer = (NavigationLayer *)baseLayer->getChildByName("NavigationLayer");
            
            if(navigationLayer)
            {
                navigationLayer->startLoadingGroupHQ(contentItem->getUri());

                HQDataProvider::getInstance()->getDataForGroupHQ(contentItem->getUri());
                HQHistoryManager::getInstance()->setGroupHQSourceId(path);
                
                auto funcCallAction = CallFunc::create([=](){
                    HQDataProvider::getInstance()->getDataForGroupHQ(contentItem->getUri());
                });
                
                Director::getInstance()->getRunningScene()->runAction(Sequence::create(DelayTime::create(0.5), funcCallAction, NULL));
            }
        }
    }
    
    resetDeepLink();
}

//Delegate Functions
void DeepLinkingSingleton::MessageBoxButtonPressed(std::string messageBoxTitle, std::string buttonTitle)
{
    IAPUpsaleLayer::createRequiresPin();
}

NS_AZOOMEE_END

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
