//
//  ArtTileHolder.cpp
//  Azoomee
//
//  Created by Macauley.Scoffins on 15/10/2019.
//

#include "ArtTileHolder.h"
#include <AzoomeeCommon/UI/Colour.h>
#include <AzoomeeCommon/UI/LayoutParams.h>
#include "HQConstants.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

const float ArtTileHolder::kTileSpacingPercent = 0.025f;
const cocos2d::Vec2 ArtTileHolder::kTileAspectRatio = Vec2(1.0f, 0.625f);
const std::string ArtTileHolder::kEmptyArtFilename = "res/OomeeHQ/ArtStudio/new_painting_button.png";
const int ArtTileHolder::kClosedMaxVisibleTileCount = 4;
const float ArtTileHolder::kTilesPerRow = 2.0f;

bool ArtTileHolder::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    _bgColour = RoundedRectSprite::create();
    _bgColour->setTexture("res/decoration/white_1px.png");
    _bgColour->setCornerRadius(HQConsts::OomeeHQTileCornerRadius);
    _bgColour->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _bgColour->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _bgColour->setColor(Colours::Color_3B::white);
    _bgColour->setOpacity(125);
    addChild(_bgColour);
    
    _bgPattern = RoundedRectSprite::create();
    _bgPattern->setTexture("res/decoration/pattern_stem_tile.png");
    _bgPattern->setCornerRadius(HQConsts::OomeeHQTileCornerRadius);
    _bgPattern->setScaleMode(RoundedRectSprite::ScaleMode::TILE);
    _bgPattern->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _bgPattern->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _bgPattern->setColor(Colours::Color_3B::white);
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
    _contentGridPadding = ((_expandable ? 5 : 2) * _tileSpacing);
    _contentClippingLayout->setPosition(Vec2(contentSize.width * 0.5f, contentSize.height - _tileSpacing));
    _resizeToggle->setPosition(Vec2(contentSize.width * 0.5f, 2.0f * _tileSpacing));
    _resizeToggle->setContentSize(_resizeToggle->getNormalTextureSize() * ((3.0f * _tileSpacing) / _resizeToggle->getNormalTextureSize().height));
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
    
    ActionFloat* resizeAction = ActionFloat::create(targetTime * durationMod, currentSize.height, targetHeight, [this](float height){
        this->setContentSize(Size(getContentSize().width, height));
        _contentClippingLayout->setContentSize(Size(_contentGridSize.width, height - _contentGridPadding));
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
    if(_artFilenames.size() == artFilenames.size())
    {
        for(int i = 0; i < _artFilenames.size(); i++)
        {
            if(_artFilenames.at(i) != artFilenames.at(i))
            {
                _artFilenames = artFilenames;
                updateContent();
                return;
            }
        }
    }
    else
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
    const Size& contentSize = getContentSize();
    _contentClippingLayout = ui::Layout::create();
    _contentClippingLayout->setClippingEnabled(true);
    _contentClippingLayout->setContentSize(Size(contentSize.width, 0));
    _contentClippingLayout->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    addChild(_contentClippingLayout);
    
    _contentTileGrid = ui::Layout::create();
    _contentTileGrid->setContentSize(Size(contentSize.width, 0));
    _contentTileGrid->setClippingEnabled(true);
    _contentTileGrid->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _contentTileGrid->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_TOP);
    _contentClippingLayout->addChild(_contentTileGrid);
}

void ArtTileHolder::updateContent()
{
    if(_artFilenames.size() > 0)
    {
        _expandable = _artFilenames.size() > kClosedMaxVisibleTileCount;
        _contentGridPadding = ((_expandable ? 5 : 2) * _tileSpacing);
        _contentTiles.clear();
        _contentRows.clear();
        _contentTileGrid->removeAllChildren();
        _resizeToggle->setVisible(_expandable);
        
        const Size& contentSize = getContentSize();
        
        const float rowWidth = contentSize.width - 2 * _tileSpacing;
        
        const Size& tileSize = calcTileSize(rowWidth);
        
        const int rows = ceil(_artFilenames.size() / kTilesPerRow);
        
        const float totalHeight = (tileSize.height * rows) + (_tileSpacing * rows);
        for(int row = 0; row < rows; row++)
        {
            ui::Layout* rowContainer = ui::Layout::create();
            rowContainer->setContentSize(Size(rowWidth, tileSize.height));
            rowContainer->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
            rowContainer->setNormalizedPosition(Vec2(0.5f,1.0 - ((row + 0.5f) / rows)));
            _contentTileGrid->addChild(rowContainer);
            _contentRows.pushBack(rowContainer);
            for(int col = 0; col < kTilesPerRow; col++)
            {
                if((row * kTilesPerRow) + col < _artFilenames.size())
                {
                    const std::string& filename = _artFilenames.at((row * kTilesPerRow) + col);
                    ArtContentTile* tile = ArtContentTile::create();
                    tile->setPlaceholderFilename("res/contentPlaceholders/Create1X1.png");
                    tile->setContentSize(tileSize);
                    tile->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
                    tile->setNormalizedPosition(Vec2((col + 0.5) / kTilesPerRow,0.5f));
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
        _openHeight = totalHeight + _contentGridPadding;
        _closedHeight = (tileSize.height * MIN(rows, 2)) + (_tileSpacing * MIN(rows,2)) + _contentGridPadding;
        _contentClippingLayout->setContentSize(Size(rowWidth, _open ? totalHeight : _closedHeight - _contentGridPadding));
        const float contentHeight = _open ? _openHeight : _closedHeight;
        if(contentSize.height != contentHeight)
        {
            setContentSize(Size(contentSize.width, contentHeight));
        }
        
    }
}

void ArtTileHolder::resizeContent()
{
    if(_artFilenames.size() > 0)
    {
        const Size& contentSize = getContentSize();
        
        const float rowWidth = contentSize.width - 2 * _tileSpacing;
        
        const Size& tileSize = calcTileSize(rowWidth);
        
        const int rows = ceil(_artFilenames.size() / kTilesPerRow);
        
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
        _openHeight = totalHeight + _contentGridPadding;
        _closedHeight = (tileSize.height * MIN(rows, 2)) + (_tileSpacing * MIN(rows, 2)) + _contentGridPadding;
        if(!_resizing)
        {
            _contentClippingLayout->setContentSize(Size(rowWidth, _open ? totalHeight : _closedHeight - _contentGridPadding));
            const float contentHeight = _open ? _openHeight : _closedHeight;
            if(contentSize.height != contentHeight)
            {
                setContentSize(Size(contentSize.width, contentHeight));
            }
        }
    }
}

cocos2d::Size ArtTileHolder::calcTileSize(float rowWidth)
{
    const float tileWidth = (rowWidth / kTilesPerRow) - _tileSpacing;
    
    return Size(tileWidth * kTileAspectRatio.x, tileWidth * kTileAspectRatio.y); //16:10 aspect ratio tiles
}

NS_AZOOMEE_END
