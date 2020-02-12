//
//  SettingsPageHeader.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 23/08/2018.
//

#include "SettingsPageHeader.h"
#include <AzoomeeCommon/UI/Colour.h>
#include "Style.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool SettingsPageHeader::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    setBackGroundColor(Colours::Color_3B::carolinaBlue);
    
    return true;
}

void SettingsPageHeader::onEnter()
{
    _text = Label::createWithTTF(_textString, Style::Font::Medium(), 53);
    _text->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _text->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _text->setHorizontalAlignment(TextHAlignment::CENTER);
	_text->setVerticalAlignment(TextVAlignment::CENTER);
    _text->setTextColor(Color4B(Colours::Color_3B::white));
	_text->setOverflow(Label::Overflow::SHRINK);
    _text->setDimensions(this->getContentSize().width * 0.8f, this->getContentSize().height * 0.7f);
	_text->setLineSpacing(20);
    this->addChild(_text);
    
    Super::onEnter();
}

void SettingsPageHeader::setText(const std::string& text)
{
    _textString = text;
}

NS_AZOOMEE_END
