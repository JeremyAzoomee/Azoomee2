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
#include <AzoomeeCommon/Data/Child/ChildDataProvider.h>
#include <AzoomeeCommon/Utils/StringFunctions.h>

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
        saveContentToLastPlayedList(contentItem->getContentItemId());
        ContentHistoryManager::getInstance()->setLastOppenedContent(contentItem);
        GameDataManager::getInstance()->startProcessingGame(contentItem);
    }
    else if(contentItem->getType()  == ConfigStorage::kContentTypeVideo || contentItem->getType()  == ConfigStorage::kContentTypeAudio)
    {
        saveContentToLastPlayedList(contentItem->getContentItemId());
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

void ContentOpener::saveContentToLastPlayedList(const std::string& contentId)
{
    const std::string& recentlyPlayedFolderLoc = FileUtils::getInstance()->getWritablePath() + "RecentlyPlayed/";
    if(!FileUtils::getInstance()->isDirectoryExist(recentlyPlayedFolderLoc))
    {
        FileUtils::getInstance()->createDirectory(recentlyPlayedFolderLoc);
    }
    const std::string& childRecentlyPlayedFolderLoc = recentlyPlayedFolderLoc + ChildDataProvider::getInstance()->getLoggedInChildId();
    if(!FileUtils::getInstance()->isDirectoryExist(childRecentlyPlayedFolderLoc))
    {
        FileUtils::getInstance()->createDirectory(childRecentlyPlayedFolderLoc);
    }
    const std::string& recentlyPlayedContentFile = childRecentlyPlayedFolderLoc + "/recentContent.txt";
    
    const std::string& fileStr = FileUtils::getInstance()->getStringFromFile(recentlyPlayedContentFile);
    
    std::vector<std::string> fileIds = splitStringToVector(fileStr, "/");
    
    auto pivot = std::find_if(fileIds.begin(), fileIds.end(), [&](const std::string& id){return id == contentId;});
    if(pivot != fileIds.end())
    {
        std::rotate(fileIds.begin(), pivot, pivot + 1);
    }
    else
    {
        fileIds.insert(fileIds.begin(),contentId);
    }
    
    while(fileIds.size() > 8)
    {
        fileIds.pop_back();
    }
    
    std::string newIdList = "";
    for(int i = 0; i < fileIds.size(); i++)
    {
        newIdList += fileIds[i];
        if(i < fileIds.size() -1)
        {
            newIdList += "/";
        }
    }
    
    FileUtils::getInstance()->writeStringToFile(newIdList, recentlyPlayedContentFile);
    
}


NS_AZOOMEE_END
