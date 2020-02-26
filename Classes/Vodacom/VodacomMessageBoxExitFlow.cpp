//
//  VodacomMessageBoxExitFlow.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 22/10/2018.
//
#ifdef AZOOMEE_VODACOM_BUILD
#include "VodacomMessageBoxExitFlow.h"
#include <AzoomeeCommon/Utils/LocaleManager.h>
#include <AzoomeeCommon/UI/Colour.h>

using namespace cocos2d;

USING_NS_TZ

NS_AZ_BEGIN

void VodacomMessageBoxExitFlow::onEnter()
{
	Super::onEnter();
	
	_messageBox->setContentSize(Size(_visibleSize.width - 100, 855));
	
	_heading = Label::createWithTTF(_("Are you sure you want to leave?"), Style::Font::Regular(), 91);
	_heading->setTextColor(Color4B::BLACK);
	_heading->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_heading->setNormalizedPosition(Vec2(0.5f,0.66));
	_heading->setHorizontalAlignment(TextHAlignment::CENTER);
	_heading->setOverflow(Label::Overflow::SHRINK);
	_heading->setDimensions(_messageBox->getContentSize().width * 0.6f, 250);
	_messageBox->addChild(_heading);
	
	std::string subHeading;
	switch(_state)
	{
		case ExitFlowState::VOUCHER:
			subHeading = _("Valid voucher required for full access.");
			break;
		case ExitFlowState::ACCOUNT_CREATE:
			subHeading = _("Your account will not be set up.");
			break;
	}
	
	_subHeading = Label::createWithTTF(subHeading, Style::Font::Regular(), 64);
	_subHeading->setTextColor(Color4B::BLACK);
	_subHeading->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_subHeading->setNormalizedPosition(Vec2(0.5f,0.45));
	_subHeading->setHorizontalAlignment(TextHAlignment::CENTER);
	_subHeading->setOverflow(Label::Overflow::SHRINK);
	_subHeading->setDimensions(_messageBox->getContentSize().width * 0.8f, 150);
	_messageBox->addChild(_subHeading);
	
	_yesButton = SettingsMessageBoxButton::create();
	_yesButton->loadTextureNormal("res/vodacom/main_button.png");
	_yesButton->setScale9Enabled(true);
	_yesButton->setContentSize(Size(_visibleSize.width * 0.25f, _yesButton->getContentSize().height));
	_yesButton->setMessageBoxButtonType(SettingsMessageBoxButtonType::CLOSE);
	_yesButton->setNormalizedPosition(Vec2(0.33f,0.3f));
	_yesButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_yesButton->addTouchEventListener(CC_CALLBACK_2(SettingsMessageBox::onButtonPressed, this));
	_messageBox->addChild(_yesButton);
	
	Label* yesLabel = Label::createWithTTF(_("Yes"), Style::Font::Regular(), 75);
	yesLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	yesLabel->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	_yesButton->addChild(yesLabel);
	
	_noButton = SettingsMessageBoxButton::create();
	_noButton->loadTextureNormal("res/vodacom/main_button.png");
	_noButton->setScale9Enabled(true);
	_noButton->setContentSize(Size(_visibleSize.width * 0.25f, _noButton->getContentSize().height));
	_noButton->setMessageBoxButtonType(SettingsMessageBoxButtonType::CANCEL);
	_noButton->setNormalizedPosition(Vec2(0.66f,0.3f));
	_noButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_noButton->addTouchEventListener(CC_CALLBACK_2(SettingsMessageBox::onButtonPressed, this));
	_messageBox->addChild(_noButton);
	
	Label* noLabel = Label::createWithTTF(_("No"), Style::Font::Regular(), 75);
	noLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	noLabel->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	_noButton->addChild(noLabel);
}

void VodacomMessageBoxExitFlow::setState(const ExitFlowState &state)
{
	_state = state;
	if(_subHeading)
	{
		switch(_state)
		{
			case ExitFlowState::VOUCHER:
				_subHeading->setString(_("You’ll need a valid voucher for full access."));
				break;
			case ExitFlowState::ACCOUNT_CREATE:
				_subHeading->setString(_("Your account will not be set up."));
				break;
		}
	}
}

NS_AZ_END
#endif
