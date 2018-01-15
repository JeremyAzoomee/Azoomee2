//
//  ContentOpener.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 18/12/2017.
//

#include "ContentOpener.h"
#include "HQDataProvider.h"
#include "HQHistoryManager.h"
#include "ContentHistoryManager.h"
#include "GameDataManager.h"
#include "VideoPlaylistManager.h"
#include "WebViewSelector.h"
#include "NavigationLayer.h"
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include <AzoomeeCommon/UI/ModalMessages.h>


using namespace cocos2d;

NS_AZOOMEE_BEGIN

static std::auto_ptr<ContentOpener> sContentOpenerSharedInstance;

ContentOpener* ContentOpener::getInstance()
{
    if(!sContentOpenerSharedInstance.get())
    {
        sContentOpenerSharedInstance.reset(new ContentOpener());
    }
    return sContentOpenerSharedInstance.get();
}

ContentOpener::~ContentOpener(void)
{
    
}

bool ContentOpener::init(void)
{
    return true;
}

void ContentOpener::openContentById(const std::string &contentId)
{
    HQContentItemObjectRef contentItem = HQDataProvider::getInstance()->getItemDataForSpecificItem(contentId);
    
    if(contentItem)
    {
        openContentObject(contentItem);
    }
}

void ContentOpener::openContentObject(const HQContentItemObjectRef &contentItem)
{
    if(contentItem == nullptr || !contentItem->isEntitled())
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
                HQHistoryManager::getInstance()->setGroupHQSourceId(contentItem->getContentItemId());
                
                auto funcCallAction = CallFunc::create([=](){
                    HQDataProvider::getInstance()->getDataForGroupHQ(contentItem->getUri());
                });
                
                Director::getInstance()->getRunningScene()->runAction(Sequence::create(DelayTime::create(0.5), funcCallAction, NULL));
            }
        }
    }
}


NS_AZOOMEE_END
