//
//  VodacomOnboardingRegisterLayer.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 11/10/2018.
//
#ifdef AZOOMEE_VODACOM_BUILD
#include "VodacomOnboardingRegisterLayer.h"
#include <AzoomeeCommon/Utils/LocaleManager.h>
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/UI/LayoutParams.h>
#include "VodacomMessageBoxExitFlow.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool VodacomOnboardingRegisterLayer::init()
{
	if(!Super::init())
	{
		return false;
	}
	
	return true;
}

void VodacomOnboardingRegisterLayer::onEnter()
{
	const Size& contentSize = getContentSize();
	
	_closeButton = ui::Button::create("res/vodacom/close.png");
	_closeButton->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
	_closeButton->setNormalizedPosition(Vec2::ANCHOR_TOP_RIGHT);
	_closeButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
		if(eType == ui::Widget::TouchEventType::ENDED)
		{
			VodacomMessageBoxExitFlow* messageBox = VodacomMessageBoxExitFlow::create();
			messageBox->setDelegate(this);
			messageBox->setState(ExitFlowState::ACCOUNT_CREATE);
			Director::getInstance()->getRunningScene()->addChild(messageBox);
		}
	});
	
	_backButton = ui::Button::create("res/vodacom/back.png");
	_backButton->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	_backButton->setNormalizedPosition(Vec2::ANCHOR_TOP_LEFT);
	_backButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
		if(eType == ui::Widget::TouchEventType::ENDED)
		{
			if(_delegate)
			{
				_flowData->setEmail(_emailInput->getText());
				_flowData->setPassword(_passwordInput->getText());
				_delegate->moveToPreviousState();
			}
		}
	});
	
	ui::Layout* buttonHolder = ui::Layout::create();
	buttonHolder->setContentSize(Size(contentSize.width, _closeButton->getContentSize().height));
	_verticalLayout->addChild(buttonHolder);
	
	buttonHolder->addChild(_closeButton);
	buttonHolder->addChild(_backButton);
	
	Label* title = Label::createWithTTF(_("Setup your account"), Style::Font::Regular(), 96);
	title->setTextColor(Color4B::BLACK);
	title->setHorizontalAlignment(TextHAlignment::CENTER);
	title->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	title->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	
	ui::Layout* titleHolder = ui::Layout::create();
	titleHolder->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
	titleHolder->setContentSize(title->getContentSize());
	titleHolder->addChild(title);
	_verticalLayout->addChild(titleHolder);
	
	Label* loginLabel = Label::createWithTTF(_("Already have an account?"), Style::Font::Regular(), 64);
	loginLabel->setTextColor(Color4B(Style::Color::skyBlue));
	loginLabel->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_LEFT);
	loginLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	Label* logIn = Label::createWithTTF(_("Log in"), Style::Font::Regular(), 64);
	logIn->setTextColor(Color4B(Style::Color::skyBlue));
	logIn->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_RIGHT);
	logIn->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
	
	DrawNode* underline = DrawNode::create();
	underline->drawRect(Vec2(0, -7), Vec2(logIn->getContentSize().width, -6), Color4F(Style::Color::skyBlue));
	logIn->addChild(underline);
	
	ui::Layout* loginLabelHolder = ui::Layout::create();
	loginLabelHolder->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,75,0,0)));
	loginLabelHolder->setContentSize(Size(loginLabel->getContentSize().width + logIn->getContentSize().width + 20, logIn->getContentSize().height));
	loginLabelHolder->addChild(loginLabel);
	loginLabelHolder->addChild(logIn);
	loginLabelHolder->setTouchEnabled(true);
	loginLabelHolder->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
		if(eType == ui::Widget::TouchEventType::ENDED)
		{
			if(_delegate)
			{
				_flowData->setUserType(UserType::ANON_FREE);
				_delegate->moveToState(FlowState::LOGIN);
			}
		}
	});
	_verticalLayout->addChild(loginLabelHolder);
	
	Label* inputTitle = Label::createWithTTF(_("Email address"), Style::Font::Regular(), 64);
	inputTitle->setTextColor(Color4B::BLACK);
	inputTitle->setHorizontalAlignment(TextHAlignment::CENTER);
	inputTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	inputTitle->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	
	ui::Layout* inputTitleHolder = ui::Layout::create();
	inputTitleHolder->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,100,0,0)));
	inputTitleHolder->setContentSize(inputTitle->getContentSize());
	inputTitleHolder->addChild(inputTitle);
	_verticalLayout->addChild(inputTitleHolder);
	
	_emailInput = TextInputLayer::createSettingsRoundedTextInput(contentSize.width * 0.6f, INPUT_IS_EMAIL);
	_emailInput->setCenterPosition(_emailInput->getContentSize() / 2.0f);
	_emailInput->setDelegate(this);
	_emailInput->setText(_flowData->getEmail());
	
	Label* emailError = Label::createWithTTF(_("*Invalid email address"), Style::Font::Regular(), 53);
	emailError->setTextColor(Color4B(Style::Color::watermelon));
	emailError->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	emailError->setNormalizedPosition(Vec2(0.1f,-0.1));
	emailError->setName("error");
	emailError->setVisible(false);
	_emailInput->addChild(emailError);
	
	ui::Layout* inputLayout = ui::Layout::create();
	inputLayout->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,50,0,0)));
	inputLayout->setContentSize(_emailInput->getContentSize());
	inputLayout->addChild(_emailInput);
	_verticalLayout->addChild(inputLayout);
	
	Label* confEmailTitle = Label::createWithTTF(_("Confirm email address"), Style::Font::Regular(), 64);
	confEmailTitle->setTextColor(Color4B::BLACK);
	confEmailTitle->setHorizontalAlignment(TextHAlignment::CENTER);
	confEmailTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	confEmailTitle->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	
	ui::Layout* confEmailTitleHolder = ui::Layout::create();
	confEmailTitleHolder->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,100,0,0)));
	confEmailTitleHolder->setContentSize(confEmailTitle->getContentSize());
	confEmailTitleHolder->addChild(confEmailTitle);
	_verticalLayout->addChild(confEmailTitleHolder);
	
	_confirmEmailInput = TextInputLayer::createSettingsRoundedTextInput(contentSize.width * 0.6f, INPUT_IS_EMAIL);
	_confirmEmailInput->setCenterPosition(_emailInput->getContentSize() / 2.0f);
	_confirmEmailInput->setDelegate(this);
	_confirmEmailInput->setText(_flowData->getEmail());
	
	Label* confEmailError = Label::createWithTTF(_("*Email addresses do not match"), Style::Font::Regular(), 53);
	confEmailError->setTextColor(Color4B(Style::Color::watermelon));
	confEmailError->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	confEmailError->setNormalizedPosition(Vec2(0.1f,-0.1));
	confEmailError->setName("error");
	confEmailError->setVisible(false);
	_confirmEmailInput->addChild(confEmailError);
	
	ui::Layout* confEmailLayout = ui::Layout::create();
	confEmailLayout->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,50,0,0)));
	confEmailLayout->setContentSize(_confirmEmailInput->getContentSize());
	confEmailLayout->addChild(_confirmEmailInput);
	_verticalLayout->addChild(confEmailLayout);
	
	Label* pwInputTitle = Label::createWithTTF(_("Password"), Style::Font::Regular(), 64);
	pwInputTitle->setTextColor(Color4B::BLACK);
	pwInputTitle->setHorizontalAlignment(TextHAlignment::CENTER);
	pwInputTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	pwInputTitle->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	
	ui::Layout* pwInputTitleHolder = ui::Layout::create();
	pwInputTitleHolder->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,100,0,0)));
	pwInputTitleHolder->setContentSize(pwInputTitle->getContentSize());
	pwInputTitleHolder->addChild(pwInputTitle);
	_verticalLayout->addChild(pwInputTitleHolder);
	
	_passwordInput = TextInputLayer::createSettingsRoundedTextInput(contentSize.width * 0.6f, INPUT_IS_NEW_PASSWORD);
	_passwordInput->setCenterPosition(_passwordInput->getContentSize() / 2.0f);
	_passwordInput->setDelegate(this);
	_passwordInput->setText(_flowData->getPassword());
	
	Label* pwError = Label::createWithTTF(_("*Invalid password"), Style::Font::Regular(), 53);
	pwError->setTextColor(Color4B(Style::Color::watermelon));
	pwError->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pwError->setNormalizedPosition(Vec2(0.1f,-0.1));
	pwError->setName("error");
	pwError->setVisible(false);
	_passwordInput->addChild(pwError);
	
	ui::Layout* pwInputLayout = ui::Layout::create();
	pwInputLayout->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,50,0,0)));
	pwInputLayout->setContentSize(_passwordInput->getContentSize());
	pwInputLayout->addChild(_passwordInput);
	_verticalLayout->addChild(pwInputLayout);
	
	_confirmButton = ui::Button::create("res/vodacom/main_button.png");
	_confirmButton->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,100,0,0)));
	_confirmButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
		if(eType == ui::Widget::TouchEventType::ENDED)
		{
			this->onConfirmPressed();
		}
	});
	_verticalLayout->addChild(_confirmButton);
	
	Label* confirmText = Label::createWithTTF(_("Next Step"), Style::Font::Regular(), _confirmButton->getContentSize().height * 0.5f);
	confirmText->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	confirmText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	confirmText->setHorizontalAlignment(TextHAlignment::CENTER);
	confirmText->setVerticalAlignment(TextVAlignment::CENTER);
	confirmText->setDimensions(_confirmButton->getContentSize().width * 0.8f, _confirmButton->getContentSize().height);
	_confirmButton->addChild(confirmText);
	
	ui::ImageView* progressIcon = ui::ImageView::create("res/vodacom/step_counter_2.png");
	progressIcon->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,200,0,0)));
	_verticalLayout->addChild(progressIcon);
	
	Label* needHelp = Label::createWithTTF(_("Need help?"), Style::Font::Regular(), 64);
	needHelp->setTextColor(Color4B(Style::Color::skyBlue));
	needHelp->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_LEFT);
	needHelp->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	Label* contactUs = Label::createWithTTF(_("Contact us"), Style::Font::Regular(), 64);
	contactUs->setTextColor(Color4B(Style::Color::skyBlue));
	contactUs->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_RIGHT);
	contactUs->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
	
	DrawNode* underline2 = DrawNode::create();
	underline2->drawRect(Vec2(0, -7), Vec2(contactUs->getContentSize().width, -6), Color4F(Style::Color::skyBlue));
	contactUs->addChild(underline2);
	
	ui::Layout* contactUsHolder = ui::Layout::create();
	contactUsHolder->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_BOTTOM);
	contactUsHolder->setAnchorPoint(Vec2(0.5f,-1.5f));
	contactUsHolder->setContentSize(Size(needHelp->getContentSize().width + contactUs->getContentSize().width + 20, contactUs->getContentSize().height));
	contactUsHolder->addChild(needHelp);
	contactUsHolder->addChild(contactUs);
	contactUsHolder->setTouchEnabled(true);
	contactUsHolder->addTouchEventListener([](Ref* pSender, ui::Widget::TouchEventType eType){
		if(eType == ui::Widget::TouchEventType::ENDED)
		{
			Application::getInstance()->openURL(_("mailto:help@azoomee.com"));
		}
	});
	this->addChild(contactUsHolder);
	
	Super::onEnter();
	
	_emailInput->focusAndShowKeyboard();
}

