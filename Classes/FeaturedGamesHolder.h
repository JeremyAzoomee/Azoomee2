//
//  FeaturedGamesHolder.h
//  Azoomee
//
//  Created by Macauley.Scoffins on 03/09/2019.
//

#ifndef FeaturedGamesHolder_h
#define FeaturedGamesHolder_h

#include "ContentTileHolder.h"
#include "FeaturedTile.h"

NS_AZOOMEE_BEGIN

class FeaturedGamesHolder : public ContentTileHolder
{
    typedef ContentTileHolder Super;
private:
    static const cocos2d::Size kMainTileSize;
    static const cocos2d::Size kSubTileSize;
    static const float kTileSpacing;
    static const cocos2d::Size kTargetSize;
    
    FeaturedTile* _mainTile = nullptr;
    cocos2d::ui::Layout* _subTiles = nullptr;
    FeaturedTile* _subTile1 = nullptr;
    FeaturedTile* _subTile2 = nullptr;
    
public:
    bool init() override;
    void onEnter() override;
    void onExit() override;
    void onSizeChanged() override;
    
    CREATE_FUNC(FeaturedGamesHolder);
};

NS_AZOOMEE_END

#endif /* FeaturedGamesHolder_h */
