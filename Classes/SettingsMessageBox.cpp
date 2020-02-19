//
//  SettingsMessageBox.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 05/09/2018.
//

#include "SettingsMessageBox.h"
#include <AzoomeeCommon/UI/Colour.h>
#include <AzoomeeCommon/UI/LayoutParams.h>
#include "Style.h"

using namespace cocos2d;

USING_NS_TZ

NS_AZ_BEGIN

bool SettingsMessageBox::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    _visibleSize = Director::getInstance()->getVisibleSize();
    this->setContentSize(_visibleSize);
    
    _overlay = LayerColor::create(Colours::Color_4B::semiTransparentOverlay, _visibleSize.width, _visibleSize.height);
    this->addChild(_overlay);
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true); // set listener to block all touches to layers bellow
    listener->onTouchBegan = [=](Touch *touch, Event *event)
    {
        return true;
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener->clone(), _overlay);
    
    _messageBox = ui::Layout::create();
    _messageBox->setBackGroundImage("res/settings/rounded_rect.png");
    _messageBox->setBackGroundImageScale9Enabled(true);
    _messageBox->setContentSize(_visibleSize * 0.5f);
    _messageBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _messageBox->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    this->addChild(_messageBox);
    
    return true;
}


void SettingsMessageBox::onEnter()
{
    Super::onEnter();
}

void SettingsMessageBox::onExit()
{
    if(_delegate)
    {
        _delegate = nullptr;
    }
    Super::onExit();
}

void SettingsMessageBox::setDelegate(AZ::SettingsMessageBoxDelegate *delegate)
{
    _delegate = delegate;
}

void SettingsMessageBox::onButtonPressed(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType eType)
{
    if(eType == ui::Widget::TouchEventType::ENDED)
    {
        SettingsMessageBoxButton* button = dynamic_cast<SettingsMessageBoxButton*>(pSender);
        if(_delegate && button)
        {
            _delegate->onButtonPressed(this, button->getMessageBoxButtonType());
        }
    }
}

NS_AZ_END
