//
//  TextButton.cpp
//  AzoomeeCommon
//
//  Created by Macauley on 30/07/2019.
//

#include "TextButton.h"
#include "Colour.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

TextButton* TextButton::create(const std::string& normalImage)
{
	TextButton* button = TextButton::create();
	button->loadTextureNormal(normalImage);
	return button;
}

bool TextButton::init()
{
	if(!Super::init())
	{
		return false;
	}
	
	_buttonText = Label::createWithTTF("", "fonts/Marker Felt.ttf", 70);
	_buttonText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_buttonText->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	_buttonText->setOverflow(Label::Overflow::SHRINK);
	_buttonText->setHorizontalAlignment(TextHAlignment::CENTER);
	_buttonText->setVerticalAlignment(TextVAlignment::CENTER);
	_buttonText->setTextColor(Color4B::WHITE);
	_buttonText->setDimensions(getContentSize().width * _textSizePercent.x, getContentSize().height * _textSizePercent.y);
	addChild(_buttonText);
	
	return true;
}
void TextButton::onSizeChanged()
{
	Super::onSizeChanged();
	
	_buttonText->setDimensions(getContentSize().width * _textSizePercent.x, getContentSize().height * _textSizePercent.y);
}

void TextButton::setOverflow(const cocos2d::Label::Overflow& overflow)
{
	_buttonText->setOverflow(overflow);
}
void TextButton::setTextAreaSizePercent(const cocos2d::Vec2& sizePercent)
{
	_textSizePercent = sizePercent;
	_buttonText->setDimensions(getContentSize().width * _textSizePercent.x, getContentSize().height * _textSizePercent.y);
}
void TextButton::setText(const std::string& text)
{
	_buttonText->setString(text);
}
void TextButton::setTextColour(const cocos2d::Color4B& colour)
{
	_buttonText->setTextColor(colour);
}
void TextButton::setTextFontInfo(const std::string& font, int fontSize)
{
	TTFConfig config = _buttonText->getTTFConfig();
	config.fontFilePath = font;
	config.fontSize = fontSize;
	_buttonText->setTTFConfig(config);
}

NS_AZOOMEE_END
