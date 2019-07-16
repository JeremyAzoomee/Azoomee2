//
//  SignupPage.cpp
//  Azoomee
//
//  Created by Macauley on 16/07/2019.
//

#include "SignupPage.h"
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/UI/LayoutParams.h>
#include <AzoomeeCommon/Strings.h>
#include <AzoomeeCommon/UI/ModalWebview.h>
#include <AzoomeeCommon/Data/Urls.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool SignupPage::init()
{
	if(!Super::init())
	{
		return false;
	}
	
	setBackGroundImage("res/onboarding/rounded_rect_45px.png");
	setBackGroundImageScale9Enabled(true);
	
	const float elementSpacing = 50.0f;
	
	_inputHolder = ui::Layout::create();
	_inputHolder->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	_inputHolder->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	_inputHolder->setLayoutType(Type::VERTICAL);
	//_inputHolder->setSizeType(SizeType::PERCENT);
	//_inputHolder->setSizePercent(Vec2(0.8f,0.3f));
	addChild(_inputHolder);
	
	_inputTitle = ui::Text::create("test input title", Style::Font::Regular(), 50);
	_inputTitle->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
	_inputTitle->setTextHorizontalAlignment(TextHAlignment::CENTER);
	_inputTitle->setTextVerticalAlignment(TextVAlignment::CENTER);
	_inputTitle->setTextColor(Color4B(130,130,130,255));
	_inputHolder->addChild(_inputTitle);
	
	ui::Scale9Sprite* inputBg = ui::Scale9Sprite::create("res/onboarding/input_rounded_frame.png");
	inputBg->setColor(Color3B(171, 168, 168));
	inputBg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	inputBg->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	_inputBox = ui::EditBox::create(inputBg->getContentSize() * 0.8f,"res/onboarding/input_rounded_frame.png");
	_inputBox->setTextHorizontalAlignment(TextHAlignment::CENTER);
	_inputBox->setPlaceHolder(_("Tap here to type").c_str());
	_inputBox->setPlaceholderFontName(Style::Font::InputSystemName);
	_inputBox->setPlaceholderFontColor(Color3B(196,196,196));
	_inputBox->setPlaceholderFontSize(50);
	_inputBox->setFont(Style::Font::InputSystemName, 50);
	_inputBox->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_inputBox->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,elementSpacing,0,0)));
	_inputBox->setFontColor(Color3B::BLACK);
	_inputBox->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
	_inputBox->setDelegate(this);
	_inputBox->addChild(inputBg);
	_inputHolder->addChild(_inputBox);
	
	_continueButton = ui::Button::create("res/onboarding/rounded_button.png");
	_continueButton->ignoreContentAdaptWithSize(false);
	_continueButton->setContentSize(Size(700,140));
	_continueButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_continueButton->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,elementSpacing,0,0)));
	_continueButton->setColor(Style::Color::darkIndigo);
	_continueButton->addTouchEventListener([this](Ref* pSender, ui::Widget::TouchEventType eType){
		if(eType == ui::Widget::TouchEventType::ENDED)
		{
			if(_continueCallback)
			{
				_continueCallback(_inputBox->getText());
			}
		}
	});
	_inputHolder->addChild(_continueButton);
	
	Label* continueLab = Label::createWithTTF(_("Continue"), Style::Font::Bold(), 70);
	continueLab->setColor(Color3B::WHITE);
	continueLab->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	continueLab->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	continueLab->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
	continueLab->setDimensions(_continueButton->getContentSize().width * 0.9f, _continueButton->getContentSize().height * 0.8f);
	continueLab->setOverflow(Label::Overflow::SHRINK);
	_continueButton->addChild(continueLab);
	
	_backButton = ui::Text::create(_("Back"), Style::Font::Bold(), 63);
	_backButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_backButton->setContentSize(Size(700,140));
	_backButton->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,elementSpacing,0,0)));
	_backButton->setTextColor(Color4B(130,130,130,255));
	_backButton->setTouchEnabled(true);
	_backButton->setTouchScaleChangeEnabled(true);
	_backButton->addTouchEventListener([this](Ref* pSender, ui::Widget::TouchEventType eType){
		if(eType == ui::Widget::TouchEventType::ENDED)
		{
			if(_backCallback)
			{
				_backCallback(_inputBox->getText());
			}
		}
	});
	_inputHolder->addChild(_backButton);
	
	_inputHolder->setContentSize(Size(0,_inputTitle->getContentSize().height + _inputBox->getContentSize().height + _continueButton->getContentSize().height + _backButton->getContentSize().height + (3 * elementSpacing)));
	
	_progressBar = ProgressBar::create();
	_progressBar->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	_progressBar->setNormalizedPosition(Vec2(0.5,0.15));
	_progressBar->setNumberOfSteps(3);
	_progressBar->setProgress(1);
	_progressBar->setScale(700 / _progressBar->getContentSize().width);
	addChild(_progressBar);
	
	_progressText = ui::Text::create(_("test progress text"), Style::Font::Regular(), 50);
	_progressText->setTextColor(Color4B(130,130,130,255));
	_progressText->setAnchorPoint(Vec2(0.5f,1.5f));
	_progressText->setNormalizedPosition(Vec2(0.5,0.15));
	addChild(_progressText);
	
	_termsLink = ui::Text::create(_("Terms of use"), Style::Font::Regular(), 35);
	_termsLink->ignoreContentAdaptWithSize(false);
	_termsLink->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	_termsLink->setNormalizedPosition(Vec2(0.025,0.025));
	_termsLink->setTextColor(Color4B(130,130,130,255));
	_termsLink->setTextVerticalAlignment(TextVAlignment::CENTER);
	_termsLink->setTextHorizontalAlignment(TextHAlignment::CENTER);
	_termsLink->setTouchEnabled(true);
	_termsLink->addTouchEventListener([](Ref* pSender, ui::Widget::TouchEventType eType){
		if(eType == ui::Widget::TouchEventType::ENDED)
		{
			ModalWebview::createWithURL(Url::TermsOfUse);
		}
	});
	Label* termsLinkLab = dynamic_cast<Label*>(_termsLink->getVirtualRenderer());
	if(termsLinkLab)
	{
		termsLinkLab->setOverflow(Label::Overflow::SHRINK);
	}
	addChild(_termsLink);
	
	_privacyPolicyLink = ui::Text::create(_("Privacy Policy"), Style::Font::Regular(), 35);
	_privacyPolicyLink->ignoreContentAdaptWithSize(false);
	_privacyPolicyLink->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
	_privacyPolicyLink->setNormalizedPosition(Vec2(0.975,0.025));
	_privacyPolicyLink->setTextColor(Color4B(130,130,130,255));
	_privacyPolicyLink->setTextVerticalAlignment(TextVAlignment::CENTER);
	_privacyPolicyLink->setTextHorizontalAlignment(TextHAlignment::CENTER);
	_privacyPolicyLink->setTouchEnabled(true);
	_privacyPolicyLink->addTouchEventListener([](Ref* pSender, ui::Widget::TouchEventType eType){
		if(eType == ui::Widget::TouchEventType::ENDED)
		{
			ModalWebview::createWithURL(Url::PrivacyPolicyNoLinks);
		}
	});
	Label* privacyPolicyLab = dynamic_cast<Label*>(_privacyPolicyLink->getVirtualRenderer());
	if(privacyPolicyLab)
	{
		privacyPolicyLab->setOverflow(Label::Overflow::SHRINK);
	}
	addChild(_privacyPolicyLink);
	
	return true;
}
void SignupPage::onEnter()
{
	Super::onEnter();
}
void SignupPage::onExit()
{
	Super::onExit();
}
void SignupPage::onSizeChanged()
{
	Super::onSizeChanged();
}

void SignupPage::setContinueCallback(const ButtonCallback& callback)
{
	_continueCallback = callback;
}
void SignupPage::setBackCallback(const ButtonCallback& callback)
{
	_backCallback = callback;
}

//Editbox Delegate Functions
void SignupPage::editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string& text)
{
	
}
void SignupPage::editBoxReturn(cocos2d::ui::EditBox* editBox)
{
	
}
void SignupPage::editBoxEditingDidEndWithAction(cocos2d::ui::EditBox* editBox, EditBoxEndAction action)
{
	
}
void SignupPage::editBoxEditingDidBegin(cocos2d::ui::EditBox* editBox)
{
	
}
void SignupPage::editBoxEditingDidEnd(cocos2d::ui::EditBox* editBox)
{
	
}

NS_AZOOMEE_END
