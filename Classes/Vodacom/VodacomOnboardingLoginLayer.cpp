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
				_delegate->moveToState(_flowData->getPrevState());
			}
		}
	});
	
	ui::Layout* buttonHolder = ui::Layout::create();
	buttonHolder->setContentSize(Size(this->getContentSize().width, _closeButton->getContentSize().height));
	this->addChild(buttonHolder);
	
	buttonHolder->addChild(_closeButton);
	buttonHolder->addChild(_backButton);
	
	Label* title = Label::createWithTTF(_("Setup your account"), Style::Font::Regular, 96);
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
			if(_delegate && _emailInput->inputIsValid() && _passwordInput->inputIsValid())
			{
				ModalMessages::getInstance()->startLoading();
				HttpRequestCreator* request = API::LoginRequest(_emailInput->getText(), _passwordInput->getText(), this);
				request->execute();
			}
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
	
	Super::onEnter();
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
			
			HttpRequestCreator* request = API::UpdateBillingDataRequest(this);
			request->execute();
		}
	}
	else if(requestTag == API::TagUpdateBillingData)
	{
		ParentDataParser::getInstance()->parseParentBillingData(body);
		if(!ParentDataProvider::getInstance()->isPaidUser())
		{
			HttpRequestCreator* request = API::AddVoucher(ParentDataProvider::getInstance()->getLoggedInParentId(), _flowData->getVoucherCode(), this);
			request->execute();
		}
		else
		{
			ModalMessages::getInstance()->stopLoading();
			_flowData->setErrorType(ErrorType::ALREADY_PREMIUM);
		}

	}
	else if(requestTag == API::TagAddVoucher)
	{
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

NS_AZOOMEE_END
