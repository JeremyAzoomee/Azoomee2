//
//  VodacomOnboardingPinLayer.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 11/10/2018.
//

#include "VodacomOnboardingPinLayer.h"
#include <AzoomeeCommon/Strings.h>
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/UI/LayoutParams.h>
#include <AzoomeeCommon/API/API.h>
#include <AzoomeeCommon/UI/ModalMessages.h>
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include <AzoomeeCommon/Data/Parent/ParentDataParser.h>
#include <AzoomeeCommon/Data/Parent/ParentDataProvider.h>
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include "VodacomMessageBoxExitFlow.h"
#include "VodacomMessageBoxNotification.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool VodacomOnboardingPinLayer::init()
{
	if(!Super::init())
	{
		return false;
	}
	
	setLayoutType(ui::Layout::Type::VERTICAL);
	
	return true;
}

void VodacomOnboardingPinLayer::onEnter()
{
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
				_flowData->setPin(_pinInput->getText());
				_delegate->moveToPreviousState();
			}
		}
	});
	
	ui::Layout* buttonHolder = ui::Layout::create();
	buttonHolder->setContentSize(Size(this->getContentSize().width, _closeButton->getContentSize().height));
	this->addChild(buttonHolder);
	
	buttonHolder->addChild(_closeButton);
	buttonHolder->addChild(_backButton);
	
	Label* title = Label::createWithTTF(_("Setup your PIN"), Style::Font::Regular, 96);
	title->setTextColor(Color4B::BLACK);
	title->setHorizontalAlignment(TextHAlignment::CENTER);
	title->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	title->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	
	ui::Layout* titleHolder = ui::Layout::create();
	titleHolder->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
	titleHolder->setContentSize(title->getContentSize());
	titleHolder->addChild(title);
	this->addChild(titleHolder);
	
	Label* subTitle = Label::createWithTTF(_("Create a 4 digit PIN to access your settings."), Style::Font::Regular, 64);
	subTitle->setTextColor(Color4B::BLACK);
	subTitle->setWidth(this->getContentSize().width * 0.65f);
	subTitle->setHorizontalAlignment(TextHAlignment::CENTER);
	subTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	subTitle->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	
	ui::Layout* subTitleHolder = ui::Layout::create();
	subTitleHolder->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,75,0,0)));
	subTitleHolder->setContentSize(subTitle->getContentSize());
	subTitleHolder->addChild(subTitle);
	this->addChild(subTitleHolder);
	
	Label* inputTitle = Label::createWithTTF(_("Pin"), Style::Font::Regular, 64);
	inputTitle->setTextColor(Color4B::BLACK);
	inputTitle->setHorizontalAlignment(TextHAlignment::CENTER);
	inputTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	inputTitle->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	
	ui::Layout* inputTitleHolder = ui::Layout::create();
	inputTitleHolder->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,100,0,0)));
	inputTitleHolder->setContentSize(inputTitle->getContentSize());
	inputTitleHolder->addChild(inputTitle);
	this->addChild(inputTitleHolder);
	
	_pinInput = TextInputLayer::createSettingsRoundedTextInput(this->getContentSize().width * 0.6f, INPUT_IS_PIN);
	_pinInput->setCenterPosition(_pinInput->getContentSize() / 2.0f);
	_pinInput->setDelegate(this);
	_pinInput->setText(_flowData->getPin());
	
	Label* pinError = Label::createWithTTF(_("*PIN must be 4 numbers only"), Style::Font::Regular, 53);
	pinError->setTextColor(Color4B(Style::Color::watermelon));
	pinError->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	pinError->setNormalizedPosition(Vec2(0.1f,-0.1));
	pinError->setName("error");
	pinError->setVisible(false);
	_pinInput->addChild(pinError);
	
	ui::Layout* inputLayout = ui::Layout::create();
	inputLayout->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,100,0,0)));
	inputLayout->setContentSize(_pinInput->getContentSize());
	inputLayout->addChild(_pinInput);
	this->addChild(inputLayout);
	
	_confirmButton = ui::Button::create("res/vodacom/main_button.png");
	_confirmButton->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,100,0,0)));
	_confirmButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
		if(eType == ui::Widget::TouchEventType::ENDED)
		{
			this->onConfirmPressed();
		}
	});
	this->addChild(_confirmButton);
	
	Label* confirmText = Label::createWithTTF(_("Next Step"), Style::Font::Regular, _confirmButton->getContentSize().height * 0.5f);
	confirmText->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	confirmText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	confirmText->setHorizontalAlignment(TextHAlignment::CENTER);
	confirmText->setVerticalAlignment(TextVAlignment::CENTER);
	confirmText->setDimensions(_confirmButton->getContentSize().width, _confirmButton->getContentSize().height);
	_confirmButton->addChild(confirmText);
	
	ui::ImageView* progressIcon = ui::ImageView::create("res/vodacom/step_counter_3.png");
	progressIcon->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,200,0,0)));
	this->addChild(progressIcon);
	
	Super::onEnter();
	
	_pinInput->focusAndShowKeyboard();
}

