//
//  VodacomOnboardingLoginLayer.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 11/10/2018.
//

#include "VodacomOnboardingLoginLayer.h"
#include <AzoomeeCommon/Strings.h>
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/UI/LayoutParams.h>
#include <AzoomeeCommon/API/API.h>
#include <AzoomeeCommon/UI/ModalMessages.h>
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include <AzoomeeCommon/Data/Parent/ParentDataParser.h>
#include <AzoomeeCommon/Data/Parent/ParentDataProvider.h>
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include "VodacomMessageBoxNotification.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool VodacomOnboardingLoginLayer::init()
{
	if(!Super::init())
	{
		return false;
	}
	
	setLayoutType(ui::Layout::Type::VERTICAL);
	
	return true;
}

void VodacomOnboardingLoginLayer::onEnter()
{
	_closeButton = ui::Button::create("res/vodacom/close.png");
	_closeButton->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
	_closeButton->setNormalizedPosition(Vec2::ANCHOR_TOP_RIGHT);
	_closeButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
		if(eType == ui::Widget::TouchEventType::ENDED)
		{
			if(_delegate)
			{
				_delegate->moveToState(FlowState::EXIT);
			}
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
	buttonHolder->setContentSize(Size(this->getContentSize().width, _closeButton->getContentSize().height));
	this->addChild(buttonHolder);
	
	buttonHolder->addChild(_closeButton);
	buttonHolder->addChild(_backButton);
	
	Label* title = Label::createWithTTF(_("Log in"), Style::Font::Regular, 96);
	title->setTextColor(Color4B::BLACK);
	title->setHorizontalAlignment(TextHAlignment::CENTER);
	title->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	title->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	
	ui::Layout* titleHolder = ui::Layout::create();
	titleHolder->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
	titleHolder->setContentSize(title->getContentSize());
	titleHolder->addChild(title);
	this->addChild(titleHolder);
	
	Label* inputTitle = Label::createWithTTF(_("Email address"), Style::Font::Regular, 64);
	inputTitle->setTextColor(Color4B::BLACK);
	inputTitle->setHorizontalAlignment(TextHAlignment::CENTER);
	inputTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	inputTitle->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	
	ui::Layout* inputTitleHolder = ui::Layout::create();
	inputTitleHolder->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,100,0,0)));
	inputTitleHolder->setContentSize(inputTitle->getContentSize());
	inputTitleHolder->addChild(inputTitle);
	this->addChild(inputTitleHolder);
	
	_emailInput = TextInputLayer::createSettingsRoundedTextInput(this->getContentSize().width * 0.6f, INPUT_IS_EMAIL);
	_emailInput->setCenterPosition(_emailInput->getContentSize() / 2.0f);
	_emailInput->setDelegate(this);
	_emailInput->setText(_flowData->getEmail());
	
	Label* emailError = Label::createWithTTF(_("*Invalid email address"), Style::Font::Regular, 53);
	emailError->setTextColor(Color4B(Style::Color::watermelon));
	emailError->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	emailError->setNormalizedPosition(Vec2(0.1f,-0.1));
	emailError->setName("error");
	emailError->setVisible(false);
	_emailInput->addChild(emailError);
	
	ui::Layout* inputLayout = ui::Layout::create();
	inputLayout->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,100,0,0)));
	inputLayout->setContentSize(_emailInput->getContentSize());
	inputLayout->addChild(_emailInput);
	this->addChild(inputLayout);
	
	Label* pwInputTitle = Label::createWithTTF(_("Password"), Style::Font::Regular, 64);
	pwInputTitle->setTextColor(Color4B::BLACK);
	pwInputTitle->setHorizontalAlignment(TextHAlignment::CENTER);
	pwInputTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	pwInputTitle->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	
	ui::Layout* pwInputTitleHolder = ui::Layout::create();
	pwInputTitleHolder->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,100,0,0)));
	pwInputTitleHolder->setContentSize(pwInputTitle->getContentSize());
	pwInputTitleHolder->addChild(pwInputTitle);
	this->addChild(pwInputTitleHolder);
	
	_passwordInput = TextInputLayer::createSettingsRoundedTextInput(this->getContentSize().width * 0.6f, INPUT_IS_PASSWORD);
	_passwordInput->setCenterPosition(_passwordInput->getContentSize() / 2.0f);
	_passwordInput->setDelegate(this);
	_passwordInput->setText(_flowData->getPassword());
	
	Label* pwError = Label::createWithTTF(_("*Invalid password"), Style::Font::Regular, 53);
	pwError->setTextColor(Color4B(Style::Color::watermelon));
	pwError->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pwError->setNormalizedPosition(Vec2(0.1f,-0.1));
	pwError->setName("error");
	pwError->setVisible(false);
	_passwordInput->addChild(pwError);
	
	ui::Layout* pwInputLayout = ui::Layout::create();
	pwInputLayout->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,100,0,0)));
	pwInputLayout->setContentSize(_passwordInput->getContentSize());
	pwInputLayout->addChild(_passwordInput);
	this->addChild(pwInputLayout);
	
	_confirmButton = ui::Button::create("res/vodacom/main_button.png");
	_confirmButton->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,100,0,0)));
	_confirmButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
		if(eType == ui::Widget::TouchEventType::ENDED)
		{
			this->onConfirmPressed();
		}
	});
	this->addChild(_confirmButton);
	
	Label* confirmText = Label::createWithTTF(_("Confirm"), Style::Font::Regular, _confirmButton->getContentSize().height * 0.5f);
	confirmText->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	confirmText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	confirmText->setHorizontalAlignment(TextHAlignment::CENTER);
	confirmText->setVerticalAlignment(TextVAlignment::CENTER);
	confirmText->setDimensions(_confirmButton->getContentSize().width, _confirmButton->getContentSize().height);
	_confirmButton->addChild(confirmText);
	
	Label* needHelp = Label::createWithTTF(_("Need help?"), Style::Font::Regular, 64);
	needHelp->setTextColor(Color4B(Style::Color::skyBlue));
	needHelp->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_LEFT);
	needHelp->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	Label* contactUs = Label::createWithTTF(_("Contact us"), Style::Font::Regular, 64);
	contactUs->setTextColor(Color4B(Style::Color::skyBlue));
	contactUs->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_RIGHT);
	contactUs->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
	
	DrawNode* underline = DrawNode::create();
	underline->drawRect(Vec2(0, -7), Vec2(contactUs->getContentSize().width, -6), Color4F(Style::Color::skyBlue));
	contactUs->addChild(underline);
	
	ui::Layout* contactUsHolder = ui::Layout::create();
	contactUsHolder->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,100,0,0)));
	contactUsHolder->setContentSize(Size(needHelp->getContentSize().width + contactUs->getContentSize().width + 20, contactUs->getContentSize().height));
	contactUsHolder->addChild(needHelp);
	contactUsHolder->addChild(contactUs);
	contactUsHolder->setTouchEnabled(true);
	contactUsHolder->addTouchEventListener([](Ref* pSender, ui::Widget::TouchEventType eType){
		if(eType == ui::Widget::TouchEventType::ENDED)
		{
			Application::getInstance()->openURL("mailto:help@azoomee.com");
		}
	});
	this->addChild(contactUsHolder);
	
	Super::onEnter();
	
	_emailInput->focusAndShowKeyboard();
}

