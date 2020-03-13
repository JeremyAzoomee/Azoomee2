//
//  SettingsNavigationButton.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 23/08/2018.
//

#include "SettingsNavigationButton.h"
#include <TinizineCommon/UI/Colour.h>
#include "Style.h"

using namespace cocos2d;

USING_NS_TZ

NS_AZ_BEGIN

bool SettingsNavigationButton::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    setBackGroundColor(Color3B::WHITE);
    
    return true;
}

void SettingsNavigationButton::onEnter()
{
    _icon = ui::ImageView::create(_iconFilename);
    _icon->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_LEFT);
    _icon->setAnchorPoint(Vec2(-0.5f,0.5f));
	_icon->ignoreContentAdaptWithSize(false);
	_icon->setContentSize(Size(213,213));
    this->addChild(_icon);
    
    _arrow = ui::ImageView::create("res/settings/right_arrow_2.png");
    _arrow->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_RIGHT);
    _arrow->setAnchorPoint(Vec2(3.0f,0.5f));
    this->addChild(_arrow);
    
    _textLayout = ui::Layout::create();
    _textLayout->setContentSize(Size(this->getContentSize().width - (_icon->getContentSize().width * 2) - (_arrow->getContentSize().width * 4.0f),this->getContentSize().height));
    _textLayout->setPosition(Vec2((_icon->getContentSize().width * 1.75f), this->getContentSize().height * 0.5f));
    _textLayout->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    this->addChild(_textLayout);
    
    _titleText = Label::createWithTTF(_titleTextString, Style::Font::Medium(), 91);
    _titleText->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    _titleText->setPosition(Vec2(0,(_textLayout->getContentSize().height * 0.5f) + 20));
    _titleText->setTextColor(Color4B(Colours::Color_3B::skyBlue));
	_titleText->setVerticalAlignment(TextVAlignment::BOTTOM);
	_titleText->setHorizontalAlignment(TextHAlignment::LEFT);
	_titleText->setOverflow(Label::Overflow::SHRINK);
	_titleText->setDimensions(_textLayout->getContentSize().width, _textLayout->getContentSize().height * 0.35f);
    _textLayout->addChild(_titleText);
    
    _subTitleText = Label::createWithTTF(_subTitleTextString, Style::Font::Medium(), 49);
    _subTitleText->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    _subTitleText->setPosition(Vec2(0,_textLayout->getContentSize().height * 0.5f));
    _subTitleText->setTextColor(Color4B(Colours::Color_3B::battleshipGrey));
	_subTitleText->setLineSpacing(20.0f);
	_subTitleText->setVerticalAlignment(TextVAlignment::TOP);
	_subTitleText->setHorizontalAlignment(TextHAlignment::LEFT);
	_subTitleText->setOverflow(Label::Overflow::SHRINK);
	_subTitleText->setDimensions(_textLayout->getContentSize().width, _textLayout->getContentSize().height * 0.35f);
    _textLayout->addChild(_subTitleText);
    
    
    Super::onEnter();
}

void SettingsNavigationButton::setIconFilename(const std::string& filename)
{
    _iconFilename = filename;
	if(_icon)
	{
		_icon->loadTexture(_iconFilename);
	}
}
void SettingsNavigationButton::setTitleText(const std::string& text)
{
    _titleTextString = text;
	if(_titleText)
	{
		_titleText->setString(_titleTextString);
		_titleText->setTTFConfig(TTFConfig(Style::Font::Medium(), _titleText->getTTFConfig().fontSize, GlyphCollection::DYNAMIC));
	}
}
void SettingsNavigationButton::setSubTitleText(const std::string& text)
{
    _subTitleTextString = text;
	if(_subTitleText)
	{
		_subTitleText->setString(_subTitleTextString);
		_subTitleText->setTTFConfig(TTFConfig(Style::Font::Medium(), _subTitleText->getTTFConfig().fontSize, GlyphCollection::DYNAMIC));
	}
}

NS_AZ_END
