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

const cocos2d::Rect DropdownContentHolder::kBgCapInsets = Rect(162, 162, 952, 1894);

bool DropdownContentHolder::init()
{
    if(Super::init())
    {
        return false;
    }
    
    setBackGroundImage("res/hqscene/dropdown_bg.png");
    setBackGroundImageScale9Enabled(true);
    setBackGroundImageCapInsets(kBgCapInsets);
    
    _clippingStencil = ui::Scale9Sprite::create(kBgCapInsets, "res/hqscene/dropdown_bg.png");
    _clippingStencil->setContentSize(getContentSize());
    _clippingStencil->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    
    _bgClipper = ClippingNode::create(_clippingStencil);
    _bgClipper->setAlphaThreshold(0.2f);
    _bgClipper->setContentSize(getContentSize());
    addChild(_bgClipper);
    
    _bgPattern = ui::ImageView::create("res/decoration/main_pattern_big.png");
    _bgPattern->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _bgPattern->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_TOP);
    _bgClipper->addChild(_bgPattern);
    
    _contentLayout->setLayoutType(Type::VERTICAL);
    _contentLayout->setSizeType(SizeType::PERCENT);
    _contentLayout->setSizePercent(Vec2(1.0f, 1.0f));
    
    createTitleLayout();
    
    return true;
}

void DropdownContentHolder::onEnter()
{
    Super::onEnter();
}

void DropdownContentHolder::onExit()
{
    Super::onExit();
}

void DropdownContentHolder::onSizeChanged()
{
    Super::onSizeChanged();
    
    _clippingStencil->setContentSize(getContentSize());
    _bgClipper->setContentSize(getContentSize());
    _titleBanner->setContentSize(Size(getContentSize().width, 2 * kBgCapInsets.origin.y));
    _categoryTitle->setTextAreaSize(Size(_titleBanner->getContentSize().width, _categoryTitle->getContentSize().height));
    _categoryIcon->setContentSize(Size(_titleBanner->getContentSize().height, _titleBanner->getContentSize().height));
}

void DropdownContentHolder::setPatternColour(const cocos2d::Color3B& colour)
{
    _bgPattern->setColor(colour);
}

void DropdownContentHolder::setFrameColour(const cocos2d::Color3B& colour)
{
    setBackGroundImageColor(colour);
}

void DropdownContentHolder::createTitleLayout()
{
    _titleBanner = ui::Layout::create();
    _titleBanner->setContentSize(Size(getContentSize().width, 2 * kBgCapInsets.origin.y));
    _titleBanner->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _titleBanner->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_TOP);
    _contentLayout->addChild(_titleBanner);
    
    _categoryTitle = DynamicText::create("", Style::Font::PoppinsBold(), 75);
    _categoryTitle->setTextVerticalAlignment(TextVAlignment::CENTER);
    _categoryTitle->setTextHorizontalAlignment(TextHAlignment::CENTER);
    _categoryTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _categoryTitle->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _categoryTitle->setOverflow(Label::Overflow::SHRINK);
    _categoryTitle->setTextAreaSize(Size(_titleBanner->getContentSize().width, _categoryTitle->getContentSize().height));
    _categoryTitle->setTextColor(Color4B::WHITE);
    _titleBanner->addChild(_categoryTitle);
    
    _categoryIcon = ui::ImageView::create();
    _categoryIcon->ignoreContentAdaptWithSize(false);
    _categoryIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _categoryIcon->setContentSize(Size(_titleBanner->getContentSize().height, _titleBanner->getContentSize().height));
    _categoryIcon->setPosition(_categoryIcon->getContentSize() / 2.0f);
    _titleBanner->addChild(_categoryIcon);
}

void DropdownContentHolder::createContentLayout()
{
    
}

NS_AZOOMEE_END
