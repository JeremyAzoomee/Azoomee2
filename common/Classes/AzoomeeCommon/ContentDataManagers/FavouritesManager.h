//
//  FavouritesManager.h
//  azoomee2
//
//  Created by Macauley on 29/03/2018.
//

#ifndef FavouritesManager_h
#define FavouritesManager_h

#include <cocos/cocos2d.h>
#include "../Tinizine.h"
#include "../Data/HQDataObject/HQContentItemObject.h"

NS_TZ_BEGIN

class FavouritesManager
{
private:
    const std::string kFavouritesFolderName = "Favourites/";
    const std::string kFavouritesFileName = "favourites.txt";
    
    std::string getFavouritesFilePath() const;
    std::vector<std::string> getFavouriteContentIds() const;
    
public:
    static FavouritesManager* getInstance(void);
    
    virtual ~FavouritesManager();
    
    void addToFavourites(const HQContentItemObjectRef& contentItem);
    void removeFromFavourites(const HQContentItemObjectRef& contentItem);
    
    bool isFavouriteContent(const std::string& contentId);
    
    std::vector<HQContentItemObjectRef> getFavouriteContent() const;
    
};

NS_TZ_END

#endif /* FavouritesManager_h */
