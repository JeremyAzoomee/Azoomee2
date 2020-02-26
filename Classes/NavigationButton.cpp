//
//  NavigationButton.cpp
//  Azoomee
//
//  Created by Macauley.Scoffins on 20/08/2019.
//

#include "NavigationButton.h"

using namespace cocos2d;

USING_NS_TZ

NS_AZ_BEGIN

const float NavigationButton::kFadeAnimDuration = 0.5f;

bool NavigationButton::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    setTouchEnabled(true);
    setContentSize(Size(144,144));
    setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
    _circle = ui::ImageView::create("res/navigation/outer_circle.png");
    _circle->ignoreContentAdaptWithSize(false);
    _circle->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _circle->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _circle->setSizeType(SizeType::PERCENT);
    _circle->setSizePercent(Vec2(1.2,1.2f));
    addChild(_circle);
    
    _activeIcon = ui::ImageView::create();
    _activeIcon->ignoreContentAdaptWithSize(false);
    _activeIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _activeIcon->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _activeIcon->setSizeType(SizeType::PERCENT);
    _activeIcon->setSizePercent(Vec2(1.2,1.2f));
    _activeIcon->setOpacity(0);
    addChild(_activeIcon);
    
    _inactiveIcon = ui::ImageView::create();
    _inactiveIcon->ignoreContentAdaptWithSize(false);
    _inactiveIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _inactiveIcon->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _inactiveIcon->setSizeType(SizeType::PERCENT);
    _inactiveIcon->setSizePercent(Vec2(1.0,1.0f));
    addChild(_inactiveIcon);
    
    return true;
}

void NavigationButton::setCircleColour(const cocos2d::Color3B& colour)
{
    _circle->setColor(colour);
}
void NavigationButton::setButtonIcons(const std::string& activeIcon, const std::string& inactiveIcon)
{
    _activeIcon->loadTexture(activeIcon);
    _inactiveIcon->loadTexture(inactiveIcon);
}

void NavigationButton::setActive(bool active)
{
    _activeIcon->runAction(active ? (FiniteTimeAction*)FadeIn::create(kFadeAnimDuration) : (FiniteTimeAction*)FadeOut::create(kFadeAnimDuration));
    _inactiveIcon->runAction(active ? (FiniteTimeAction*)FadeOut::create(kFadeAnimDuration) : (FiniteTimeAction*)FadeIn::create(kFadeAnimDuration));
    _circle->runAction(active ? (FiniteTimeAction*)FadeOut::create(kFadeAnimDuration) : (FiniteTimeAction*)FadeIn::create(kFadeAnimDuration));
}

NS_AZ_END
