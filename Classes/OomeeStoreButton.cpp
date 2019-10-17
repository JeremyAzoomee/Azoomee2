//
//  OomeeStoreButton.cpp
//  Azoomee
//
//  Created by Macauley.Scoffins on 11/10/2019.
//

#include "OomeeStoreButton.h"
#include "SceneManagerScene.h"
#include <AzoomeeCommon/UI/Style.h>
#include "HQConstants.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

const cocos2d::Size OomeeStoreButton::kOverflowPadding = Size(0, 25);
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
    _frame->setColor(Style::Color::macaroniAndCheese);
    _frame->setTexture("res/decoration/white_1px.png");
    _frame->setCornerRadius(HQConsts::OomeeHQTileCornerRadius);
    addChild(_frame);
    
    _background = RoundedRectSprite::create();
    _background->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _background->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _background->setTexture("res/OomeeHQ/OomeeStore/background.png");
    _background->setCornerRadius(HQConsts::OomeeHQTileCornerRadius);
    _frame->addChild(_background);
    
    _oomees = Sprite::create("res/OomeeHQ/OomeeStore/charecters.png");
    _oomees->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _oomees->setNormalizedPosition(Vec2(0.33f, 0.0f));
    _background->addChild(_oomees);
    
    _logo = Sprite::create("res/OomeeHQ/OomeeStore/oomee_store_blue_logo.png");
    _logo->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _logo->setNormalizedPosition(Vec2(0.75f, 0.5f));
    _background->addChild(_logo);
    
    return true;
}

void OomeeStoreButton::onSizeChanged()
{
    Super::onSizeChanged();
    
    const Size& contentSize = getContentSize();
    _frame->setContentSize(contentSize - kOverflowPadding);
    _background->setContentSize(contentSize - (kFramePadding + kOverflowPadding));
    _oomees->setScale((contentSize.height - (kFramePadding.height / 2.0f)) / _oomees->getContentSize().height);
    _logo->setScale((contentSize.height * 0.5f) / _logo->getContentSize().height);
    
}


NS_AZOOMEE_END
