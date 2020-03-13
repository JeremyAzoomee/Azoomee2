//
//  OomeeMakerButton.cpp
//  Azoomee
//
//  Created by Macauley.Scoffins on 11/10/2019.
//

#include "OomeeMakerButton.h"
#include "SceneManagerScene.h"
#include <TinizineCommon/UI/Colour.h>
#include "HQConstants.h"

using namespace cocos2d;

USING_NS_TZ

NS_AZ_BEGIN

const cocos2d::Size OomeeMakerButton::kOverflowPadding = Size(0, 80);
const cocos2d::Size OomeeMakerButton::kFramePadding = Size(10,10);

bool OomeeMakerButton::init()
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
    _background->setTexture("res/OomeeHQ/OomeeMaker/background.png");
    _background->setCornerRadius(HQConsts::OomeeHQTileCornerRadius);
    _frame->addChild(_background);
    
    _oomees = RoundedRectSprite::create();
    _oomees->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _oomees->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _oomees->setTexture("res/OomeeHQ/OomeeMaker/oomees.png");
    _oomees->setCornerRadius(HQConsts::OomeeHQTileCornerRadius);
    _background->addChild(_oomees);
    
    return true;
}

void OomeeMakerButton::onSizeChanged()
{
    Super::onSizeChanged();
    
    const Size& contentSize = getContentSize();
    _frame->setContentSize(contentSize - kOverflowPadding);
    _background->setContentSize(contentSize - (kFramePadding + kOverflowPadding));
    _oomees->setContentSize(Size(MIN(_oomees->getTexture()->getPixelsWide() * (contentSize.height / _oomees->getTexture()->getPixelsHigh()), contentSize.width) - kFramePadding.width,contentSize.height - (kFramePadding.height / 2.0f)));
}

NS_AZ_END