void VodacomOnboardingLoginLayer::onConfirmPressed()
{
	if(_delegate && _emailInput->inputIsValid() && _passwordInput->inputIsValid())
	{
		_flowData->setEmail(_emailInput->getText());
		_flowData->setPassword(_passwordInput->getText());
		ModalMessages::getInstance()->startLoading();
		HttpRequestCreator* request = API::LoginRequest(_emailInput->getText(), _passwordInput->getText(), this);
		request->execute();
	}
}

void VodacomOnboardingLoginLayer::onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body)
{
	if(requestTag == API::TagLogin)
	{
		if(ParentDataParser::getInstance()->parseParentLoginData(body))
		{
			ConfigStorage::getInstance()->setFirstSlideShowSeen();
			ParentDataParser::getInstance()->setLoggedInParentCountryCode(getValueFromHttpResponseHeaderForKey("X-AZ-COUNTRYCODE", headers));
			AnalyticsSingleton::getInstance()->signInSuccessEvent();
			AnalyticsSingleton::getInstance()->setIsUserAnonymous(false);
			_flowData->setUserType(UserType::FREE);
			UserDefault* def = UserDefault::getInstance();
			def->setStringForKey("username", _flowData->getEmail());
			def->flush();
			HttpRequestCreator* request = API::UpdateBillingDataRequest(ParentDataProvider::getInstance()->getLoggedInParentId(), this);
			request->execute();
		}
		else
		{
			_flowData->setErrorType(ErrorType::LOGIN);
			if(_delegate)
			{
				_delegate->moveToState(FlowState::ERROR);
			}
			ModalMessages::getInstance()->stopLoading();
		}
	}
	else if(requestTag == API::TagUpdateBillingData)
	{
		ParentDataParser::getInstance()->parseParentBillingData(body);
		if(!ParentDataProvider::getInstance()->isPaidUser())
		{
			ModalMessages::getInstance()->stopLoading();
			VodacomMessageBoxNotification* messageBox = VodacomMessageBoxNotification::create();
			messageBox->setHeading(_("Logged in"));
			Director::getInstance()->getRunningScene()->addChild(messageBox);
			this->runAction(Sequence::createWithTwoActions(DelayTime::create(4.0f), CallFunc::create([messageBox, this](){
				messageBox->removeFromParent();
				ModalMessages::getInstance()->startLoading();
				HttpRequestCreator* request = API::AddVoucher(ParentDataProvider::getInstance()->getLoggedInParentId(), _flowData->getVoucherCode(), this);
				request->execute();
			})));
		}
		else
		{
			ModalMessages::getInstance()->stopLoading();
			_flowData->setErrorType(ErrorType::ALREADY_PREMIUM);
			if(_delegate)
			{
				_delegate->moveToState(FlowState::ERROR);
			}
		}

	}
	else if(requestTag == API::TagAddVoucher)
	{
		HttpRequestCreator* request = API::UpdateBillingDataRequest(ParentDataProvider::getInstance()->getLoggedInParentId(), this);
		request->requestTag = "billingAfterVoucher";
		request->execute();
	}
	else if(requestTag == "billingAfterVoucher")
	{
		ParentDataParser::getInstance()->parseParentBillingData(body);
		ModalMessages::getInstance()->stopLoading();
		if(_delegate)
		{
			_delegate->moveToState(FlowState::SUCCESS);
		}
	}
	
}
void VodacomOnboardingLoginLayer::onHttpRequestFailed(const std::string& requestTag, long errorCode)
{
	if(requestTag == API::TagLogin)
	{
		_flowData->setErrorType(ErrorType::LOGIN);
		if(_delegate)
		{
			_delegate->moveToState(FlowState::ERROR);
		}
	}
	else if(requestTag == API::TagAddVoucher)
	{
		_flowData->setErrorType(ErrorType::VOUCHER);
		if(_delegate)
		{
			_delegate->moveToState(FlowState::ERROR);
		}
	}
	ModalMessages::getInstance()->stopLoading();
}


void VodacomOnboardingLoginLayer::textInputIsValid(TextInputLayer* inputLayer, bool isValid)
{
	auto errorMsg = inputLayer->getChildByName("error");
	if(errorMsg)
	{
		errorMsg->setVisible(!isValid);
	}
	if(inputLayer == _emailInput)
	{
		_flowData->setEmail(inputLayer->getText());
	}
	else if(inputLayer == _passwordInput)
	{
		_flowData->setPassword(inputLayer->getText());
	}
}
void VodacomOnboardingLoginLayer::textInputReturnPressed(TextInputLayer* inputLayer)
{
	if(inputLayer == _emailInput)
	{
		_flowData->setEmail(inputLayer->getText());
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
void VodacomOnboardingLoginLayer::editBoxEditingDidBegin(TextInputLayer* inputLayer)
{
	
}
void VodacomOnboardingLoginLayer::editBoxEditingDidEnd(TextInputLayer* inputLayer)
{
	
}

NS_AZOOMEE_END
