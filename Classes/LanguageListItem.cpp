//
//  LanguageListItem.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 01/10/2018.
//

#include "LanguageListItem.h"
#include <AzoomeeCommon/UI/Colour.h>
#include "Style.h"

using namespace cocos2d;

USING_NS_TZ

NS_AZ_BEGIN

bool LanguageListItem::init()
{
	if(!Super::init())
	{
		return false;
	}
	
	setBackGroundColorType(BackGroundColorType::SOLID);
	setBackGroundColor(Color3B::WHITE);
	
	return true;
}

void LanguageListItem::onEnter()
{
	const Size& contentSize = this->getContentSize();
	
	_flag = ui::ImageView::create(_flagFilename);
	_flag->setNormalizedPosition(Vec2(0,0.5));
	_flag->setAnchorPoint(Vec2(-0.5,0.5));
	_flag->setContentSize(Size(contentSize.height * 0.65f, contentSize.height * 0.65f));
	_flag->ignoreContentAdaptWithSize(false);
	this->addChild(_flag);
	
	_languageText = Label::createWithSystemFont(_language, Style::Font::Medium(), 91);
	_languageText->setTextColor(Color4B(Colours::Color_3B::battleshipGrey));
	_languageText->setPosition(Vec2(_flag->getContentSize().width * 2.0f, contentSize.height / 2.0f));
	_languageText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	this->addChild(_languageText);
	
	_radialSelect = ui::ImageView::create(_selected ? "res/settings/tick_button.png" : "res/settings/tick_box_empty.png");
	_radialSelect->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_RIGHT);
	_radialSelect->setAnchorPoint(Vec2(1.5,0.5));
	this->addChild(_radialSelect);
	
	
	Super::onEnter();
}

void LanguageListItem::setLanguage(const std::string &text)
{
	_language = text;
}

void LanguageListItem::setFlagImage(const std::string &imageFilename)
{
	_flagFilename = imageFilename;
}

void LanguageListItem::setSelected(bool selected)
{
	_selected = selected;
	if(_radialSelect)
	{
		_radialSelect->loadTexture(_selected ? "res/settings/tick_button.png" : "res/settings/tick_box_empty.png");
	}
}


NS_AZ_END
