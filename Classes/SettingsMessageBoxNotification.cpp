//
//  SettingsMessageBoxNotification.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 03/10/2018.
//

#include "SettingsMessageBoxNotification.h"
#include <AzoomeeCommon/Strings.h>
#include <AzoomeeCommon/UI/Style.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

void SettingsMessageBoxNotification::onEnter()
{
	Super::onEnter();
	
	_messageBox->setContentSize(Size(_visibleSize.width - 100, _visibleSize.height * 0.27f));
	
	_heading = Label::createWithTTF(_headingStr, Style::Font::Medium(), 91);
	_heading->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	_heading->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_heading->setWidth(_messageBox->getContentSize().width * 0.75f);
	_heading->setTextColor(Color4B::BLACK);
	_heading->setHorizontalAlignment(TextHAlignment::CENTER);
	_messageBox->addChild(_heading);
	
	_closeButton = SettingsMessageBoxButton::create();
	_closeButton->loadTextureNormal("res/settings/exit_button_red.png");
	_closeButton->setAnchorPoint(Vec2(-0.5f,1.5f));
	_closeButton->setNormalizedPosition(Vec2::ANCHOR_TOP_LEFT);
	_closeButton->setMessageBoxButtonType(SettingsMessageBoxButtonType::CLOSE);
	_closeButton->addTouchEventListener(CC_CALLBACK_2(SettingsMessageBox::onButtonPressed, this));
	_messageBox->addChild(_closeButton);
}

void SettingsMessageBoxNotification::setHeading(const std::string &heading)
{
	_headingStr = heading;
	if(_heading)
	{
		_heading->setString(_headingStr);
	}
}


NS_AZOOMEE_END

