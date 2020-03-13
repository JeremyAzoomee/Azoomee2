//
//  FriendsList.cpp
//  Azoomee
//
//  Created by Macauley.Scoffins on 24/10/2019.
//

#include "FriendsList.h"
#include <TinizineCommon/UI/Colour.h>
#include <TinizineCommon/UI/LayoutParams.h>
#include "AddFriendTile.h"

using namespace cocos2d;

USING_NS_TZ

NS_AZ_BEGIN

const float FriendsList::kTileSpacingPercent = 0.5f;

bool FriendsList::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    setSizeType(SizeType::ABSOLUTE);
    
    setLayoutType(Type::VERTICAL);
    
    return true;
}

void FriendsList::onEnter()
{
    refreshTiles();
    Super::onEnter();
}

void FriendsList::onExit()
{
    Super::onExit();
}

void FriendsList::onSizeChanged()
{
    Super::onSizeChanged();
    refreshTiles();
}

void FriendsList::setTileSize(const cocos2d::Size &size)
{
    const float edgeSize = MIN(size.width, size.height);
    _tileSize = Size(edgeSize, edgeSize);
}

void FriendsList::setMaxRows(int rows)
{
    _rows = rows;
}

void FriendsList::setMinColumns(int minColumns)
{
    _minCols = minColumns;
}

void FriendsList::enableScaleToFill(bool enable)
{
    _scaleToFill = enable;
}

void FriendsList::setPlaceholder(const std::string &filename)
{
    _placeholderFilename = filename;
}

void FriendsList::setFriendSelectedCallback(const FriendSelectedCallback &callback)
{
    _callback = callback;
}

void FriendsList::setAddFriendSelectedCallback(const AddFriendSelectedCallback& callback)
{
    _addFriendCallback = callback;
}

void FriendsList::setShowAddFriendTile(bool show)
{
    _showAddFriendTile = show;
}

void FriendsList::setFriendsList(const Chat::FriendList& friends)
{
    if(_friends.size() != friends.size())
    {
        _friends = friends;
        refreshTiles();
    }
}

void FriendsList::refreshTiles()
{
    _tiles.clear();
    removeAllChildren();
    
    const Size& contentSize = getContentSize();
    
    const float paddingSize = _tileSize.width * kTileSpacingPercent;
    const Size& paddedTileSize = Size(_tileSize.width + paddingSize, _tileSize.width * 1.5f + paddingSize);
    
    const int tilesPerRow = MAX(floor(contentSize.width / paddedTileSize.width), _minCols);
    
    const float rowWidth = tilesPerRow * paddedTileSize.width;
    
    int rows = 1;
    
    if(_rows > 0)
    {
        rows = _rows;
    }
    else if(_friends.size() > 0)
    {
        rows = ceil((_friends.size() + _showAddFriendTile) / (float)tilesPerRow);
    }
    float scaleToFill = _scaleToFill ? contentSize.width / rowWidth : 1.0f;
    float totalHeight = (paddedTileSize.height * scaleToFill) * rows;
    for(int row = 0; row < rows; row++)
    {
        ui::Layout* rowContainer = ui::Layout::create();
        rowContainer->setContentSize(Size(rowWidth, paddedTileSize.height));
        rowContainer->setLayoutType(Type::HORIZONTAL);
        rowContainer->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
        rowContainer->setScale(scaleToFill);
        addChild(rowContainer);
        for(int col = 0; col < tilesPerRow; col++)
        {
            if(_showAddFriendTile && row == 0 && col == 0)
            {
                AddFriendTile* tile = AddFriendTile::create();
                tile->setTileWidth(_tileSize.width);
                tile->setLayoutParameter(CreateCenterVerticalLinearLayoutParam(ui::Margin(_tileSize.width * (kTileSpacingPercent / 2.0f), 0, _tileSize.width * (kTileSpacingPercent / 2.0f), 0)));
                tile->setSelectedCallback([this](){
                    if(_addFriendCallback)
                    {
                        _addFriendCallback();
                    }
                });
                rowContainer->addChild(tile);
            }
            else
            {
                FriendTile* tile = FriendTile::create();
                tile->setTileWidth(_tileSize.width);
                tile->setLayoutParameter(CreateCenterVerticalLinearLayoutParam(ui::Margin(_tileSize.width * (kTileSpacingPercent / 2.0f), 0, _tileSize.width * (kTileSpacingPercent / 2.0f), 0)));
                tile->setSelectedCallback([this](Chat::FriendRef friendData){
                    if(_callback)
                    {
                        _callback(friendData);
                    }
                });
                tile->setTouchEnabled(false);
                rowContainer->addChild(tile);
                _tiles.pushBack(tile);
            }
        }
    }
    
    setContentSize(Size(contentSize.width, totalHeight));
    for(int i = 0; i < MIN(_tiles.size(), _friends.size()); i++)
    {
        FriendTile* tile = _tiles.at(i);
        tile->setFriendData(_friends.at(i));
        tile->setTouchEnabled(true);
    }
}


NS_AZ_END
