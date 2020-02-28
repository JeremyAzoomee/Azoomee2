//
//  OomeeStoreButton.cpp
//  Azoomee
//
//  Created by Macauley.Scoffins on 11/10/2019.
//

#include "OomeeStoreButton.h"
#include "SceneManagerScene.h"
#include <TinizineCommon/UI/Colour.h>
#include "HQConstants.h"

using namespace cocos2d;

USING_NS_TZ

NS_AZ_BEGIN

const cocos2d::Size OomeeStoreButton::kOverflowPadding = Size(0, 30);
const cocos2d::Size OomeeStoreButton::kFramePadding = Size(10,10);

bool OomeeStoreButton::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    ignoreContentAdaptWithSize(false);
    
    _frame = RoundedRectSprite::create();
    _frame->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _frame->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _frame->setColor(Colours::Color_3B::macaroniAndCheese);
    _frame->setTexture("res/decoration/white_1px.png");
    _frame->setCornerRadius(HQConsts::OomeeHQTileCornerRadius);
    addChild(_frame);
    
    _background = RoundedRectSprite::create();
    _background->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _background->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _background->setTexture("res/OomeeHQ/OomeeStore/background.png");
    _background->setCornerRadius(HQConsts::OomeeHQTileCornerRadius);
    _frame->addChild(_background);
    
    _foreground = RoundedRectSprite::create();
    _foreground->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _foreground->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _foreground->setTexture("res/OomeeHQ/OomeeStore/foreground.png");
    _foreground->setCornerRadius(HQConsts::OomeeHQTileCornerRadius);
    _background->addChild(_foreground);
    
    return true;
}

void OomeeStoreButton::onSizeChanged()
{
    Super::onSizeChanged();
    
    const Size& contentSize = getContentSize();
    _frame->setContentSize(contentSize - kOverflowPadding);
    _background->setContentSize(contentSize - (kFramePadding + kOverflowPadding));
    _foreground->setContentSize(Size(MIN(_foreground->getTexture()->getPixelsWide() * (contentSize.height / _foreground->getTexture()->getPixelsHigh()), contentSize.width) - kFramePadding.width,contentSize.height - (kFramePadding.height / 2.0f)));
    
}


NS_AZ_END
