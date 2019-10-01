//
//  DropdownContentHolder.cpp
//  Azoomee
//
//  Created by Macauley.Scoffins on 09/09/2019.
//

#include "DropdownContentHolder.h"
#include <AzoomeeCommon/UI/LayoutParams.h>
#include <AzoomeeCommon/UI/Style.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

const cocos2d::Rect DropdownContentHolder::kBgCapInsets = Rect(162, 162, 103, 100);
const float DropdownContentHolder::kTileSpacing = 32.0f;
const float DropdownContentHolder::kDropdownOpenIconScale = 0.885f;
const cocos2d::Vec2 DropdownContentHolder::kTileAspectRatio = Vec2(1.0f, 0.75f);

bool DropdownContentHolder::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    _iconDownloader = ImageDownloader::create("imageCache", ImageDownloader::CacheMode::File);
    
    setBackGroundImage("res/hqscene/dropdown_bg.png");
    setBackGroundImageScale9Enabled(true);
    setBackGroundImageCapInsets(kBgCapInsets);
    
    setSizeType(SizeType::ABSOLUTE);
    
    _bgPattern = RoundedRectSprite::create();
    _bgPattern->setTexture("res/decoration/pattern_stem_tile.png");
    _bgPattern->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _bgPattern->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_TOP);
    _bgPattern->setCornerRadius(150);
    Texture2D* texture = _bgPattern->getTexture();
    if(texture)
    {
        const Size& texSize = texture->getContentSizeInPixels();
        CCASSERT(texSize.width == ccNextPOT(texSize.width) && texSize.height == ccNextPOT(texSize.height),
                 "TileSprite only works with PO2 textures");
        
        Texture2D::TexParams params;
        params.minFilter = GL_NEAREST;
        params.magFilter = GL_NEAREST;
        params.wrapS = GL_REPEAT;
        params.wrapT = GL_REPEAT;
        texture->setTexParameters(params);
    }
    addChild(_bgPattern, -1);
    
    _contentLayout->setLayoutType(Type::VERTICAL);
    _contentLayout->setSizeType(SizeType::PERCENT);
    _contentLayout->setSizePercent(Vec2(1.0f, 1.0f));
    _contentLayout->setClippingEnabled(true);
    
    createTitleLayout();
    
    createContentLayout();
    
    return true;
}

void DropdownContentHolder::onEnter()
{
    setContentLayoutVisible(_open);
    const float targetHeight = _open ? _openHeight : _closedHeight;
    const Size& contentSize = getContentSize();
    if(contentSize.height != targetHeight)
    {
        setContentSize(Size(contentSize.width, targetHeight));
        _contentTileGrid->setContentSize(Size(_contentGridSize.width, MIN(targetHeight - _closedHeight - (2 * kTileSpacing), _contentGridSize.height)));
    }
    Super::onEnter();
}

void DropdownContentHolder::onExit()
{
    _iconDownloader->setDelegate(nullptr);
    stopAllActions();
    _iconLayout->stopAllActions();
    _resizing = false;
    Super::onExit();
}

void DropdownContentHolder::onSizeChanged()
{
    Super::onSizeChanged();
    
    const Size& contentSize = getContentSize();
    const Size& bgSize = contentSize - Size(12,12);
    const Size& bgTexSize = _bgPattern->getTexture()->getContentSizeInPixels();
    const Vec2& scales = Vec2(bgSize.width / bgTexSize.width, bgSize.height / bgTexSize.height) / 2.0f;
    _bgPattern->setTextureRect(Rect(Vec2(0,0), Size(bgTexSize.width * scales.x, bgTexSize.height * scales.y)));
    _bgPattern->setContentSize(bgSize);
    _titleBanner->setContentSize(Size(contentSize.width, 2 * kBgCapInsets.origin.y));
    _categoryTitle->setTextAreaSize(Size(_titleBanner->getContentSize().width * 0.5f, _categoryTitle->getContentSize().height));
    _iconLayout->setContentSize(Size(_titleBanner->getContentSize().height - 12.0f, _titleBanner->getContentSize().height - 12.0f));
    _iconLayout->setPosition(Vec2(_iconLayout->getContentSize().width, 8.0f));
    _iconStencil->setContentSize(_iconLayout->getContentSize());
    _iconClippingNode->setContentSize(_iconLayout->getContentSize());
    _categoryIcon->setScale(MIN((_iconLayout->getContentSize().height * 0.9f) / _categoryIcon->getContentSize().height, (_iconLayout->getContentSize().width * 0.9f) / _categoryIcon->getContentSize().width));
    
    resizeContent();
    
    _closedHeight = _titleBanner->getContentSize().height;
    _openHeight = _titleBanner->getContentSize().height + _contentGridSize.height + (2 * kTileSpacing);
    
    if(!_resizing)
    {
        const float targetHeight = _open ? _openHeight : _closedHeight;
        if(contentSize.height != targetHeight)
        {
            setContentSize(Size(contentSize.width, targetHeight));
            _contentTileGrid->setContentSize(Size(_contentGridSize.width, MIN(targetHeight - _closedHeight - (2 * kTileSpacing), _contentGridSize.height)));
        }
    }
}

