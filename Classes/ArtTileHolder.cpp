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

const float ArtTileHolder::kTileSpacingPercent = 0.025f;
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
    _bgColour->setColor(Style::Color::purplyPink);
    _bgColour->setOpacity(125);
    addChild(_bgColour);
    
    _bgPattern = RoundedRectSprite::create();
    _bgPattern->setTexture("res/decoration/pattern_stem_tile.png");
    _bgPattern->setCornerRadius(27);
    _bgPattern->setScaleMode(RoundedRectSprite::ScaleMode::TILE);
    _bgPattern->setTileScaleFactor(2.0f);
    _bgPattern->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _bgPattern->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _bgPattern->setColor(Style::Color::purplyPink);
    addChild(_bgPattern);
    
    createContentLayout();
    
    _resizeToggle = ui::Button::create("res/OomeeHQ/ArtStudio/toggle_switch_closed.png");
    _resizeToggle->ignoreContentAdaptWithSize(false);
    _resizeToggle->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _resizeToggle->setVisible(false);
    _resizeToggle->addTouchEventListener([this](Ref* pSender, ui::Widget::TouchEventType eType){
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            if(_togglePressedCallback)
            {
                _togglePressedCallback();
            }
            this->toggleOpened(!_open);
        }
    });
    addChild(_resizeToggle);
    
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
    _contentClippingLayout->setPosition(Vec2(contentSize.width * 0.5f, contentSize.height - _tileSpacing));
    _resizeToggle->setPosition(Vec2(contentSize.width * 0.5f, 2.0f * _tileSpacing));
    _resizeToggle->setContentSize(_resizeToggle->getNormalTextureSize() * ((2.0f * _tileSpacing) / _resizeToggle->getNormalTextureSize().height));
    resizeContent();
}

void ArtTileHolder::toggleOpened(bool open)
{
    _open = open;
    _resizeToggle->setRotation(_open ? 180 : 0);

    const Size& currentSize = getContentSize();
    const float targetHeight = _open ? _openHeight : _closedHeight;
    const float dist = abs(currentSize.height - targetHeight);
    const float maxDist = _openHeight - _closedHeight;
    const float durationMod = dist / maxDist;
    const float targetTime = 0.5f;
    const float internalHeightOffset = ((_artFilenames.size() > 4 ? 5 : 2) * _tileSpacing);
    
    ActionFloat* resizeAction = ActionFloat::create(targetTime * durationMod, currentSize.height, targetHeight, [this, internalHeightOffset](float height){
        this->setContentSize(Size(getContentSize().width, height));
        _contentClippingLayout->setContentSize(Size(_contentGridSize.width, height - internalHeightOffset));
        //_contentTileGrid->setContentSize(Size(_contentGridSize.width, MIN(height - _closedHeight - (2.0f * _tileSpacing),_contentGridSize.height)));
        if(_resizeCallback)
        {
            _resizeCallback();
        }
    });
    _resizing = true;
    stopAllActions();
    
    runAction(Sequence::createWithTwoActions(EaseSineInOut::create(resizeAction), CallFunc::create([this](){
        _resizing = false;
    })));
    
    //setContentSize(Size(getContentSize().width, _open ? _openHeight : _closedHeight));
    //if(_resizeCallback)
    //{
    //    _resizeCallback();
    //}
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
    bool newData = false;
    if(_artFilenames.size() == artFilenames.size())
    {
        for(int i = 0; i < _artFilenames.size(); i++)
        {
            if(_artFilenames.at(i) != artFilenames.at(i))
            {
                newData = true;
                continue;
            }
        }
    }
    else
    {
        newData = true;
    }
    if(newData)
    {
        _artFilenames = artFilenames;
        updateContent();
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
void ArtTileHolder::setToggleSelectedCallback(const OnResizeCallback &callback)
{
    _togglePressedCallback = callback;
}

void ArtTileHolder::createContentLayout()
{
    _contentClippingLayout = ui::Layout::create();
    _contentClippingLayout->setClippingEnabled(true);
    _contentClippingLayout->setContentSize(Size(getContentSize().width, 0));
    _contentClippingLayout->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    addChild(_contentClippingLayout);
    
    _contentTileGrid = ui::Layout::create();
    _contentTileGrid->setContentSize(Size(getContentSize().width, 0));
    _contentTileGrid->setClippingEnabled(true);
    _contentTileGrid->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _contentTileGrid->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_TOP);
    _contentClippingLayout->addChild(_contentTileGrid);
}

void ArtTileHolder::updateContent()
{
    if(_artFilenames.size() > 0)
    {
        _contentTiles.clear();
        _contentRows.clear();
        _contentTileGrid->removeAllChildren();
        _resizeToggle->setVisible(_artFilenames.size() > 4);
        
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
                    const std::string& filename = _artFilenames.at((row * tilesPerRow) + col);
                    ArtContentTile* tile = ArtContentTile::create();
                    tile->setPlaceholderFilename("res/contentPlaceholders/Create1X1.png");
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
                    tile->setArtFilename(filename);
                    tile->setNewArtTile(filename == kEmptyArtFilename);
                    
                    rowContainer->addChild(tile);
                    _contentTiles.pushBack(tile);
                }
            }
        }
        _contentGridSize = Size(rowWidth, totalHeight);
        _contentTileGrid->setContentSize(_contentGridSize);
        _openHeight = totalHeight + ((_artFilenames.size() > 4 ? 5 : 2) * _tileSpacing);
        _closedHeight = (tileSize.height * MIN(rows, 2)) + (_tileSpacing * MIN(rows,2)) + ((_artFilenames.size() > 4 ? 5 : 2) * _tileSpacing);
        _contentClippingLayout->setContentSize(Size(rowWidth, _open ? totalHeight : (tileSize.height * MIN(rows, 2)) + (_tileSpacing * MIN(rows, 2))));
        const float contentHeight = _open ? _openHeight : _closedHeight;
        if(contentSize.height != contentHeight)
        {
            setContentSize(Size(contentSize.width, contentHeight));
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
        _openHeight = totalHeight + ((_artFilenames.size() > 4 ? 5 : 2) * _tileSpacing);
        _closedHeight = (tileSize.height * MIN(rows, 2)) + (_tileSpacing * MIN(rows, 2)) + ((_artFilenames.size() > 4 ? 5 : 2) * _tileSpacing);
        if(!_resizing)
        {
            _contentClippingLayout->setContentSize(Size(rowWidth, _open ? totalHeight : (tileSize.height * MIN(rows, 2)) + (_tileSpacing * MIN(rows, 2))));
            const float contentHeight = _open ? _openHeight : _closedHeight;
            if(contentSize.height != contentHeight)
            {
                setContentSize(Size(contentSize.width, contentHeight));
            }
        }
    }
}

NS_AZOOMEE_END
