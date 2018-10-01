//
//  LanguageListItem.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 01/10/2018.
//

#include "LanguageListItem.h"
#include <AzoomeeCommon/UI/Style.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool LanguageListItem::init()
{
	if(!Super::init())
	{
		return false;
	}
	
	_flag = ui::ImageView::create();
	_flag->setNormalizedPosition(Vec2(0,0.5));
	_flag->setAnchorPoint(Vec2(-0.5,0.5));
	_flag->setContentSize(Size(this->getContentSize().height * 0.65f, this->getContentSize().height * 0.65f));
	_flag->ignoreContentAdaptWithSize(false);
	this->addChild(_flag);
	
	_languageText = Label::createWithTTF("", Style::Font::Medium, 91);
	_languageText->setTextColor(Color4B(Style::Color::battleshipGrey));
	_languageText->setPosition(Vec2(_flag->getContentSize().width * 2.0f, this->getContentSize().height));
	_languageText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	this->addChild(_languageText);
	
	return true;
}

void LanguageListItem::onEnter()
{
	_flag->loadTexture(_flagFilename);
	_languageText->setString(_language);
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


NS_AZOOMEE_END