void DropdownContentHolder::setPatternColour(const cocos2d::Color3B& colour)
{
    _bgPattern->setColor(colour);
}

void DropdownContentHolder::setFrameColour(const cocos2d::Color3B& colour)
{
    setBackGroundImageColor(colour);
    _iconBackground->setBackGroundColor(colour);
}

void DropdownContentHolder::setOnResizeCallback(const OnResizeCallback& callback)
{
    _resizeCallback = callback;
}

bool DropdownContentHolder::isOpen() const
{
    return _open;
}

bool DropdownContentHolder::isResizing() const
{
    return _resizing;
}

void DropdownContentHolder::toggleOpened(bool open)
{
    _open = open;
    
    const Size& currentSize = getContentSize();
    const float targetHeight = _open ? _openHeight : _closedHeight;
    const float dist = abs(currentSize.height - targetHeight);
    const float maxDist = _openHeight - _closedHeight;
    const float durationMod = dist / maxDist;
    const float targetTime = 1.0f;
    const float targetScale = _open ? kDropdownOpenIconScale : 1.0f;
    
    if(_open)
    {
        setContentLayoutVisible(true);
    }
    
    ActionFloat* resizeAction = ActionFloat::create(targetTime * durationMod, currentSize.height, targetHeight, [this](float height){
        this->setContentSize(Size(getContentSize().width, height));
        _contentTileGrid->setContentSize(Size(_contentGridSize.width, MIN(height - _closedHeight - (2 * kTileSpacing),_contentGridSize.height)));
        if(_resizeCallback)
        {
            _resizeCallback();
        }
    });
    _resizing = true;
    stopAllActions();
    runAction(Sequence::createWithTwoActions(resizeAction, CallFunc::create([this](){
        _resizing = false;
        if(!_open)
        {
            setContentLayoutVisible(false);
        }
    })));
    _iconLayout->stopAllActions();
    _iconLayout->runAction(ScaleTo::create(targetTime * durationMod, targetScale));
}

void DropdownContentHolder::createTitleLayout()
{
    _titleBanner = ui::Layout::create();
    _titleBanner->setContentSize(Size(getContentSize().width, 2 * kBgCapInsets.origin.y));
    _titleBanner->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
    _contentLayout->addChild(_titleBanner);
    
    _categoryTitle = DynamicText::create("", Style::Font::PoppinsBold(), 75);
    _categoryTitle->setTextVerticalAlignment(TextVAlignment::CENTER);
    _categoryTitle->setTextHorizontalAlignment(TextHAlignment::CENTER);
    _categoryTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _categoryTitle->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _categoryTitle->setOverflow(Label::Overflow::SHRINK);
    _categoryTitle->setTextAreaSize(Size(_titleBanner->getContentSize().width * 0.6f, _categoryTitle->getContentSize().height));
    _categoryTitle->setTextColor(Color4B::WHITE);
    _titleBanner->addChild(_categoryTitle);
    
    _iconLayout = ui::Layout::create();
    _iconLayout->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    _iconLayout->setContentSize(Size(_titleBanner->getContentSize().height - 12.0f, _titleBanner->getContentSize().height - 12.0f));
    _iconLayout->setPosition(Vec2(_iconLayout->getContentSize().width, 8.0f));
    _titleBanner->addChild(_iconLayout);
    
    _iconStencil = Sprite::create("res/hqscene/circle.png");
    _iconStencil->setContentSize(_iconLayout->getContentSize());
    _iconStencil->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    
    _iconClippingNode = ClippingNode::create(_iconStencil);
    _iconClippingNode->setContentSize(_iconLayout->getContentSize());
    _iconClippingNode->setAlphaThreshold(0.5f);
    _iconLayout->addChild(_iconClippingNode);
    
    _iconBackground = ui::Layout::create();
    _iconBackground->setBackGroundColorType(BackGroundColorType::SOLID);
    _iconBackground->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _iconBackground->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _iconBackground->setSizeType(SizeType::PERCENT);
    _iconBackground->setSizePercent(Vec2(1.0f, 1.0f));
    _iconClippingNode->addChild(_iconBackground);
    
    _categoryIcon = Sprite::create("res/hqscene/Charecter place holder 1.png");
    _categoryIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _categoryIcon->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _categoryIcon->setScale(MIN((_iconLayout->getContentSize().height * 0.9f) / _categoryIcon->getContentSize().height, (_iconLayout->getContentSize().width * 0.9f) / _categoryIcon->getContentSize().width));
    _iconClippingNode->addChild(_categoryIcon);
}

