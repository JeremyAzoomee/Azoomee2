//
//  AddFriendTile.cpp
//  Azoomee
//
//  Created by Macauley.Scoffins on 24/10/2019.
//

#include "AddFriendTile.h"
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/Utils/LocaleManager.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

const float AddFriendTile::kHeightScale = 1.5f;
const float AddFriendTile::kFrameThickness = 10.0f;

bool AddFriendTile::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    _frame = ui::ImageView::create("res/hqscene/circle.png");
    _frame->ignoreContentAdaptWithSize(false);
    _frame->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_TOP);
    _frame->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _frame->setColor(Color3B::WHITE);
    addChild(_frame);
    
    _clippingStencil = Sprite::create("res/hqscene/circle.png");
    _clippingStencil->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    
    _contentClipper = ClippingNode::create(_clippingStencil);
    _contentClipper->setAlphaThreshold(0.5f);
    addChild(_contentClipper);
    
    const Color3B& bgColour = Style::Color::darkIndigoThree;
    
    _bgColour = ui::ImageView::create("res/hqscene/circle.png");
    _bgColour->ignoreContentAdaptWithSize(false);
    _bgColour->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_TOP);
    _bgColour->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _bgColour->setColor(bgColour);
    _contentClipper->addChild(_bgColour);
    
    _pattern = ui::ImageView::create("res/decoration/main_pattern_small.png");
    _pattern->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _pattern->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _pattern->setColor(Color3B::WHITE);
    _pattern->setScale(0.4f);
    _bgColour->addChild(_pattern);
    
    _circleGradient = LayerRadialGradient::create();
    _circleGradient->setStartColor(bgColour);
    _circleGradient->setStartOpacity(0);
    _circleGradient->setEndColor(bgColour);
    _circleGradient->setEndOpacity(255);
    _bgColour->addChild(_circleGradient);
    
    _plusIcon = ui::ImageView::create("res/hqscene/add_friend_icon.png");
    _plusIcon->ignoreContentAdaptWithSize(false);
    _plusIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _plusIcon->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _bgColour->addChild(_plusIcon);
    
    _text = DynamicText::create(_("Add Friend"), Style::Font::PoppinsBold(), 55);
    _text->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _text->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _text->setTextHorizontalAlignment(TextHAlignment::CENTER);
    _text->setTextVerticalAlignment(TextVAlignment::BOTTOM);
    _text->setColor(Color3B::WHITE);
    _text->setOverflow(Label::Overflow::SHRINK);
    addChild(_text);
    
    setTouchEnabled(true);
    addTouchEventListener([this](Ref* pSender, ui::Widget::TouchEventType eType){
        if(eType == ui::Widget::TouchEventType::ENDED)
        {
            if(_selectedCallback)
            {
                _selectedCallback();
            }
        }
    });
    
    return true;
}

void AddFriendTile::onEnter()
{
    Super::onEnter();
}

void AddFriendTile::onExit()
{
    Super::onExit();
}

void AddFriendTile::onSizeChanged()
{
    Super::onSizeChanged();
    resizeContent();
}

void AddFriendTile::setTileWidth(float width)
{
    setContentSize(Size(width, width * kHeightScale));
}

void AddFriendTile::setSelectedCallback(const SelectedCallback &callback)
{
    _selectedCallback = callback;
}

void AddFriendTile::resizeContent()
{
    const Size& contentSize = getContentSize();
    _frame->setContentSize(Size(contentSize.width, contentSize.width));
    const Size& clipperSize = Size(contentSize.width - kFrameThickness, contentSize.width - kFrameThickness);
    _contentClipper->setContentSize(contentSize);
    _clippingStencil->setContentSize(clipperSize);
    _clippingStencil->setPosition(Vec2(contentSize.width / 2.0f, contentSize.height - (kFrameThickness / 2.0f)));
    _bgColour->setContentSize(clipperSize);
    _bgColour->setPosition(Vec2(contentSize.width / 2.0f, contentSize.height - (kFrameThickness / 2.0f)));
    _circleGradient->setContentSize(clipperSize);
    _circleGradient->setCenter(Vec2(clipperSize / 2.0f));
    _circleGradient->setRadius(clipperSize.height / 2.0f);
    _plusIcon->setContentSize(clipperSize);
    _text->setTextAreaSize(Size(contentSize.width, _text->getContentSize().height));
}


NS_AZOOMEE_END
