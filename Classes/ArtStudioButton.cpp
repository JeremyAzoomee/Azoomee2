//
//  ArtStudioButton.cpp
//  Azoomee
//
//  Created by Macauley.Scoffins on 11/10/2019.
//

#include "ArtStudioButton.h"
#include "SceneManagerScene.h"
#include <AzoomeeCommon/UI/Colour.h>
#include "HQConstants.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

const cocos2d::Size ArtStudioButton::kOverflowPadding = Size(0, 30);
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
    _frame->setColor(Colours::Color_3B::purplyPink);
    _frame->setTexture("res/decoration/white_1px.png");
    _frame->setCornerRadius(HQConsts::OomeeHQTileCornerRadius);
    addChild(_frame);
    
    _background = RoundedRectSprite::create();
    _background->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _background->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _background->setTexture("res/OomeeHQ/ArtStudio/background.png");
    _background->setCornerRadius(HQConsts::OomeeHQTileCornerRadius);
    _frame->addChild(_background);
    
    _foreground = RoundedRectSprite::create();
    _foreground->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _foreground->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _foreground->setTexture("res/OomeeHQ/ArtStudio/foreground.png");
    _foreground->setCornerRadius(HQConsts::OomeeHQTileCornerRadius);
    _background->addChild(_foreground);
    
    return true;
}

void ArtStudioButton::onSizeChanged()
{
    Super::onSizeChanged();
    
    const Size& contentSize = getContentSize();
    _frame->setContentSize(contentSize - kOverflowPadding);
    _background->setContentSize(contentSize - (kFramePadding + kOverflowPadding));
    _foreground->setContentSize(Size(MIN(_foreground->getTexture()->getPixelsWide() * (contentSize.height / _foreground->getTexture()->getPixelsHigh()), contentSize.width) - kFramePadding.width,contentSize.height - (kFramePadding.height / 2.0f)));
    
}


NS_AZOOMEE_END