void VodacomOnboardingRegisterLayer::onConfirmPressed()
{
	auto emailErrorMsg = _emailInput->getChildByName("error");
	if(emailErrorMsg)
	{
		emailErrorMsg->setVisible(!_emailInput->inputIsValid());
	}
	auto emailConfErrorMsg = _confirmEmailInput->getChildByName("error");
	if(emailConfErrorMsg)
	{
		emailConfErrorMsg->setVisible(_confirmEmailInput->getText() != _emailInput->getText());
	}
	auto pwErrorMsg = _passwordInput->getChildByName("error");
	if(pwErrorMsg)
	{
		pwErrorMsg->setVisible(!_passwordInput->inputIsValid());
	}
	
	if(_delegate && _emailInput->inputIsValid() && _passwordInput->inputIsValid() && (_emailInput->getText() == _confirmEmailInput->getText()))
	{
		_flowData->setEmail(_emailInput->getText());
		_flowData->setPassword(_passwordInput->getText());
		_delegate->moveToState(FlowState::PIN);
	}
}

//Delegate Functions

void VodacomOnboardingRegisterLayer::textInputIsValid(TextInputLayer* inputLayer, bool isValid)
{
	auto errorMsg = inputLayer->getChildByName("error");
	if(errorMsg)
	{
		errorMsg->setVisible(!isValid);
	}
	
	auto errorMsg2 = _confirmEmailInput->getChildByName("error");
	errorMsg2->setVisible(_confirmEmailInput->getText() != _emailInput->getText());
	
	if(inputLayer == _emailInput)
	{
		_flowData->setEmail(inputLayer->getText());
	}
	else if(inputLayer == _passwordInput)
	{
		_flowData->setPassword(inputLayer->getText());
	}
}
void VodacomOnboardingRegisterLayer::textInputReturnPressed(TextInputLayer* inputLayer)
{
	if(inputLayer == _emailInput)
	{
		_flowData->setEmail(inputLayer->getText());
		_confirmEmailInput->focusAndShowKeyboard();
	}
	else if(inputLayer == _confirmEmailInput)
	{
		_passwordInput->focusAndShowKeyboard();
	}
	else if(inputLayer == _passwordInput)
	{
		_flowData->setPassword(inputLayer->getText());
		this->runAction(Sequence::create(DelayTime::create(0.1f), CallFunc::create([&](){
			this->onConfirmPressed();
		}),NULL));
	}
}
void VodacomOnboardingRegisterLayer::editBoxEditingDidBegin(TextInputLayer* inputLayer)
{
	
}
void VodacomOnboardingRegisterLayer::editBoxEditingDidEnd(TextInputLayer* inputLayer)
{
	
}

void VodacomOnboardingRegisterLayer::onButtonPressed(SettingsMessageBox *pSender, SettingsMessageBoxButtonType type)
{
	pSender->removeFromParent();
	if(type == SettingsMessageBoxButtonType::CLOSE)
	{
		if(_delegate)
		{
			_delegate->moveToState(FlowState::EXIT);
		}
	}
}

NS_AZOOMEE_END
#endif
