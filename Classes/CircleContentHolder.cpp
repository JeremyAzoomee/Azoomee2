//
//  CircleContentHolder.cpp
//  Azoomee
//
//  Created by Macauley.Scoffins on 05/09/2019.
//

#include "CircleContentHolder.h"
#include <AzoomeeCommon/UI/LayoutParams.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

const float CircleContentHolder::kTileSpacingPercent = 0.1f;

bool CircleContentHolder::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    setSizeType(SizeType::ABSOLUTE);
    
    _contentLayout->setLayoutType(Type::VERTICAL);
    
    return true;
}

void CircleContentHolder::onEnter()
{
    refreshTiles();
    Super::onEnter();
}

void CircleContentHolder::onExit()
{
    Super::onExit();
}

void CircleContentHolder::onSizeChanged()
{
    Super::onSizeChanged();
    refreshTiles();
}

void CircleContentHolder::setTileSize(const cocos2d::Size &size)
{
    const float edgeSize = MIN(size.width, size.height);
    _tileSize = Size(edgeSize, edgeSize);
}

void CircleContentHolder::setMaxRows(int rows)
{
    _rows = rows;
}

void CircleContentHolder::setMinColumns(int minColumns)
{
    _minCols = minColumns;
}

void CircleContentHolder::enableScaleToFill(bool enable)
{
    _scaleToFill = enable;
}

void CircleContentHolder::setPlaceholder(const std::string &filename)
{
    _placeholderFilename = filename;
}

void CircleContentHolder::refreshTiles()
{
    _tiles.clear();
    _contentLayout->removeAllChildren();
    
    const Size& contentSize = getContentSize();
    
    const float paddedCircleSize = _tileSize.width * (1.0f + kTileSpacingPercent);
    
    const int tilesPerRow = MAX(floor(contentSize.width / paddedCircleSize), _minCols);
    
    const float rowWidth = tilesPerRow * paddedCircleSize;
    
    int rows = 1;
    
    if(_rows > 0)
    {
        rows = _rows;
    }
    else if(_contentData)
    {
        rows = ceil(_contentData->getContentItems().size() / (float)tilesPerRow);
    }
    float scaleToFill = _scaleToFill ? contentSize.width / rowWidth : 1.0f;
    float totalHeight = (paddedCircleSize * scaleToFill) * rows;
    for(int row = 0; row < rows; row++)
    {
        ui::Layout* rowContainer = ui::Layout::create();
        rowContainer->setContentSize(Size(rowWidth, paddedCircleSize));
        rowContainer->setLayoutType(Type::HORIZONTAL);
        rowContainer->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
        rowContainer->setScale(scaleToFill);
        _contentLayout->addChild(rowContainer);
        for(int col = 0; col < tilesPerRow; col++)
        {
            CircleTile* tile = CircleTile::create();
            tile->setContentSize(_tileSize);
            tile->setLayoutParameter(CreateCenterVerticalLinearLayoutParam(ui::Margin(_tileSize.width * (kTileSpacingPercent / 2.0f), 0, _tileSize.width * (kTileSpacingPercent / 2.0f), 0)));
            tile->setContentSelectedCallback([this, row, tilesPerRow, col](HQContentItemObjectRef content){
                if(_callback)
                {
                    _callback(content, (row * tilesPerRow) + col);
                }
            });
            tile->setTouchEnabled(false);
            tile->setPlaceholderFilename(_placeholderFilename);
            tile->setEmptyImage(_placeholderFilename);
            rowContainer->addChild(tile);
            _tiles.pushBack(tile);
        }
    }
    _contentLayout->setContentSize(Size(rowWidth, totalHeight));
    
    setContentSize(Size(contentSize.width, totalHeight));
    if(_contentData)
    {
        const auto& content = _contentData->getContentItems();
        for(int i = 0; i < MIN(_tiles.size(), content.size()); i++)
        {
            CircleTile* tile = _tiles.at(i);
            tile->setContentItemData(content.at(i));
            tile->setTouchEnabled(true);
        }
    }
}

NS_AZOOMEE_END
