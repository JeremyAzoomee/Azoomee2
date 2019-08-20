//
//  NavigationButton.cpp
//  Azoomee
//
//  Created by Macauley.Scoffins on 20/08/2019.
//

#include "NavigationButton.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool NavigationButton::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    setTouchEnabled(true);
    setContentSize(Size(200,200));
    setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
    _circle = ui::ImageView::create("res/navigation/outer_circle");
    _circle->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _circle->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _circle->setSizeType(SizeType::PERCENT);
    _circle->setSizePercent(Vec2(1.0,1.0f));
    addChild(_circle);
    
    _activeIcon = ui::ImageView::create();
    _activeIcon->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _activeIcon->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _activeIcon->setSizeType(SizeType::PERCENT);
    _activeIcon->setSizePercent(Vec2(1.0,1.0f));
    _activeIcon->setOpacity(0);
    addChild(_activeIcon);
    
    _inactiveIcon = ui::ImageView::create();
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
    _activeIcon->runAction(active ? (FiniteTimeAction*)FadeIn::create(0.5f) : (FiniteTimeAction*)FadeOut::create(0.5f));
    _inactiveIcon->runAction(active ? (FiniteTimeAction*)FadeOut::create(0.5f) : (FiniteTimeAction*)FadeIn::create(0.5f));
}

NS_AZOOMEE_END
