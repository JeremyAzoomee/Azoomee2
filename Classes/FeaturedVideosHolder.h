//
//  FeaturedVideosHolder.h
//  Azoomee
//
//  Created by Macauley.Scoffins on 04/09/2019.
//

#ifndef FeaturedVideosHolder_h
#define FeaturedVideosHolder_h

#include "ContentTileHolder.h"
#include "FeaturedTile.h"

NS_AZOOMEE_BEGIN

class FeaturedVideosHolder : public ContentTileHolder
{
    typedef ContentTileHolder Super;
private:
    
    FeaturedTile* _tile1 = nullptr;;
    FeaturedTile* _tile2 = nullptr;
    FeaturedTile* _tile3 = nullptr;
    
public:
    static const float kTileSpacing;
    
    bool init() override;
    void onEnter() override;
    void onExit() override;
    void onSizeChanged() override;
    
    CREATE_FUNC(FeaturedVideosHolder);
};

NS_AZOOMEE_END

#endif /* FeaturedVideosHolder_h */
