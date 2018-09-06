//
//  SettingsKidsPage.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 06/09/2018.
//

#include "SettingsKidsPage.h"
#include <AzoomeeCommon/UI/LayoutParams.h>
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/Data/Parent/ParentDataProvider.h>
#include "KidDetailsLayer.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool SettingsKidsPage::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    setLayoutType(ui::Layout::Type::VERTICAL);
    
    return true;
}

void SettingsKidsPage::onEnter()
{
    _headerBanner = SettingsPageHeader::create();
    _headerBanner->setContentSize(Size(this->getContentSize().width, 316));
    _headerBanner->setText("Every child has a unique Kid Code. Share it with your friends and family to invite them to safely connect on Azoomee Chat.");
    _headerBanner->setLayoutParameter(CreateTopLinearLayoutParam());
    this->addChild(_headerBanner);
    
    _kidList = ui::ListView::create();
    _kidList->setDirection(ui::ScrollView::Direction::VERTICAL);
    _kidList->setBounceEnabled(true);
    _kidList->setContentSize(Size(this->getContentSize().width - 100, this->getContentSize().height - 456));
    _kidList->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
    _kidList->setItemsMargin(50);
    _kidList->setTopPadding(50);
    _kidList->setBottomPadding(50);
    this->addChild(_kidList);
    
    for(int i = 0; i < ParentDataProvider::getInstance()->getAmountOfAvailableChildren(); i++)
    {
        KidDetailsLayer* kidLayer = KidDetailsLayer::create();
        kidLayer->setContentSize(Size(_kidList->getContentSize().width, 1650));
        kidLayer->setChildNum(i);
        _kidList->addChild(kidLayer);
    }
    
    _footerBanner = ui::Layout::create();
    _footerBanner->setContentSize(Size(this->getContentSize().width, 150));
    _footerBanner->setBackGroundColor(Style::Color::skyBlue);
    _footerBanner->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    this->addChild(_footerBanner);
    
    ui::ImageView* newProfileImg = ui::ImageView::create("res/settings/add_new_profile_button.png");
    newProfileImg->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    newProfileImg->setNormalizedPosition(Vec2::ANCHOR_BOTTOM_LEFT);
    
    Label* newProfileText = Label::createWithTTF("Add New Profile", Style::Font::Medium, 75);
    newProfileText->setTextColor(Color4B::WHITE);
    newProfileText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    newProfileText->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_RIGHT);
    
    ui::Layout* centerLayout = ui::Layout::create();
    centerLayout->setContentSize(Size(newProfileImg->getContentSize().width + newProfileText->getContentSize().width + 30,_footerBanner->getContentSize().height));
    centerLayout->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    centerLayout->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
    centerLayout->addChild(newProfileText);
    centerLayout->addChild(newProfileImg);
    
    _footerBanner->addChild(centerLayout);
    
    Super::onEnter();
}

NS_AZOOMEE_END
