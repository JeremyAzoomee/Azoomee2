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
    
    auto avatarLayout = ui::Layout::create();
    avatarLayout->setSizeType(ui::Widget::SizeType::PERCENT);
    avatarLayout->setSizePercent(Vec2(0.5,1.0));
    this->addChild(avatarLayout);
    
    _avatar = ui::ImageView::create("res/chat/ui/avatar/avatar_background.png");
    _avatar->ignoreContentAdaptWithSize(false);
    _avatar->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _avatar->setNormalizedPosition(Vec2(0.5,0.5));
    //_avatar->setContentSize(Size(this->getContentSize().width /2, this->getContentSize().height / 3.0f));
    _avatar->setSizePercent(Vec2(0.9,0.9));
    _avatar->setSizeType(ui::Widget::SizeType::PERCENT);
    avatarLayout->addChild(_avatar);
    
    _labelLayout = ui::Layout::create();
    _labelLayout->setSizeType(ui::Widget::SizeType::PERCENT);
    _labelLayout->setSizePercent(Vec2(0.5,1.0));
    this->addChild(_labelLayout);
    
    _nameLabel = ui::Text::create(ChildDataProvider::getInstance()->getLoggedInChildName(),Style::Font::Regular , 100);
    _nameLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _nameLabel->setNormalizedPosition(Vec2(0.5,0.75));
    _nameLabel->setContentSize(Size(this->getContentSize().width /2, this->getContentSize().height / 3.0f));
    _labelLayout->addChild(_nameLabel);
    
    _kidCodeLabel = ui::Text::create(ParentDataProvider::getInstance()->getInviteCodeForAnAvailableChild(ChildDataProvider::getInstance()->getLoggedInChildNumber()), Style::Font::Regular, 70);
    _kidCodeLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _kidCodeLabel->setNormalizedPosition(Vec2(0.5,0.5));
    _kidCodeLabel->setContentSize(Size(this->getContentSize().width /2, this->getContentSize().height / 3.0f));
    _labelLayout->addChild(_kidCodeLabel);
    
    _editOomeeButton = ui::Button::create("res/buttons/greenMainButton.png");
    _editOomeeButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _editOomeeButton->setNormalizedPosition(Vec2(0.5,0.25));
    _editOomeeButton->setContentSize(Size(this->getContentSize().width /2, this->getContentSize().height / 3.0f));
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
