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
    
    FeaturedTile* _mainTile = nullptr;
    FeaturedTile* _subTile1 = nullptr;
    FeaturedTile* _subTile2 = nullptr;
    
    bool _useWideLayout = false;
    bool _useFixedHeight = false;
    
public:
    static const float kTileSpacing;
    
    bool init() override;
    void onEnter() override;
    void onExit() override;
    void onSizeChanged() override;
    
    void enableWideLayout(bool enable);
    void enableFixedHeight(bool enable);
    
    CREATE_FUNC(FeaturedGamesHolder);
};

NS_AZOOMEE_END

#endif /* FeaturedGamesHolder_h */
