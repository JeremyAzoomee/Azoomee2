//
//  SettingsMessageBoxTryAgain.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 03/10/2018.
//

#include "SettingsMessageBoxTryAgain.h"
#include <AzoomeeCommon/Strings.h>
#include <AzoomeeCommon/UI/Style.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

void SettingsMessageBoxTryAgain::onEnter()
{
	Super::onEnter();
	
	_messageBox->setContentSize(Size(_visibleSize.width - 100, _visibleSize.height * 0.27f));
	
	_heading = Label::createWithTTF(_headingStr, Style::Font::Medium(), 91);
	_heading->setPosition(Vec2(_messageBox->getContentSize().width / 2, _messageBox->getContentSize().height * 0.8f));
	_heading->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
	_heading->setTextColor(Color4B::BLACK);
	_heading->setHorizontalAlignment(TextHAlignment::CENTER);
	_heading->setOverflow(Label::Overflow::SHRINK);
	_heading->setDimensions(_messageBox->getContentSize().width * 0.8f, _messageBox->getContentSize().height * 0.15f);
	_messageBox->addChild(_heading);

	_subHeading = Label::createWithTTF(_subHeadingString, Style::Font::Medium(), 59);
	_subHeading->setPosition(_heading->getPosition() - Vec2(0, _heading->getContentSize().height));
	_subHeading->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
	_subHeading->setTextColor(Color4B::BLACK);
	_subHeading->setHorizontalAlignment(TextHAlignment::CENTER);
	_subHeading->setOverflow(Label::Overflow::SHRINK);
	_subHeading->setDimensions(_messageBox->getContentSize().width * 0.8f, _messageBox->getContentSize().height * 0.3f);
	_messageBox->addChild(_subHeading);
	
	_tryAgainButton = SettingsMessageBoxButton::create();
	_tryAgainButton->loadTextureNormal("res/settings/settings_rounded.png");
	_tryAgainButton->setColor(Style::Color::skyBlue);
	_tryAgainButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_tryAgainButton->setNormalizedPosition(Vec2(0.5f,0.25f));
	_tryAgainButton->setMessageBoxButtonType(SettingsMessageBoxButtonType::TRY_AGAIN);
	_tryAgainButton->addTouchEventListener(CC_CALLBACK_2(SettingsMessageBox::onButtonPressed, this));
	_messageBox->addChild(_tryAgainButton);
	
	Label* tryAgainLabel = Label::createWithTTF(_("Try again"), Style::Font::Medium(), 75);
	tryAgainLabel->setTextColor(Color4B::WHITE);
	tryAgainLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	tryAgainLabel->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	_tryAgainButton->addChild(tryAgainLabel);
	
}

void SettingsMessageBoxTryAgain::setHeading(const std::string &heading)
{
	_headingStr = heading;
	if(_heading)
	{
		_heading->setString(_headingStr);
		_subHeading->setPosition(_heading->getPosition() - Vec2(0, _heading->getContentSize().height + 10));
	}
}

void SettingsMessageBoxTryAgain::setSubHeading(const std::string &subHeading)
{
	_subHeadingString = subHeading;
	if(_subHeading)
	{
		_subHeading->setString(_subHeadingString);
	}
}


NS_AZOOMEE_END
