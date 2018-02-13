//
//  RecentlyPlayedManager.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 25/01/2018.
//

#include "RecentlyPlayedManager.h"
#include <AzoomeeCommon/Data/Child/ChildDataProvider.h>
#include <AzoomeeCommon/Utils/StringFunctions.h>
#include "HQDataProvider.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

static std::auto_ptr<RecentlyPlayedManager> sRecentlyPlayedManagerSharedInstance;

RecentlyPlayedManager* RecentlyPlayedManager::getInstance()
{
    if (!sRecentlyPlayedManagerSharedInstance.get())
    {
        sRecentlyPlayedManagerSharedInstance.reset(new RecentlyPlayedManager());
    }
    
    return sRecentlyPlayedManagerSharedInstance.get();
}

RecentlyPlayedManager::~RecentlyPlayedManager(void)
{
}

void RecentlyPlayedManager::addContentIdToRecentlyPlayedFile(const std::string& contentId)
{
    std::vector<std::string> fileIds = getRecentContentIds();
    
    auto pivot = std::find(fileIds.begin(), fileIds.end(), contentId);
    if(pivot != fileIds.end())
    {
        std::rotate(fileIds.begin(), pivot, pivot + 1);
    }
    else
    {
        fileIds.insert(fileIds.begin(), contentId);
    }
    
    fileIds.resize(_kMaxRecentContent);
    
    const std::string& newIdList = joinStrings(fileIds, "/");
    
    FileUtils::getInstance()->writeStringToFile(newIdList, getRecentlyPlayedFilePath());
}

std::vector<HQContentItemObjectRef> RecentlyPlayedManager::getRecentlyPlayedContent() const
{
    std::vector<HQContentItemObjectRef> recentContent;
    
    const std::vector<std::string>& ids = getRecentContentIds();
    for(const std::string& id : ids)
    {
        HQContentItemObjectRef content = HQDataProvider::getInstance()->getItemDataForSpecificItem(id);
        if(content)
        {
            recentContent.push_back(content);
        }
    }
    
    return recentContent;
}

void RecentlyPlayedManager::addContentIdToRecentlyPlayedFileForHQ(const std::string &contentId, const std::string &hq)
{
    std::vector<std::string> fileIds = getRecentContentIdsForHQ(hq);
    
    auto pivot = std::find(fileIds.begin(), fileIds.end(), contentId);
    if(pivot != fileIds.end())
    {
        std::rotate(fileIds.begin(), pivot, pivot + 1);
    }
    else
    {
        fileIds.insert(fileIds.begin(), contentId);
    }
    
    fileIds.resize(_kMaxRecentContent);
    
    const std::string& newIdList = joinStrings(fileIds, "/");
    
    FileUtils::getInstance()->writeStringToFile(newIdList, getRecentlyPlayedFilePathForHQ(hq));
}

std::vector<HQContentItemObjectRef> RecentlyPlayedManager::getRecentlyPlayedContentForHQ(const std::string &hq) const
{
    std::vector<HQContentItemObjectRef> recentContent;
    
    const std::vector<std::string>& ids = getRecentContentIdsForHQ(hq);
    for(const std::string& id : ids)
    {
        HQContentItemObjectRef content = HQDataProvider::getInstance()->getItemDataForSpecificItem(id);
        if(content)
        {
            recentContent.push_back(content);
        }
    }
    
    return recentContent;
}

std::vector<std::string> RecentlyPlayedManager::getRecentContentIds() const
{
    const std::string& fileStr = FileUtils::getInstance()->getStringFromFile(getRecentlyPlayedFilePath());
    
    return splitStringToVector(fileStr, "/");
}

std::string RecentlyPlayedManager::getRecentlyPlayedFilePath() const
{
    const std::string& recentlyPlayedFolderLoc = FileUtils::getInstance()->getWritablePath() + _kRecentlyPlayedFolderName;
    if(!FileUtils::getInstance()->isDirectoryExist(recentlyPlayedFolderLoc))
    {
        FileUtils::getInstance()->createDirectory(recentlyPlayedFolderLoc);
    }
    const std::string& childRecentlyPlayedFolderLoc = recentlyPlayedFolderLoc + ChildDataProvider::getInstance()->getLoggedInChildId();
    if(!FileUtils::getInstance()->isDirectoryExist(childRecentlyPlayedFolderLoc))
    {
        FileUtils::getInstance()->createDirectory(childRecentlyPlayedFolderLoc);
    }
    
    return childRecentlyPlayedFolderLoc + "/" + _kRecentlyPlayedFileName;
}

std::vector<std::string> RecentlyPlayedManager::getRecentContentIdsForHQ(const std::string &hq) const
{
    const std::string& fileStr = FileUtils::getInstance()->getStringFromFile(getRecentlyPlayedFilePathForHQ(hq));
    
    return splitStringToVector(fileStr, "/");
}

std::string RecentlyPlayedManager::getRecentlyPlayedFilePathForHQ(const std::string &hq) const
{
    const std::string& recentlyPlayedFolderLoc = FileUtils::getInstance()->getWritablePath() + _kRecentlyPlayedFolderName;
    if(!FileUtils::getInstance()->isDirectoryExist(recentlyPlayedFolderLoc))
    {
        FileUtils::getInstance()->createDirectory(recentlyPlayedFolderLoc);
    }
    const std::string& childRecentlyPlayedFolderLoc = recentlyPlayedFolderLoc + ChildDataProvider::getInstance()->getLoggedInChildId();
    if(!FileUtils::getInstance()->isDirectoryExist(childRecentlyPlayedFolderLoc))
    {
        FileUtils::getInstance()->createDirectory(childRecentlyPlayedFolderLoc);
    }
    
    return childRecentlyPlayedFolderLoc + "/" + hq + _kRecentlyPlayedFileName;
}

NS_AZOOMEE_END
