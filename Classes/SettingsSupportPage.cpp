//
//  SettingsSupportPage.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 23/08/2018.
//

#include "SettingsSupportPage.h"
#include <AzoomeeCommon/Data/ConfigStorage.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool SettingsSupportPage::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    this->setLayoutType(ui::Layout::Type::VERTICAL);
    this->setSizeType(ui::Widget::SizeType::PERCENT);
    this->setSizePercent(Vec2(1.0f,1.0f));
    
    return true;
}

void SettingsSupportPage::onEnter()
{
    _headerBanner = SettingsPageHeader::create();
    _headerBanner->setContentSize(Size(this->getContentSize().width, 316));
    _headerBanner->setText("Version Number " + ConfigStorage::getInstance()->getVersionNumberToDisplay());
    this->addChild(_headerBanner);
    
    Super::onEnter();
}

NS_AZOOMEE_END
