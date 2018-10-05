//
//  SettingsSupportPage.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 23/08/2018.
//

#include "SettingsSupportPage.h"
#include <AzoomeeCommon/Strings.h>
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include <AzoomeeCommon/UI/LayoutParams.h>
#include <AzoomeeCommon/UI/Style.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool SettingsSupportPage::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    this->setLayoutType(ui::Layout::Type::VERTICAL);
    
    return true;
}

void SettingsSupportPage::onEnter()
{
    _headerBanner = SettingsPageHeader::create();
    _headerBanner->setContentSize(Size(this->getContentSize().width, 316));
    _headerBanner->setText(ConfigStorage::getInstance()->getVersionNumberToDisplay());
    _headerBanner->setLayoutParameter(CreateTopLinearLayoutParam());
    this->addChild(_headerBanner);
    
    _supportBox = ui::Layout::create();
    _supportBox->setContentSize(Size(this->getContentSize().width - 100, this->getContentSize().height * 0.45f));
    _supportBox->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,50,0,0)));
    _supportBox->setBackGroundImage("res/settings/rounded_rect.png");
    _supportBox->setBackGroundImageScale9Enabled(true);
    this->addChild(_supportBox);
    
    Label* text = Label::createWithTTF(_("Need some help?\n\n\nVisit our support page at support.azoomee.com\n\nOr\n\nContact us directly at help@azoomee.com"), Style::Font::Medium, 75);
    text->setWidth(_supportBox->getContentSize().width * 0.8f);
    text->setHorizontalAlignment(TextHAlignment::CENTER);
    text->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    text->setPosition(Vec2(_supportBox->getContentSize().width * 0.5f, _supportBox->getContentSize().height * 0.5f));
    text->setTextColor(Color4B::BLACK);
    _supportBox->addChild(text);
    
    Super::onEnter();
}

NS_AZOOMEE_END
