//
//  VodacomMessageBoxInfo.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 22/10/2018.
//
#ifdef AZOOMEE_VODACOM_BUILD
#include "VodacomMessageBoxInfo.h"
#include <AzoomeeCommon/UI/Colour.h>
#include <AzoomeeCommon/Utils/LocaleManager.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

void VodacomMessageBoxInfo::onEnter()
{
	Super::onEnter();
	
	_messageBox->setContentSize(Size(_visibleSize.width - 100, 855));
	
	_heading = Label::createWithTTF(_headingText, Style::Font::Regular(), 64);
	_heading->setTextColor(Color4B::BLACK);
	_heading->setNormalizedPosition(Vec2(0.5f, 0.5f));
	_heading->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	_heading->setHorizontalAlignment(TextHAlignment::CENTER);
	_heading->setOverflow(Label::Overflow::SHRINK);
	_heading->setDimensions(_messageBox->getContentSize().width * 0.8f, 200);
	_messageBox->addChild(_heading);
	
	_okButton = SettingsMessageBoxButton::create();
	_okButton->loadTextureNormal("res/vodacom/main_button.png");
	_okButton->setScale9Enabled(true);
	_okButton->setContentSize(Size(_visibleSize.width * 0.25f, _okButton->getContentSize().height));
	_okButton->setMessageBoxButtonType(SettingsMessageBoxButtonType::CANCEL);
	_okButton->setNormalizedPosition(Vec2(0.5f,0.33f));
	_okButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_okButton->addTouchEventListener(CC_CALLBACK_2(SettingsMessageBox::onButtonPressed, this));
	_messageBox->addChild(_okButton);
	
	Label* okLabel = Label::createWithTTF(_("OK"), Style::Font::Regular(), 75);
	okLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	okLabel->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	_okButton->addChild(okLabel);
	
}

void VodacomMessageBoxInfo::setHeading(const std::string &heading)
{
	_headingText = heading;
	if(_heading)
	{
		_heading->setString(_headingText);
	}
}

NS_AZOOMEE_END
#endif