void DropdownContentHolder::createContentLayout()
{
    _contentTileGrid = ui::Layout::create();
    _contentTileGrid->setContentSize(Size(getContentSize().width, 0));
    _contentTileGrid->setLayoutType(Type::VERTICAL);
    _contentTileGrid->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
    _contentTileGrid->setClippingEnabled(true);
    _contentLayout->addChild(_contentTileGrid);
}

void DropdownContentHolder::setContentItemData(const HQCarouselObjectRef& contentData)
{
    _contentData = contentData;
    updateContent();
}

void DropdownContentHolder::updateContent()
{
    if(_contentData)
    {
        _contentTiles.clear();
        _contentRows.clear();
        _contentTileGrid->removeAllChildren();
        
        //_iconDownloader->downloadImage(this, _contentData->getIcon());
        
        _categoryTitle->setString(_contentData->getTitle());
        const auto& contentList = _contentData->getContentItems();
        
        const Size& contentSize = getContentSize();
        
        const float tilesPerRow = 2.0f;
        
        const float rowWidth = contentSize.width - ((tilesPerRow + 1) * kTileSpacing);
        const float tileWidth = (rowWidth / tilesPerRow) - kTileSpacing;
        
        const Size& tileSize = Size(tileWidth * kTileAspectRatio.x, tileWidth * kTileAspectRatio.y); //4:3 aspect ratio tiles
        
        const int rows = ceil(contentList.size() / tilesPerRow);
        
        float totalHeight = (tileSize.height * rows) + (kTileSpacing * (rows + 1));
        for(int row = 0; row < rows; row++)
        {
            ui::Layout* rowContainer = ui::Layout::create();
            rowContainer->setContentSize(Size(rowWidth, tileSize.height));
            rowContainer->setLayoutType(Type::HORIZONTAL);
            rowContainer->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,kTileSpacing,0,0)));
            _contentTileGrid->addChild(rowContainer);
            _contentRows.pushBack(rowContainer);
            for(int col = 0; col < tilesPerRow; col++)
            {
                if((row * tilesPerRow) + col < contentList.size())
                {
                    RoundedRectTile* tile = RoundedRectTile::create();
                    tile->setContentSize(tileSize);
                    tile->setLayoutParameter(CreateCenterVerticalLinearLayoutParam(ui::Margin(kTileSpacing / 2.0f, 0, kTileSpacing / 2.0f, 0)));
                    tile->setContentSelectedCallback([this, row, tilesPerRow, col](HQContentItemObjectRef content){
                        if(_callback)
                        {
                            _callback(content, (row * tilesPerRow) + col);
                        }
                    });
                    tile->setTouchEnabled(false);
                    rowContainer->addChild(tile);
                    _contentTiles.pushBack(tile);
                }
            }
        }
        _contentGridSize = Size(rowWidth, totalHeight);
        _contentTileGrid->setContentSize(_contentGridSize);
        
        if(_contentData)
        {
            const auto& content = _contentData->getContentItems();
            for(int i = 0; i < MIN(_contentTiles.size(), content.size()); i++)
            {
                RoundedRectTile* tile = _contentTiles.at(i);
                tile->setContentItemData(content.at(i));
                tile->setTouchEnabled(true);
            }
        }
    }
}

void DropdownContentHolder::resizeContent()
{
    if(_contentData)
    {
        const auto& contentList = _contentData->getContentItems();
        const Size& contentSize = getContentSize();
        
        const float tilesPerRow = 2.0f;
        
        const float rowWidth = contentSize.width - ((tilesPerRow + 1) * kTileSpacing);
        const float tileWidth = (rowWidth / tilesPerRow) - kTileSpacing;
        
        const Size& tileSize = Size(tileWidth * kTileAspectRatio.x, tileWidth * kTileAspectRatio.y); //4:3 aspect ratio tiles
        
        const int rows = ceil(contentList.size() / tilesPerRow);
        
        float totalHeight = (tileSize.height * rows) + (kTileSpacing * (rows + 1));
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
        _contentTileGrid->forceDoLayout();
    }
}

void DropdownContentHolder::setContentLayoutVisible(bool visible)
{
    _contentTileGrid->setVisible(visible);
    for(auto tile : _contentTiles)
    {
        if(_visible)
        {
            tile->startCheckingForOnScreenPosition(tile);
        }
        else
        {
            tile->stopCheckingOnScreenPosition();
        }
    }
}

// delegate functions
void DropdownContentHolder::onImageDownloadComplete(const ImageDownloaderRef& downloader)
{
    _categoryIcon->setTexture(downloader->getLocalImagePath());
    _categoryIcon->setScale(MIN((_iconLayout->getContentSize().height * 0.9f) / _categoryIcon->getContentSize().height, (_iconLayout->getContentSize().width * 0.9f) / _categoryIcon->getContentSize().width));
}

void DropdownContentHolder::onImageDownloadFailed()
{
    
}

NS_AZOOMEE_END
