//
//  CircleContentHolder.h
//  Azoomee
//
//  Created by Macauley.Scoffins on 05/09/2019.
//

#ifndef CircleContentHolder_h
#define CircleContentHolder_h

#include "ContentTileHolder.h"
#include "CircleTile.h"
#include <AzoomeeCommon/UI/DynamicText.h>

NS_AZOOMEE_BEGIN

class CircleContentHolder : public ContentTileHolder
{
    typedef ContentTileHolder Super;
private:
    
    int _rows = -1;
    cocos2d::Size _tileSize;
    cocos2d::Vector<CircleTile*> _tiles;
    
    void refreshTiles();
    
public:
    static const float kTileSpacing;
    
    bool init() override;
    void onEnter() override;
    void onExit() override;
    void onSizeChanged() override;
    
    void setTileSize(const cocos2d::Size& size);
    void setMaxRows(int rows);
    
    CREATE_FUNC(CircleContentHolder);
};

NS_AZOOMEE_END

#endif /* CircleContentHolder_h */
