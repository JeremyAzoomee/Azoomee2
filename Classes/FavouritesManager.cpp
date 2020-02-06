//
//  FavouritesManager.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 29/03/2018.
//

#include "FavouritesManager.h"
#include <AzoomeeCommon/Data/Child/ChildManager.h>
#include <AzoomeeCommon/Utils/StringFunctions.h>
#include "HQDataProvider.h"
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include <AzoomeeCommon/Utils/DirUtil.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

static std::auto_ptr<FavouritesManager> sFavouritesManagerSharedInstance;

FavouritesManager* FavouritesManager::getInstance(void)
{
    if (!sFavouritesManagerSharedInstance.get())
    {
        sFavouritesManagerSharedInstance.reset(new FavouritesManager());
    }
    
    return sFavouritesManagerSharedInstance.get();
}

FavouritesManager::~FavouritesManager()
{
    
}

void FavouritesManager::addToFavourites(const HQContentItemObjectRef& contentItem)
{
    const std::string& contentId = contentItem->getContentItemId();
    std::vector<std::string> fileIds = getFavouriteContentIds();
    
    auto pivot = std::find(fileIds.begin(), fileIds.end(), contentId);
    if(pivot == fileIds.end()) // not already favourited
    {
        fileIds.insert(fileIds.begin(), contentId);
        const std::string& newIdList = StringFunctions::joinStrings(fileIds, "/");
        FileUtils::getInstance()->writeStringToFile(newIdList, getFavouritesFilePath());
        AnalyticsSingleton::getInstance()->favouriteContentItem(contentId);
    }
}

void FavouritesManager::removeFromFavourites(const HQContentItemObjectRef& contentItem)
{
    const std::string& contentId = contentItem->getContentItemId();
    std::vector<std::string> fileIds = getFavouriteContentIds();
    
    auto pivot = std::find(fileIds.begin(), fileIds.end(), contentId);
    if(pivot != fileIds.end()) // not already favourited
    {
        fileIds.erase(pivot);
        std::string newIdList = StringFunctions::joinStrings(fileIds, "/");
        if(newIdList == "")
        {
            newIdList = "-";
        }
        FileUtils::getInstance()->writeStringToFile(newIdList, getFavouritesFilePath());
        AnalyticsSingleton::getInstance()->unFavouriteContentItem(contentId);
    }
}

std::vector<HQContentItemObjectRef> FavouritesManager::getFavouriteContent() const
{
    const std::vector<std::string>& ids = getFavouriteContentIds();
    return HQDataProvider::getInstance()->getContentItemsFromIDs(ids);
}

std::string FavouritesManager::getFavouritesFilePath() const
{
    const std::string& favouritesFolderLoc = DirUtil::getCachesPath() + kFavouritesFolderName;
    if(!FileUtils::getInstance()->isDirectoryExist(favouritesFolderLoc))
    {
        FileUtils::getInstance()->createDirectory(favouritesFolderLoc);
    }
    const std::string& childFavouritesFolderLoc = favouritesFolderLoc + ChildManager::getInstance()->getLoggedInChild()->getId();
    if(!FileUtils::getInstance()->isDirectoryExist(childFavouritesFolderLoc))
    {
        FileUtils::getInstance()->createDirectory(childFavouritesFolderLoc);
    }
    
    return childFavouritesFolderLoc + "/" + kFavouritesFileName;
}

std::vector<std::string> FavouritesManager::getFavouriteContentIds() const
{
    const std::string& fileStr = FileUtils::getInstance()->getStringFromFile(getFavouritesFilePath());
    
    return StringFunctions::splitStringToVector(fileStr, "/");
}

bool FavouritesManager::isFavouriteContent(const std::string &contentId)
{
    const std::vector<std::string>& favList = getFavouriteContentIds();
    return std::find(favList.begin(), favList.end(), contentId) != favList.end();
}

NS_AZOOMEE_END
