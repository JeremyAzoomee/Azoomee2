//
//  VodacomOnboardingTnCLayer.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 08/11/2018.
//
#ifdef AZOOMEE_VODACOM_BUILD
#include "VodacomOnboardingTnCLayer.h"
#include <AzoomeeCommon/UI/LayoutParams.h>
#include <AzoomeeCommon/Utils/LocaleManager.h>
#include <AzoomeeCommon/UI/Colour.h>
#include <AzoomeeCommon/UI/ModalWebview.h>
#include "Urls.h"

using namespace cocos2d;

USING_NS_TZ

NS_AZ_BEGIN

bool VodacomOnboardingTnCLayer::init()
{
	if(!Super::init())
	{
		return false;
	}
	
	return true;
}

void VodacomOnboardingTnCLayer::onEnter()
{
	const Size& contentSize = getContentSize();
	
	_closeButton = ui::Button::create("res/vodacom/close.png");
	_closeButton->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
	_closeButton->setLayoutParameter(CreateRightLinearLayoutParam());
	_closeButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
		if(eType == ui::Widget::TouchEventType::ENDED)
		{
			if(_delegate)
			{
				_delegate->moveToState(FlowState::EXIT);
			}
		}
	});
	_verticalLayout->addChild(_closeButton);
	
	Label* title = Label::createWithTTF(_("Terms and Conditions"), Style::Font::Regular(), 96);
	title->setHorizontalAlignment(TextHAlignment::CENTER);
	title->setTextColor(Color4B::BLACK);
	title->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	title->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	title->setWidth(contentSize.width * 0.8f);
	
	ui::Layout* titleHolder = ui::Layout::create();
	titleHolder->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
	titleHolder->setContentSize(title->getContentSize());
	_verticalLayout->addChild(titleHolder);
	titleHolder->addChild(title);
	
	Label* subTitle = Label::createWithTTF(_("please tick to confirm that:"), Style::Font::Regular(), 64);
	subTitle->setHorizontalAlignment(TextHAlignment::CENTER);
	subTitle->setTextColor(Color4B::BLACK);
	subTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	subTitle->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	subTitle->setWidth(contentSize.width * 0.8f);
	
	ui::Layout* subTitleHolder = ui::Layout::create();
	subTitleHolder->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,50,0,0)));
	subTitleHolder->setContentSize(subTitle->getContentSize());
	_verticalLayout->addChild(subTitleHolder);
	subTitleHolder->addChild(subTitle);
	
	ui::Layout* over18Layout = ui::Layout::create();
	over18Layout->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,100,0,0)));
	over18Layout->setContentSize(Size(contentSize.width * 0.8f, 0));
	
	_over18Checkbox = ui::CheckBox::create("res/vodacom/checkbox_bg.png", "res/vodacom/checkbox_tick.png");
	_over18Checkbox->setNormalizedPosition(Vec2::ANCHOR_TOP_LEFT);
	_over18Checkbox->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	_over18Checkbox->addEventListener([&](Ref* pSender, ui::CheckBox::EventType eType){
		if(eType == ui::CheckBox::EventType::SELECTED)
		{
			if(_tnCCheckbox->isSelected())
			{
				_errorMessage->setVisible(false);
			}
		}
		else
		{
			_errorMessage->setVisible(true);
		}
	});
	over18Layout->addChild(_over18Checkbox);
	
	Label* over18Label = Label::createWithTTF(_("You are over 18"), Style::Font::Regular(), 64);
	over18Label->setTextColor(Color4B::BLACK);
	over18Label->setNormalizedPosition(Vec2::ANCHOR_TOP_RIGHT);
	over18Label->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
	over18Label->setHorizontalAlignment(TextHAlignment::LEFT);
	over18Label->setVerticalAlignment(TextVAlignment::TOP);
	over18Label->setWidth(contentSize.width * 0.8f - _over18Checkbox->getContentSize().width - 80);
	over18Layout->addChild(over18Label);
	over18Layout->setContentSize(Size(contentSize.width * 0.8f, over18Label->getContentSize().height));
	_verticalLayout->addChild(over18Layout);
	
	ui::Layout* tnCLayout = ui::Layout::create();
	tnCLayout->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,100,0,0)));
	tnCLayout->setContentSize(Size(contentSize.width * 0.8f, 0));
	
	_tnCCheckbox = ui::CheckBox::create("res/vodacom/checkbox_bg.png", "res/vodacom/checkbox_tick.png");
	_tnCCheckbox->setNormalizedPosition(Vec2::ANCHOR_TOP_LEFT);
	_tnCCheckbox->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	_tnCCheckbox->addEventListener([&](Ref* pSender, ui::CheckBox::EventType eType){
		if(eType == ui::CheckBox::EventType::SELECTED)
		{
			if(_over18Checkbox->isSelected())
			{
				_errorMessage->setVisible(false);
			}
		}
		else
		{
			_errorMessage->setVisible(true);
		}
	});
	tnCLayout->addChild(_tnCCheckbox);
	
	Label* tnCLabel = Label::createWithTTF(_("You agree to our Terms of Service, Acceptable Use Policy and Privacy Policy,"), Style::Font::Regular(), 64);
	tnCLabel->setTextColor(Color4B::BLACK);
	tnCLabel->setNormalizedPosition(Vec2::ANCHOR_TOP_RIGHT);
	tnCLabel->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
	tnCLabel->setHorizontalAlignment(TextHAlignment::LEFT);
	tnCLabel->setVerticalAlignment(TextVAlignment::TOP);
	tnCLabel->setWidth(contentSize.width * 0.8f - _tnCCheckbox->getContentSize().width - 80);
	tnCLayout->addChild(tnCLabel);
	tnCLayout->setContentSize(Size(contentSize.width * 0.8f, tnCLabel->getContentSize().height));
	_verticalLayout->addChild(tnCLayout);
	
	ui::Layout* marketingLayout = ui::Layout::create();
	marketingLayout->setLayoutType(ui::Layout::Type::HORIZONTAL);
	marketingLayout->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,100,0,0)));
	marketingLayout->setContentSize(Size(contentSize.width * 0.8f, 0));
	
	_marketingCheckbox = ui::CheckBox::create("res/vodacom/checkbox_bg.png", "res/vodacom/checkbox_tick.png");
	_marketingCheckbox->setNormalizedPosition(Vec2::ANCHOR_TOP_LEFT);
	_marketingCheckbox->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	marketingLayout->addChild(_marketingCheckbox);
	
	Label* marketingLabel = Label::createWithTTF(_("You agree to receive marketing materials from Azoomee (optional. You can also opt-out in the future at any time.)"), Style::Font::Regular(), 64);
	marketingLabel->setTextColor(Color4B::BLACK);
	marketingLabel->setNormalizedPosition(Vec2::ANCHOR_TOP_RIGHT);
	marketingLabel->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
	marketingLabel->setHorizontalAlignment(TextHAlignment::LEFT);
	marketingLabel->setVerticalAlignment(TextVAlignment::TOP);
	marketingLabel->setWidth(contentSize.width * 0.8f - _marketingCheckbox->getContentSize().width - 80);
	marketingLayout->addChild(marketingLabel);
	marketingLayout->setContentSize(Size(contentSize.width * 0.8f, marketingLabel->getContentSize().height));
	_verticalLayout->addChild(marketingLayout);
	
	_errorMessage = Label::createWithTTF("*You must be over 18 and agree to our T&Cs", Style::Font::Regular(), 59);
	_errorMessage->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_errorMessage->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	_errorMessage->setTextColor(Color4B(Colours::Color_3B::watermelon));
	_errorMessage->setHorizontalAlignment(TextHAlignment::LEFT);
	_errorMessage->setVerticalAlignment(TextVAlignment::TOP);
	_errorMessage->setVisible(false);
	_errorMessage->setWidth(contentSize.width * 0.8f - _marketingCheckbox->getContentSize().width - 50);
	
	ui::Layout* errorLayout = ui::Layout::create();
	errorLayout->setContentSize(_errorMessage->getContentSize());
	errorLayout->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(_marketingCheckbox->getContentSize().width + 50,20,0,0)));
	errorLayout->addChild(_errorMessage);
	_verticalLayout->addChild(errorLayout);
	
	_confirmButton = ui::Button::create("res/vodacom/main_button.png");
	_confirmButton->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,100,0,0)));
	_confirmButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
		if(eType == ui::Widget::TouchEventType::ENDED)
		{
			this->onConfirmPressed();
		}
	});
	_verticalLayout->addChild(_confirmButton);
	
	Label* confirmText = Label::createWithTTF(_("Confirm"), Style::Font::Regular(), _confirmButton->getContentSize().height * 0.5f);
	confirmText->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	confirmText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	confirmText->setHorizontalAlignment(TextHAlignment::CENTER);
	confirmText->setVerticalAlignment(TextVAlignment::CENTER);
	confirmText->setDimensions(_confirmButton->getContentSize().width * 0.8f, _confirmButton->getContentSize().height);
	_confirmButton->addChild(confirmText);
	
	Label* privacyPolicy = Label::createWithTTF(_("Privacy Policy"), Style::Font::Regular(), 64);
	privacyPolicy->setTextColor(Color4B(Colours::Color_3B::skyBlue));
	privacyPolicy->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	privacyPolicy->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	
	DrawNode* underline = DrawNode::create();
	underline->drawRect(Vec2(0, -7), Vec2(privacyPolicy->getContentSize().width, -6), Color4F(Colours::Color_3B::skyBlue));
	privacyPolicy->addChild(underline);
	
	ui::Layout* privacyPolicyHolder = ui::Layout::create();
	privacyPolicyHolder->setPosition(Vec2(contentSize.width / 2, 0));
	privacyPolicyHolder->setAnchorPoint(Vec2(0.5f,-1.5f));
	privacyPolicyHolder->setContentSize(privacyPolicy->getContentSize());
	privacyPolicyHolder->addChild(privacyPolicy);
	privacyPolicyHolder->setTouchEnabled(true);
	privacyPolicyHolder->addTouchEventListener([](Ref* pSender, ui::Widget::TouchEventType eType){
		if(eType == ui::Widget::TouchEventType::ENDED)
		{
			ModalWebview::createWithURL(Url::PrivacyPolicyNoLinks);
		}
	});
	this->addChild(privacyPolicyHolder);
	
	Label* tnc = Label::createWithTTF(_("Terms and Conditions"), Style::Font::Regular(), 64);
	tnc->setTextColor(Color4B(Colours::Color_3B::skyBlue));
	tnc->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	tnc->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	
	DrawNode* underline2 = DrawNode::create();
	underline2->drawRect(Vec2(0, -7), Vec2(tnc->getContentSize().width, -6), Color4F(Colours::Color_3B::skyBlue));
	tnc->addChild(underline2);
	
	ui::Layout* tncHolder = ui::Layout::create();
	tncHolder->setPosition(Vec2(contentSize.width / 2, privacyPolicyHolder->getContentSize().height * 2.5f + 100));
	tncHolder->setAnchorPoint(Vec2(0.5f,0));
	tncHolder->setContentSize(tnc->getContentSize());
	tncHolder->addChild(tnc);
	tncHolder->setTouchEnabled(true);
	tncHolder->addTouchEventListener([](Ref* pSender, ui::Widget::TouchEventType eType){
		if(eType == ui::Widget::TouchEventType::ENDED)
		{
			ModalWebview::createWithURL(Url::TermsOfUse);
		}
	});
	this->addChild(tncHolder);
	
	Super::onEnter();
}

void VodacomOnboardingTnCLayer::onConfirmPressed()
{
	if(_over18Checkbox->isSelected() && _tnCCheckbox->isSelected())
	{
		if(_delegate)
		{
			_flowData->setAcceptedMarketing(_marketingCheckbox->isSelected());
			_delegate->moveToState(FlowState::REGISTER);
		}
	}
	else
	{
		_errorMessage->setVisible(true);
	}
}

NS_AZ_END
#endif
