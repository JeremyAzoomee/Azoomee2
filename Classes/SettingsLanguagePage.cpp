//
//  SettingsLanguagePage.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 01/10/2018.
//

#include "SettingsLanguagePage.h"
#include <AzoomeeCommon/UI/LayoutParams.h>
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/Utils/LocaleManager.h>
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include <AzoomeeCommon/UI/ModalMessages.h>
#include "LanguageListItem.h"
#include "HQHistoryManager.h"

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
	
	for(auto data : LocaleManager::kLanguageParams)
	{
		LanguageListItem* listItem = LanguageListItem::create();
		listItem->setLanguage(data._name);
		listItem->setFlagImage(StringUtils::format("res/settings/flag_%s.png",data._identifier.c_str()));
		listItem->setContentSize(Size(this->getContentSize().width - 10, 245));
		listItem->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
		listItem->setTouchEnabled(true);
		listItem->addTouchEventListener([&, data](Ref* pSender, ui::Widget::TouchEventType eType){
			if(eType == ui::Widget::TouchEventType::ENDED)
			{
				for(auto item : _languageList->getChildren())
				{
					LanguageListItem* langItem = dynamic_cast<LanguageListItem*>(item);
					if(langItem)
					{
						langItem->setSelected(false);
					}
				}
				LanguageListItem* selectedItem = dynamic_cast<LanguageListItem*>(pSender);
				if(selectedItem)
				{
					selectedItem->setSelected(true);
				}
				LocaleManager::getInstance()->changeLanguage(data._identifier);
                HQHistoryManager::getInstance()->clearCachedHQData();
			}
		});
		if(data._identifier == LocaleManager::getInstance()->getLanguageID())
		{
			listItem->setSelected(true);
		}
		_languageList->addChild(listItem);
	}
	
	Super::onEnter();
}

void SettingsLanguagePage::onExit()
{
	Super::onExit();
}
NS_AZOOMEE_END

