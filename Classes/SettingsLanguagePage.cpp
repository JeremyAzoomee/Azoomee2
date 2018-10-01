//
//  SettingsLanguagePage.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 01/10/2018.
//

#include "SettingsLanguagePage.h"
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include <AzoomeeCommon/UI/LayoutParams.h>
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/Strings.h>
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include <AzoomeeCommon/UI/ModalMessages.h>
#include <AzoomeeCommon/UI/MessageBox.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool SettingsLanguagePage::init()
{
	if(!Super::init())
	{
		return false;
	}
	
	this->setLayoutType(ui::Layout::Type::VERTICAL);
	
	return true;
}

void SettingsLanguagePage::onEnter()
{
	_languageList = ui::ListView::create();
	_languageList->setDirection(ui::ScrollView::Direction::VERTICAL);
	_languageList->setBounceEnabled(true);
	_languageList->setContentSize(this->getContentSize());
	_languageList->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
	_languageList->setItemsMargin(10);
	_languageList->setTopPadding(0);
	_languageList->setBottomPadding(0);
	this->addChild(_languageList);
	
	
	
	Super::onEnter();
}

void SettingsLanguagePage::onExit()
{
	Super::onExit();
}
NS_AZOOMEE_END

