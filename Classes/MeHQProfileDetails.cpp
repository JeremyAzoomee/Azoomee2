//
//  MeHQProfileDetails.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 14/05/2018.
//

#include "MeHQProfileDetails.h"
#include <AzoomeeCommon/Data/Child/ChildDataProvider.h>
#include <AzoomeeCommon/Data/Parent/ParentDataProvider.h>
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/UI/LayoutParams.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool MeHQProfileDetails::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    this->setContentSize(Size(Director::getInstance()->getVisibleSize().width, 1000));
    setBackGroundColor(Color3B::RED);
    setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    setLayoutType(ui::Layout::Type::HORIZONTAL);
    
    _avatar = ui::ImageView::create("res/chat/ui/avatar/avatar_background.png");
    _avatar->setContentSize(this->getContentSize()/2);
    _avatar->setSizeType(ui::Widget::SizeType::ABSOLUTE);
    _avatar->ignoreContentAdaptWithSize(true);
    this->addChild(_avatar);
    
    _labelLayout = ui::Layout::create();
    _labelLayout->setSizeType(ui::Widget::SizeType::PERCENT);
    _labelLayout->setSizePercent(Vec2(0.5,1.0));
    _labelLayout->setLayoutType(ui::Layout::Type::VERTICAL);
    _labelLayout->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
    
    this->addChild(_labelLayout);
    
    _nameLabel = ui::Text::create(ChildDataProvider::getInstance()->getLoggedInChildName(),Style::Font::Regular , 100);
    _labelLayout->addChild(_nameLabel);
    
    _kidCodeLabel = ui::Text::create(ParentDataProvider::getInstance()->getInviteCodeForAnAvailableChild(ChildDataProvider::getInstance()->getLoggedInChildNumber()), Style::Font::Regular, 70);
    _labelLayout->addChild(_kidCodeLabel);
    
    _editOomeeButton = ui::Button::create("res/buttons/greenMainButton.png");
    _labelLayout->addChild(_editOomeeButton);
    
    return true;
}

void MeHQProfileDetails::onEnter()
{
    Super::onEnter();
}

void MeHQProfileDetails::onExit()
{
    Super::onExit();
}

void MeHQProfileDetails::onSizeChanged()
{
    Super::onSizeChanged();
}

NS_AZOOMEE_END
