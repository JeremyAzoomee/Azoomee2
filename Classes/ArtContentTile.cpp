//
//  ArtContentTile.cpp
//  Azoomee
//
//  Created by Macauley.Scoffins on 14/10/2019.
//

#include "ArtContentTile.h"
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/UI/LayoutParams.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

const Size ArtContentTile::kDropshadowPadding = Size(76,76);

bool ArtContentTile::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    addTouchEventListener([this](Ref* pSender, ui::Widget::TouchEventType eType){
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            if(_newArtTile)
            {
                if(_editCallback)
                {
                    _editCallback("");
                }
            }
            else
            {
                this->toggleSelectedMode(!_selectedModeEnabled);
            }
        }
    });
    
    _dropShadow = ui::ImageView::create("res/hqscene/DropDownBoxStencil.png");
    _dropShadow->setContentSize(getContentSize() + kDropshadowPadding);
    _dropShadow->setScale9Enabled(true);
    _dropShadow->ignoreContentAdaptWithSize(false);
    _dropShadow->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _dropShadow->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    addChild(_dropShadow);
    
    _stencil = ui::Scale9Sprite::create("res/hqscene/DropDownBoxStencil.png");
    _stencil->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
    _imageClipper = ClippingNode::create(_stencil);
    _imageClipper->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    _imageClipper->setAlphaThreshold(0.9f);
    addChild(_imageClipper);
    
    _contentImage = Sprite::create();
    _contentImage->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _contentImage->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _imageClipper->addChild(_contentImage);
    
    _buttonOverlay = RoundedRectSprite::create();
    _buttonOverlay->setTexture("res/decoration/white_1px.png");
    _buttonOverlay->setCornerRadius(60);
    _buttonOverlay->setColor(Style::Color::purplyPink);
    _buttonOverlay->setOpacity(125);
    _buttonOverlay->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _buttonOverlay->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _buttonOverlay->setIgnoreAnchorPointForPosition(false);
    addChild(_buttonOverlay);
    
    _deleteButton = ui::Button::create();
    _deleteButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _deleteButton->setNormalizedPosition(Vec2(0.6,0.5f));
    _deleteButton->addTouchEventListener([this](Ref* pSender, ui::Widget::TouchEventType eType){
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            if(_deleteCallback)
            {
                _deleteCallback(_artFilename);
            }
        }
    });
    _buttonOverlay->addChild(_deleteButton);
    
    _editButton = ui::Button::create();
    _editButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _editButton->setNormalizedPosition(Vec2(0.4,0.5f));
    _editButton->addTouchEventListener([this](Ref* pSender, ui::Widget::TouchEventType eType){
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            if(_editCallback)
            {
                _editCallback(_artFilename);
            }
        }
    });
    _buttonOverlay->addChild(_editButton);
    
    return true;
}

void ArtContentTile::onEnter()
{
    startCheckingForOnScreenPosition(this);
    Super::onEnter();
}

void ArtContentTile::onExit()
{
    stopCheckingOnScreenPosition();
    Super::onExit();
}

void ArtContentTile::onSizeChanged()
{
    Super::onSizeChanged();
    
    const Size& contentSize = getContentSize();
    _dropShadow->setContentSize(contentSize + kDropshadowPadding);
    _imageClipper->setContentSize(contentSize);
    _stencil->setPosition(contentSize / 2.0f);
    _stencil->setContentSize(contentSize + kDropshadowPadding);
    _buttonOverlay->setContentSize(contentSize);
    _editButton->setContentSize(Size(contentSize.width * 0.2f, contentSize.width * 0.2f));
    _deleteButton->setContentSize(Size(contentSize.width * 0.2f, contentSize.width * 0.2f));
    resizeContentImage();
}

void ArtContentTile::setArtFilename(const std::string& artFilename)
{
    _artFilename = artFilename;
}

void ArtContentTile::setPlaceholderFilename(const std::string& placeholder)
{
    _placholderFilename = placeholder;
}

void ArtContentTile::setEditCallback(const ArtContentTileCallback& callback)
{
    _editCallback = callback;
}

void ArtContentTile::setDeleteCallback(const ArtContentTileCallback& callback)
{
    _deleteCallback = callback;
}

void ArtContentTile::toggleSelectedMode(bool enabled)
{
    _selectedModeEnabled = enabled;
    _buttonOverlay->setVisible(enabled);
}

void ArtContentTile::resizeContentImage()
{
    const Size& contentSize = getContentSize();
    const Size& imageSize = _contentImage->getContentSize();
    _contentImage->setScale(MIN(contentSize.height / imageSize.height, contentSize.width / imageSize.width));
}

void ArtContentTile::elementDisappeared(cocos2d::Node *sender)
{
    _contentImage->setTexture(_placholderFilename);
    resizeContentImage();
}

void ArtContentTile::elementAppeared(cocos2d::Node *sender)
{
    _contentImage->setTexture(_artFilename);
    resizeContentImage();
}

NS_AZOOMEE_END
