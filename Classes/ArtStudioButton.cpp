//
//  ArtStudioButton.cpp
//  Azoomee
//
//  Created by Macauley.Scoffins on 11/10/2019.
//

#include "ArtStudioButton.h"
#include "SceneManagerScene.h"
#include <AzoomeeCommon/UI/Style.h>
#include "HQConstants.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

const cocos2d::Size ArtStudioButton::kOverflowPadding = Size(0, 25);
const cocos2d::Size ArtStudioButton::kFramePadding = Size(10,10);

bool ArtStudioButton::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    ignoreContentAdaptWithSize(false);
    
    _frame = RoundedRectSprite::create();
    _frame->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _frame->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _frame->setColor(Style::Color::purplyPink);
    _frame->setTexture("res/decoration/white_1px.png");
    _frame->setCornerRadius(HQConsts::OomeeHQTileCornerRadius);
    addChild(_frame);
    
    _background = RoundedRectSprite::create();
    _background->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _background->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _background->setTexture("res/OomeeHQ/ArtStudio/background.png");
    _background->setCornerRadius(HQConsts::OomeeHQTileCornerRadius);
    _frame->addChild(_background);
    
    _tools = Sprite::create("res/OomeeHQ/ArtStudio/tools.png");
    _tools->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _tools->setNormalizedPosition(Vec2(0.35f, 0.0f));
    _background->addChild(_tools);
    
    _logo = Sprite::create("res/OomeeHQ/ArtStudio/art_studio_logo.png");
    _logo->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _logo->setNormalizedPosition(Vec2(0.75f, 0.5f));
    _background->addChild(_logo);
    
    _paint = RoundedRectSprite::create();
    _paint->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    _paint->setNormalizedPosition(Vec2::ANCHOR_TOP_RIGHT);
    _paint->setTexture("res/OomeeHQ/ArtStudio/paint_drop_red.png");
    _paint->setCornerRadius(HQConsts::OomeeHQTileCornerRadius);
    _paint->setRoundedCorners(false, false, false, true);
    _background->addChild(_paint);
    
    return true;
}

void ArtStudioButton::onSizeChanged()
{
    Super::onSizeChanged();
    
    const Size& contentSize = getContentSize();
    _frame->setContentSize(contentSize - kOverflowPadding);
    _background->setContentSize(contentSize - (kFramePadding + kOverflowPadding));
    _tools->setScale((contentSize.height - (kFramePadding.height / 2.0f)) / _tools->getContentSize().height);
    _logo->setScale((contentSize.height * 0.5f) / _logo->getContentSize().height);
    _paint->setContentSize(_paint->getTexture()->getContentSize() * _tools->getScale());
    
}


NS_AZOOMEE_END
