//
//  SignupTermsPage.cpp
//  Azoomee
//
//  Created by Macauley on 18/07/2019.
//

#include "SignupTermsPage.h"
#include <AzoomeeCommon/Strings.h>
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/UI/LayoutParams.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool SignupTermsPage::init()
{
	if(!Super::init())
	{
		return false;
	}
	
	setBackGroundColorType(BackGroundColorType::SOLID);
	setBackGroundColor(Color3B::WHITE);
	
	_contentLayout = ui::Layout::create();
	_contentLayout->setContentSize(Size(1067,1751));
	_contentLayout->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_contentLayout->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	_contentLayout->setLayoutType(Type::VERTICAL);
	addChild(_contentLayout);
	//"•    "
	ui::Text* para1 = ui::Text::create(_("Azoomee takes the protection and usage of your personal data very seriously. We use your personal data for the following purposes:"), Style::Font::Medium(), 50);
	para1->setTextHorizontalAlignment(TextHAlignment::LEFT);
	para1->setTextVerticalAlignment(TextVAlignment::TOP);
	para1->setTextColor(Color4B(130,130,130,255));
	para1->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,0,0,65)));
	para1->setTextAreaSize(Size(_contentLayout->getContentSize().width, 270));
	Label* para1Lab = dynamic_cast<Label*>(para1->getVirtualRenderer());
	if(para1Lab)
	{
		para1Lab->setOverflow(Label::Overflow::SHRINK);
	}
	_contentLayout->addChild(para1);
	
	ui::Text* bullet1 = ui::Text::create(_("To create a unique Azoomee account"), Style::Font::Regular(), 42);
	bullet1->setTextHorizontalAlignment(TextHAlignment::LEFT);
	bullet1->setTextVerticalAlignment(TextVAlignment::TOP);
	bullet1->setTextColor(Color4B(130,130,130,255));
	bullet1->setLayoutParameter(CreateRightLinearLayoutParam(ui::Margin(0,0,0,30)));
	bullet1->setTextAreaSize(Size(987, 55));
	Label* bullet1Lab = dynamic_cast<Label*>(bullet1->getVirtualRenderer());
	if(bullet1Lab)
	{
		bullet1Lab->setOverflow(Label::Overflow::SHRINK);
	}
	ui::Text* bulletpoint = ui::Text::create("•    ", Style::Font::Regular(), 42);
	bulletpoint->setTextHorizontalAlignment(TextHAlignment::RIGHT);
	bulletpoint->setTextVerticalAlignment(TextVAlignment::TOP);
	bulletpoint->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
	bulletpoint->setNormalizedPosition(Vec2::ANCHOR_TOP_LEFT);
	bulletpoint->setTextColor(Color4B(130,130,130,255));
	bullet1->addChild(bulletpoint);
	
	_contentLayout->addChild(bullet1);
	
	ui::Text* bullet2 = ui::Text::create(_("To contact you when there are important service notifications"), Style::Font::Regular(), 42);
	bullet2->setTextHorizontalAlignment(TextHAlignment::LEFT);
	bullet2->setTextVerticalAlignment(TextVAlignment::TOP);
	bullet2->setTextColor(Color4B(130,130,130,255));
	bullet2->setLayoutParameter(CreateRightLinearLayoutParam(ui::Margin(0,0,0,20)));
	bullet2->setTextAreaSize(Size(987, 110));
	Label* bullet2Lab = dynamic_cast<Label*>(bullet2->getVirtualRenderer());
	if(bullet2Lab)
	{
		bullet2Lab->setOverflow(Label::Overflow::SHRINK);
	}
	auto bulletpoint2 = bulletpoint->clone();
	bulletpoint2->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
	bulletpoint2->setNormalizedPosition(Vec2::ANCHOR_TOP_LEFT);
	bullet2->addChild(bulletpoint2);
	_contentLayout->addChild(bullet2);
	
	ui::Text* bullet3 = ui::Text::create(_("To identify your account should you contact us for support"), Style::Font::Regular(), 42);
	bullet3->setTextHorizontalAlignment(TextHAlignment::LEFT);
	bullet3->setTextVerticalAlignment(TextVAlignment::TOP);
	bullet3->setTextColor(Color4B(130,130,130,255));
	bullet3->setLayoutParameter(CreateRightLinearLayoutParam(ui::Margin(0,0,0,70)));
	bullet3->setTextAreaSize(Size(987, 110));
	Label* bullet3Lab = dynamic_cast<Label*>(bullet3->getVirtualRenderer());
	if(bullet3Lab)
	{
		bullet3Lab->setOverflow(Label::Overflow::SHRINK);
	}
	auto bulletpoint3 = bulletpoint->clone();
	bulletpoint3->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
	bulletpoint3->setNormalizedPosition(Vec2::ANCHOR_TOP_LEFT);
	bullet3->addChild(bulletpoint3);
	_contentLayout->addChild(bullet3);
	
	ui::Text* para2 = ui::Text::create(_("If you'd like to learn more about how we use the data you provide us, please read our Privacy Policy"), Style::Font::Regular(), 42);
	para2->setTextHorizontalAlignment(TextHAlignment::LEFT);
	para2->setTextVerticalAlignment(TextVAlignment::TOP);
	para2->setTextColor(Color4B(130,130,130,255));
	para2->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,0,0,70)));
	para2->setTextAreaSize(Size(_contentLayout->getContentSize().width, 110));
	Label* para2Lab = dynamic_cast<Label*>(para2->getVirtualRenderer());
	if(para2Lab)
	{
		para2Lab->setOverflow(Label::Overflow::SHRINK);
	}
	_contentLayout->addChild(para2);
	
	ui::Text* para3 = ui::Text::create(_("Please check the following to confirm that:"), Style::Font::Medium(), 50);
	para3->setTextHorizontalAlignment(TextHAlignment::LEFT);
	para3->setTextVerticalAlignment(TextVAlignment::TOP);
	para3->setTextColor(Color4B(92,91,91,255));
	para3->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,0,0,35)));
	para3->setTextAreaSize(Size(_contentLayout->getContentSize().width, 70));
	Label* para3Lab = dynamic_cast<Label*>(para3->getVirtualRenderer());
	if(para3Lab)
	{
		para3Lab->setOverflow(Label::Overflow::SHRINK);
	}
	_contentLayout->addChild(para3);
	
	ui::Text* checkbox1 = ui::Text::create(_("You are over 18"), Style::Font::Regular(), 42);
	checkbox1->setTextHorizontalAlignment(TextHAlignment::LEFT);
	checkbox1->setTextVerticalAlignment(TextVAlignment::TOP);
	checkbox1->setTextColor(Color4B(130,130,130,255));
	checkbox1->setLayoutParameter(CreateRightLinearLayoutParam(ui::Margin(0,0,0,35)));
	checkbox1->setTextAreaSize(Size(987, 55));
	Label* checkbox1Lab = dynamic_cast<Label*>(checkbox1->getVirtualRenderer());
	if(checkbox1Lab)
	{
		checkbox1Lab->setOverflow(Label::Overflow::SHRINK);
	}
	_contentLayout->addChild(checkbox1);
	
	_over18Checkbox = ui::CheckBox::create("res/onboarding/check-box-empty.png", "res/onboarding/correct-symbol.png");
	_over18Checkbox->ignoreContentAdaptWithSize(false);
	_over18Checkbox->setContentSize(Size(60,60));
	_over18Checkbox->setAnchorPoint(Vec2(1.2f,0.8f));
	_over18Checkbox->setNormalizedPosition(Vec2::ANCHOR_TOP_LEFT);
	_over18Checkbox->setColor(Color3B(79,78,78));
	checkbox1->addChild(_over18Checkbox);
	
	ui::Text* checkbox2 = ui::Text::create(_("You agree to our Terms of Service, Acceptable Use Policy and Privacy Policy"), Style::Font::Regular(), 42);
	checkbox2->setTextHorizontalAlignment(TextHAlignment::LEFT);
	checkbox2->setTextVerticalAlignment(TextVAlignment::TOP);
	checkbox2->setTextColor(Color4B(130,130,130,255));
	checkbox2->setLayoutParameter(CreateRightLinearLayoutParam(ui::Margin(0,0,0,50)));
	checkbox2->setTextAreaSize(Size(987, 110));
	Label* checkbox2Lab = dynamic_cast<Label*>(checkbox2->getVirtualRenderer());
	if(checkbox2Lab)
	{
		checkbox2Lab->setOverflow(Label::Overflow::SHRINK);
	}
	_contentLayout->addChild(checkbox2);
	
	_acceptTermsCheckBox = ui::CheckBox::create("res/onboarding/check-box-empty.png", "res/onboarding/correct-symbol.png");
	_acceptTermsCheckBox->ignoreContentAdaptWithSize(false);
	_acceptTermsCheckBox->setContentSize(Size(60,60));
	_acceptTermsCheckBox->setAnchorPoint(Vec2(1.2f,0.8f));
	_acceptTermsCheckBox->setNormalizedPosition(Vec2::ANCHOR_TOP_LEFT);
	_acceptTermsCheckBox->setColor(Color3B(79,78,78));
	checkbox2->addChild(_acceptTermsCheckBox);
	
	ui::Text* checkbox3 = ui::Text::create(_("You agree to receive marketing materials from Azoomee (optional. You can also opt-out in the future at any time.)"), Style::Font::Regular(), 42);
	checkbox3->setTextHorizontalAlignment(TextHAlignment::LEFT);
	checkbox3->setTextVerticalAlignment(TextVAlignment::TOP);
	checkbox3->setTextColor(Color4B(130,130,130,255));
	checkbox3->setLayoutParameter(CreateRightLinearLayoutParam(ui::Margin(0,0,0,90)));
	checkbox3->setTextAreaSize(Size(987, 150));
	Label* checkbox3Lab = dynamic_cast<Label*>(checkbox3->getVirtualRenderer());
	if(checkbox3Lab)
	{
		checkbox3Lab->setOverflow(Label::Overflow::SHRINK);
	}
	_contentLayout->addChild(checkbox3);
	
	_acceptMarketingCheckBox = ui::CheckBox::create("res/onboarding/check-box-empty.png", "res/onboarding/correct-symbol.png");
	_acceptMarketingCheckBox->ignoreContentAdaptWithSize(false);
	_acceptMarketingCheckBox->setContentSize(Size(60,60));
	_acceptMarketingCheckBox->setAnchorPoint(Vec2(1.2f,0.8f));
	_acceptMarketingCheckBox->setNormalizedPosition(Vec2::ANCHOR_TOP_LEFT);
	_acceptMarketingCheckBox->setColor(Color3B(79,78,78));
	checkbox3->addChild(_acceptMarketingCheckBox);
	
	_submitButton = ui::Button::create("res/onboarding/rounded_button.png");
	_submitButton->ignoreContentAdaptWithSize(false);
	_submitButton->setContentSize(Size(700,140));
	_submitButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_submitButton->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,0,0,50)));
	_submitButton->setColor(Style::Color::darkIndigo);
	_submitButton->addTouchEventListener([this](Ref* pSender, ui::Widget::TouchEventType eType){
		if(eType == ui::Widget::TouchEventType::ENDED)
		{
			if(_submitCallback)
			{
				_submitCallback(_over18Checkbox->isSelected(), _acceptTermsCheckBox->isSelected(), _acceptMarketingCheckBox->isSelected());
			}
		}
	});
	_contentLayout->addChild(_submitButton);
	
	Label* submitLab = Label::createWithTTF(_("Submit"), Style::Font::Bold(), 70);
	submitLab->setColor(Color3B::WHITE);
	submitLab->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	submitLab->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	submitLab->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
	submitLab->setDimensions(_submitButton->getContentSize().width * 0.9f, _submitButton->getContentSize().height * 0.8f);
	submitLab->setOverflow(Label::Overflow::SHRINK);
	_submitButton->addChild(submitLab);
	
	_backButton = ui::Text::create(_("Back"), Style::Font::Bold(), 63);
	_backButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_backButton->setContentSize(Size(700,140));
	_backButton->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
	_backButton->setTextColor(Color4B(130,130,130,255));
	_backButton->setTouchEnabled(true);
	_backButton->setTouchScaleChangeEnabled(true);
	_backButton->addTouchEventListener([this](Ref* pSender, ui::Widget::TouchEventType eType){
		if(eType == ui::Widget::TouchEventType::ENDED)
		{
			if(_backCallback)
			{
				_backCallback(_over18Checkbox->isSelected(), _acceptTermsCheckBox->isSelected(), _acceptMarketingCheckBox->isSelected());
			}
		}
	});
	_contentLayout->addChild(_backButton);
	
	return true;
}

void SignupTermsPage::onSizeChanged()
{
	Super::onSizeChanged();
	_contentLayout->setScale(MIN((this->getContentSize().width - 270) / _contentLayout->getContentSize().width, (this->getContentSize().height - 190) / _contentLayout->getContentSize().height));
}

void SignupTermsPage::setSubmitCallback(const ButtonCallback& callback)
{
	_submitCallback = callback;
}
void SignupTermsPage::setBackCallback(const ButtonCallback& callback)
{
	_backCallback = callback;
}

void SignupTermsPage::setOver18Selected(bool selected)
{
	if(_over18Checkbox)
	{
		_over18Checkbox->setSelected(selected);
	}
}
void SignupTermsPage::setTermsAccepted(bool selected)
{
	if(_acceptTermsCheckBox)
	{
		_acceptTermsCheckBox->setSelected(selected);
	}
}
void SignupTermsPage::setMarketingAccepted(bool selected)
{
	if(_acceptMarketingCheckBox)
	{
		_acceptMarketingCheckBox->setSelected(selected);
	}
}

NS_AZOOMEE_END
