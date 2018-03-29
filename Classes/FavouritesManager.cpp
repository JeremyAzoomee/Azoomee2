//
//  FavouritesManager.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 29/03/2018.
//

#include "FavouritesManager.h"
#include <AzoomeeCommon/Data/Child/ChildDataProvider.h>
#include <AzoomeeCommon/Utils/StringFunctions.h>
#include "HQDataProvider.h"

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
        const std::string& newIdList = joinStrings(fileIds, "/");
        FileUtils::getInstance()->writeStringToFile(newIdList, getFavouritesFilePath());
    }
}

void FavouritesManager::removeFromFavourites(const HQContentItemObject& contentItem)
{
    
}

std::vector<HQContentItemObjectRef> FavouritesManager::getFavouriteContent() const
{
    std::vector<HQContentItemObjectRef> favContent;
    
    const std::vector<std::string>& ids = getFavouriteContentIds();
    for(const std::string& id : ids)
    {
        HQContentItemObjectRef content = HQDataProvider::getInstance()->getItemDataForSpecificItem(id);
        if(content)
        {
            favContent.push_back(content);
        }
    }
    
    return favContent;
}

std::string FavouritesManager::getFavouritesFilePath() const
{
    const std::string& favouritesFolderLoc = FileUtils::getInstance()->getWritablePath() + kFavouritesFolderName;
    if(!FileUtils::getInstance()->isDirectoryExist(favouritesFolderLoc))
    {
        FileUtils::getInstance()->createDirectory(favouritesFolderLoc);
    }
    const std::string& childFavouritesFolderLoc = favouritesFolderLoc + ChildDataProvider::getInstance()->getLoggedInChildId();
    if(!FileUtils::getInstance()->isDirectoryExist(childFavouritesFolderLoc))
    {
        FileUtils::getInstance()->createDirectory(childFavouritesFolderLoc);
    }
    
    return childFavouritesFolderLoc + "/" + kFavouritesFileName;
}

std::vector<std::string> FavouritesManager::getFavouriteContentIds() const
{
    const std::string& fileStr = FileUtils::getInstance()->getStringFromFile(getFavouritesFilePath());
    
    return splitStringToVector(fileStr, "/");
}

NS_AZOOMEE_END
