//
//  ArtTileHolder.cpp
//  Azoomee
//
//  Created by Macauley.Scoffins on 15/10/2019.
//

#include "ArtTileHolder.h"
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/UI/LayoutParams.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

const float ArtTileHolder::kTileSpacingPercent = 0.035f;
const cocos2d::Vec2 ArtTileHolder::kTileAspectRatio = Vec2(1.0f, 0.625f);
const std::string ArtTileHolder::kEmptyArtFilename = "res/OomeeHQ/ArtStudio/new_painting_button.png";

bool ArtTileHolder::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    _bgColour = RoundedRectSprite::create();
    _bgColour->setTexture("res/decoration/white_1px.png");
    _bgColour->setCornerRadius(27);
    _bgColour->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _bgColour->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _bgColour->setColor(Style::Color::strongPink);
    addChild(_bgColour);
    
    _bgPattern = RoundedRectSprite::create();
    _bgPattern->setTexture("res/decoration/pattern_stem_tile.png");
    _bgPattern->setCornerRadius(27);
    _bgPattern->setScaleMode(RoundedRectSprite::ScaleMode::TILE);
    _bgPattern->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _bgPattern->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _bgPattern->setColor(Style::Color::strongPink);
    addChild(_bgPattern);
    
    createContentLayout();
    
    return true;
}

void ArtTileHolder::onEnter()
{
    Super::onEnter();
}

void ArtTileHolder::onExit()
{
    Super::onExit();
}

void ArtTileHolder::onSizeChanged()
{
    Super::onSizeChanged();
    const Size& contentSize = getContentSize();
    _bgColour->setContentSize(contentSize);
    _bgPattern->setContentSize(contentSize);
    _tileSpacing = contentSize.width * kTileSpacingPercent;
    _contentTileGrid->setPosition(Vec2(contentSize.width * 0.5f, contentSize.height - _tileSpacing));
    resizeContent();
}

void ArtTileHolder::toggleOpened(bool open)
{
    
}

void ArtTileHolder::setOnResizeCallback(const OnResizeCallback& callback)
{
    _resizeCallback = callback;
}

bool ArtTileHolder::isOpen() const
{
    return _open;
}

bool ArtTileHolder::isResizing() const
{
    return _resizing;
}

void ArtTileHolder::setArtFilenames(const std::vector<std::string>& artFilenames)
{
    _artFilenames = artFilenames;
    updateContent();
}

void ArtTileHolder::setTilePlaceholder(const std::string& tilePlaceholder)
{
    _tilePlaceholder = tilePlaceholder;
    for(auto tile : _contentTiles)
    {
        tile->setPlaceholderFilename(_tilePlaceholder);
    }
}

void ArtTileHolder::setEditCallback(const ArtContentTileCallback& callback)
{
    _editCallback = callback;
}

void ArtTileHolder::setDeleteCallback(const ArtContentTileCallback& callback)
{
    _deleteCallback = callback;
}

void ArtTileHolder::createContentLayout()
{
    _contentTileGrid = ui::Layout::create();
    _contentTileGrid->setContentSize(Size(getContentSize().width, 0));
    _contentTileGrid->setClippingEnabled(true);
    _contentTileGrid->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    addChild(_contentTileGrid);
}

void ArtTileHolder::updateContent()
{
    if(_artFilenames.size() > 0)
    {
        _contentTiles.clear();
        _contentRows.clear();
        _contentTileGrid->removeAllChildren();
        
        const Size& contentSize = getContentSize();
        
        const float tilesPerRow = 2.0f;
        
        const float rowWidth = contentSize.width - 2 * _tileSpacing;
        const float tileWidth = (rowWidth / tilesPerRow) - _tileSpacing;
        
        const Size& tileSize = Size(tileWidth * kTileAspectRatio.x, tileWidth * kTileAspectRatio.y); //16:10 aspect ratio tiles
        
        const int rows = ceil(_artFilenames.size() / tilesPerRow);
        
        const float totalHeight = (tileSize.height * rows) + (_tileSpacing * rows);
        for(int row = 0; row < rows; row++)
        {
            ui::Layout* rowContainer = ui::Layout::create();
            rowContainer->setContentSize(Size(rowWidth, tileSize.height));
            rowContainer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            rowContainer->setNormalizedPosition(Vec2(0.5f,1.0 - ((row + 0.5f) / rows)));
            _contentTileGrid->addChild(rowContainer);
            _contentRows.pushBack(rowContainer);
            for(int col = 0; col < tilesPerRow; col++)
            {
                if((row * tilesPerRow) + col < _artFilenames.size())
                {
                    ArtContentTile* tile = ArtContentTile::create();
                    tile->setPlaceholderFilename(_tilePlaceholder);
                    tile->setContentSize(tileSize);
                    tile->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                    tile->setNormalizedPosition(Vec2((col + 0.5) / tilesPerRow,0.5f));
                    tile->setEditCallback([this](const std::string& filename){
                        if(_editCallback)
                        {
                            _editCallback(filename);
                        }
                    });
                    tile->setDeleteCallback([this](const std::string& filename){
                        if(_deleteCallback)
                        {
                            _deleteCallback(filename);
                        }
                    });
                    tile->setTouchEnabled(false);
                    tile->setArtFilename(_artFilenames.at((row * tilesPerRow) + col));
                    rowContainer->addChild(tile);
                    _contentTiles.pushBack(tile);
                }
            }
        }
        _contentGridSize = Size(rowWidth, totalHeight);
        _contentTileGrid->setContentSize(_contentGridSize);
        if(contentSize.height != totalHeight + (2 * _tileSpacing))
        {
            setContentSize(Size(contentSize.width, totalHeight + (2 * _tileSpacing)));
        }
        //_contentTileGrid->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,_tileSpacing, 0, 0)));
        
    }
}

void ArtTileHolder::resizeContent()
{
    if(_artFilenames.size() > 0)
    {
        const Size& contentSize = getContentSize();
        
        const float tilesPerRow = 2.0f;
        
        const float rowWidth = contentSize.width - 2 * _tileSpacing;
        const float tileWidth = (rowWidth / tilesPerRow) - _tileSpacing;
        
        const Size& tileSize = Size(tileWidth * kTileAspectRatio.x, tileWidth * kTileAspectRatio.y); //16:10 aspect ratio tiles
        
        const int rows = ceil(_artFilenames.size() / tilesPerRow);
        
        float totalHeight = (tileSize.height * rows) + (_tileSpacing * rows);
        for(auto tile : _contentTiles)
        {
            tile->setContentSize(tileSize);
        }
        
        for(auto row : _contentRows)
        {
            row->setContentSize(Size(rowWidth, tileSize.height));
        }
        
        _contentGridSize = Size(rowWidth, totalHeight);
        _contentTileGrid->setContentSize(_contentGridSize);
        if(contentSize.height != totalHeight + (2 * _tileSpacing))
        {
            setContentSize(Size(contentSize.width, totalHeight + (2 * _tileSpacing)));
        }
    }
}

NS_AZOOMEE_END
