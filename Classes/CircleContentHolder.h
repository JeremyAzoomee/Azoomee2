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
#include "Azoomee.h"

USING_NS_TZ

NS_AZ_BEGIN

class CircleContentHolder : public ContentTileHolder
{
    typedef ContentTileHolder Super;
private:
    
    int _rows = -1;
    int _minCols = 4;
    cocos2d::Size _tileSize;
    cocos2d::Vector<CircleTile*> _tiles;
    
    bool _scaleToFill = true;
    
    std::string _placeholderFilename;
    
    void refreshTiles();
    
public:
    static const float kTileSpacingPercent;
    
    bool init() override;
    void onEnter() override;
    void onExit() override;
    void onSizeChanged() override;
    
    void setTileSize(const cocos2d::Size& size);
    void setMaxRows(int rows);
    void setMinColumns(int minColumns);
    void enableScaleToFill(bool enable);
    
    void setPlaceholder(const std::string& filename);
    
    CREATE_FUNC(CircleContentHolder);
};

NS_AZ_END

#endif /* CircleContentHolder_h */
