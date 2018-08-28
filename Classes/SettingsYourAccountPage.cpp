//
//  SettingsYourAccountPage.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 28/08/2018.
//

#include "SettingsYourAccountPage.h"
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include <AzoomeeCommon/UI/LayoutParams.h>
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/Strings.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool SettingsYourAccountPage::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    this->setLayoutType(ui::Layout::Type::VERTICAL);
    
    return true;
}

void SettingsYourAccountPage::onEnter()
{
    _headerBanner = SettingsPageHeader::create();
    _headerBanner->setContentSize(Size(this->getContentSize().width, 316));
    _headerBanner->setText("Learn more about managing your account… adipiscing elit. consectetur adipiscing elit.");
    _headerBanner->setLayoutParameter(CreateTopLinearLayoutParam());
    this->addChild(_headerBanner);
    
    _editAccountLayer = EditAccountLayer::create();
    _editAccountLayer->setContentSize(Size(this->getContentSize().width - 100, this->getContentSize().height * 0.5f));
    _editAccountLayer->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,50,0,0)));
    this->addChild(_editAccountLayer);
    
    Super::onEnter();
}

NS_AZOOMEE_END

