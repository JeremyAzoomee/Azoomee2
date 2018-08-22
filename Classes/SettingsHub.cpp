//
//  SettingsHub.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 22/08/2018.
//

#include "SettingsHub.h"
#include <AzoomeeCommon/UI/Style.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool SettingsHub::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    const Size& visibleSize = Director::getInstance()->getVisibleSize();
    
    this->setContentSize(visibleSize);
    
    _contentLayout = ui::Layout::create();
    _contentLayout->setContentSize(visibleSize);
    _contentLayout->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    _contentLayout->setBackGroundColor(Style::Color::greyBlue);
    _contentLayout->setBackGroundColorOpacity(50);
    _contentLayout->setLayoutType(ui::Layout::Type::VERTICAL);
    this->addChild(_contentLayout);
    
    _titleLayout = ui::Layout::create();
    _titleLayout->setContentSize(Size(visibleSize.width, 150));
    _titleLayout->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    _titleLayout->setBackGroundColor(Style::Color::skyBlue);
    _contentLayout->addChild(_titleLayout);
    
    _navigationLayout = ui::Layout::create();
    _navigationLayout->setContentSize(Size(visibleSize.width, visibleSize.height - _titleLayout->getContentSize().height));
    _contentLayout->addChild(_navigationLayout);
    
    
    return true;
}

void SettingsHub::onEnter()
{
    Super::onEnter();
}

void SettingsHub::onSizeChanged()
{
    
}

NS_AZOOMEE_END