void VodacomOnboardingPinLayer::onConfirmPressed()
{
	if(_delegate && _pinInput->inputIsValid())
	{
		_flowData->setPin(_pinInput->getText());
		ModalMessages::getInstance()->startLoading();
		const std::string &sourceDevice = ConfigStorage::getInstance()->getDeviceInformation();
		HttpRequestCreator* request = API::RegisterParentRequest(_flowData->getEmail(), _flowData->getPassword(), _pinInput->getText(), "VODACOM", sourceDevice, "false", this);
		request->execute();
	}
}

//Delegate Functions
void VodacomOnboardingPinLayer::onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body)
{
	if(requestTag == API::TagRegisterParent)
	{
		HttpRequestCreator* request = API::LoginRequest(_flowData->getEmail(), _flowData->getPassword(), this);
		request->execute();
	}
	else if(requestTag == API::TagLogin)
	{
		if(ParentDataParser::getInstance()->parseParentLoginData(body))
		{
			ConfigStorage::getInstance()->setFirstSlideShowSeen();
			ParentDataParser::getInstance()->setLoggedInParentCountryCode(getValueFromHttpResponseHeaderForKey("X-AZ-COUNTRYCODE", headers));
			AnalyticsSingleton::getInstance()->signInSuccessEvent();
			AnalyticsSingleton::getInstance()->setIsUserAnonymous(false);
			_flowData->setUserType(UserType::REGISTERED);
			UserDefault* def = UserDefault::getInstance();
			def->setStringForKey("username", _flowData->getEmail());
			def->flush();
			ModalMessages::getInstance()->stopLoading();
			VodacomMessageBoxNotification* messageBox = VodacomMessageBoxNotification::create();
			messageBox->setHeading(_("Account created"));
			Director::getInstance()->getRunningScene()->addChild(messageBox);
			this->runAction(Sequence::createWithTwoActions(DelayTime::create(4.0f), CallFunc::create([messageBox,this](){
				messageBox->removeFromParent();
				ModalMessages::getInstance()->startLoading();
				HttpRequestCreator* request = API::AddVoucher(ParentDataProvider::getInstance()->getLoggedInParentId(), _flowData->getVoucherCode(), this);
				request->execute();
			})));
		}
	}
	else if(requestTag == API::TagAddVoucher)
	{
		HttpRequestCreator* request = API::UpdateBillingDataRequest(ParentDataProvider::getInstance()->getLoggedInParentId(), this);
		request->execute();
	}
	else if(requestTag == API::TagUpdateBillingData)
	{
		ParentDataParser::getInstance()->parseParentBillingData(body);
		if(_delegate)
		{
			_flowData->resetStateStack();
			_delegate->moveToState(FlowState::ADD_CHILD);
		}
		ModalMessages::getInstance()->stopLoading();
	}
}
void VodacomOnboardingPinLayer::onHttpRequestFailed(const std::string& requestTag, long errorCode)
{
	if(requestTag == API::TagRegisterParent)
	{
		_flowData->setErrorType(ErrorType::ALREADY_REGISTERED);
		if(_delegate)
		{
			_flowData->popState(); // set back to return to register screen
			_delegate->moveToState(FlowState::ERROR);
		}
	}
	else if(requestTag == API::TagAddVoucher)
	{
		_flowData->setErrorType(ErrorType::VOUCHER);
		if(_delegate)
		{
			_flowData->resetStateStack();
			_delegate->moveToState(FlowState::ERROR);
		}
	}
	ModalMessages::getInstance()->stopLoading();
}

//Delegate Functions

void VodacomOnboardingPinLayer::textInputIsValid(TextInputLayer* inputLayer, bool isValid)
{
	auto errorMsg = inputLayer->getChildByName("error");
	if(errorMsg)
	{
		errorMsg->setVisible(!isValid);
	}
	_flowData->setPin(inputLayer->getText());
}
void VodacomOnboardingPinLayer::textInputReturnPressed(TextInputLayer* inputLayer)
{
	_flowData->setPin(inputLayer->getText());
	this->runAction(Sequence::create(DelayTime::create(0.1f), CallFunc::create([&](){
		this->onConfirmPressed();
	}),NULL));
}
void VodacomOnboardingPinLayer::editBoxEditingDidBegin(TextInputLayer* inputLayer)
{
	
}
void VodacomOnboardingPinLayer::editBoxEditingDidEnd(TextInputLayer* inputLayer)
{
	
}

void VodacomOnboardingPinLayer::onButtonPressed(SettingsMessageBox *pSender, SettingsMessageBoxButtonType type)
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
