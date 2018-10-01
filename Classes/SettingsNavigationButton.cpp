//
//  SettingsNavigationButton.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 23/08/2018.
//

#include "SettingsNavigationButton.h"
#include <AzoomeeCommon/UI/Style.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

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
    _icon->setAnchorPoint(Vec2(-0.5,0.5));
	_icon->ignoreContentAdaptWithSize(false);
	_icon->setContentSize(Size(213,213));
    this->addChild(_icon);
    
    _arrow = ui::ImageView::create("res/settings/right_arrow_2.png");
    _arrow->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_RIGHT);
    _arrow->setAnchorPoint(Vec2(1.5,0.5));
    this->addChild(_arrow);
    
    _textLayout = ui::Layout::create();
    _textLayout->setContentSize(Size(this->getContentSize().width - (_icon->getContentSize().width * 2) - (_arrow->getContentSize().width * 1.5),this->getContentSize().height));
    _textLayout->setPosition(Vec2((_icon->getContentSize().width * 1.75f), this->getContentSize().height * 0.5f));
    _textLayout->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    this->addChild(_textLayout);
    
    _titleText = Label::createWithTTF(_titleTextString, Style::Font::Medium, 91);
    _titleText->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    _titleText->setPosition(Vec2(0,(_textLayout->getContentSize().height * 0.5f) + 20));
    _titleText->setTextColor(Color4B(Style::Color::skyBlue));
    _textLayout->addChild(_titleText);
    
    _subTitleText = Label::createWithTTF(_subTitleTextString, Style::Font::Medium, 48);
    _subTitleText->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    _subTitleText->setPosition(Vec2(0,_textLayout->getContentSize().height * 0.5f));
    _subTitleText->setWidth(_textLayout->getContentSize().width);
    _subTitleText->setTextColor(Color4B(Style::Color::battleshipGrey));
    _textLayout->addChild(_subTitleText);
    
    
    Super::onEnter();
}

void SettingsNavigationButton::setIconFilename(const std::string& filename)
{
    _iconFilename = filename;
}
void SettingsNavigationButton::setTitleText(const std::string& text)
{
    _titleTextString = text;
}
void SettingsNavigationButton::setSubTitleText(const std::string& text)
{
    _subTitleTextString = text;
}

NS_AZOOMEE_END
