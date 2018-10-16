//
//  VodacomOnboardingErrorLayer.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 11/10/2018.
//

#include "VodacomOnboardingErrorLayer.h"
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/UI/LayoutParams.h>
#include <AzoomeeCommon/Strings.h>
#include <AzoomeeCommon/UI/ModalMessages.h>
#include <AzoomeeCommon/API/API.h>
#include <AzoomeeCommon/Data/Parent/ParentDataProvider.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool VodacomOnboardingErrorLayer::init()
{
	if(!Super::init())
	{
		return false;
	}
	
	setLayoutType(ui::Layout::Type::VERTICAL);
	
	return true;
}

void VodacomOnboardingErrorLayer::onEnter()
{
	switch(_flowData->getErrorType())
	{
		case ErrorType::NONE:
		{
			
			break;
		}
		case ErrorType::LOGIN:
		{
			setupForLoginError();
			break;
		}
		case ErrorType::VOUCHER:
		{
			setupForVoucherError();
			break;
		}
		case ErrorType::RESET:
		{
			setupForPasswordReset();
			break;
		}
		case ErrorType::ALREADY_PREMIUM:
		{
			setupForAlreadyPremium();
			break;
		}
		case ErrorType::ALREADY_REGISTERED:
		{
			setupForAlreadyRegistered();
			break;
		}
	}
	
	Super::onEnter();
}

void VodacomOnboardingErrorLayer::setupForVoucherError()
{
	ui::Button* closeButton = ui::Button::create("res/vodacom/close.png");
	closeButton->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
	closeButton->setNormalizedPosition(Vec2::ANCHOR_TOP_RIGHT);
	closeButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
		if(eType == ui::Widget::TouchEventType::ENDED)
		{
			if(_delegate)
			{
				_delegate->moveToState(FlowState::EXIT);
			}
		}
	});
	
	ui::Button* backButton = ui::Button::create("res/vodacom/back.png");
	backButton->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	backButton->setNormalizedPosition(Vec2::ANCHOR_TOP_LEFT);
	backButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
		if(eType == ui::Widget::TouchEventType::ENDED)
		{
			if(_delegate)
			{
				_delegate->moveToPreviousState();
			}
		}
	});
	
	ui::Layout* buttonHolder = ui::Layout::create();
	buttonHolder->setContentSize(Size(this->getContentSize().width, closeButton->getContentSize().height));
	this->addChild(buttonHolder);
	
	buttonHolder->addChild(closeButton);
	buttonHolder->addChild(backButton);
	
	Label* title = Label::createWithTTF(_("Oops!"), Style::Font::Regular, 96);
	title->setTextColor(Color4B::BLACK);
	title->setHorizontalAlignment(TextHAlignment::CENTER);
	title->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	title->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	
	ui::Layout* titleHolder = ui::Layout::create();
	titleHolder->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
	titleHolder->setContentSize(title->getContentSize());
	titleHolder->addChild(title);
	this->addChild(titleHolder);
	
	Label* subHeading = Label::createWithTTF(_("Your voucher code isn’t valid. Please re-enter it."), Style::Font::Regular, 64);
	subHeading->setTextColor(Color4B::BLACK);
	subHeading->setHorizontalAlignment(TextHAlignment::CENTER);
	subHeading->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	subHeading->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	subHeading->setWidth(this->getContentSize().width * 0.7f);
	
	ui::Layout* subHeadingHolder = ui::Layout::create();
	subHeadingHolder->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,100,0,0)));
	subHeadingHolder->setContentSize(subHeading->getContentSize());
	subHeadingHolder->addChild(subHeading);
	this->addChild(subHeadingHolder);
	
	Label* inputTitle = Label::createWithTTF(_("Voucher code"), Style::Font::Regular, 64);
	inputTitle->setTextColor(Color4B::BLACK);
	inputTitle->setHorizontalAlignment(TextHAlignment::CENTER);
	inputTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	inputTitle->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	
	ui::Layout* inputTitleHolder = ui::Layout::create();
	inputTitleHolder->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,100,0,0)));
	inputTitleHolder->setContentSize(inputTitle->getContentSize());
	inputTitleHolder->addChild(inputTitle);
	this->addChild(inputTitleHolder);
	
	_voucherInput = TextInputLayer::createSettingsRoundedTextInput(this->getContentSize().width * 0.6f, INPUT_IS_VOUCHER);
	_voucherInput->setCenterPosition(_voucherInput->getContentSize() / 2.0f);
	
	ui::Layout* inputLayout = ui::Layout::create();
	inputLayout->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,100,0,0)));
	inputLayout->setContentSize(_voucherInput->getContentSize());
	inputLayout->addChild(_voucherInput);
	this->addChild(inputLayout);
	
	_confirmButton = ui::Button::create("res/vodacom/main_button.png");
	_confirmButton->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,100,0,0)));
	_confirmButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
		if(eType == ui::Widget::TouchEventType::ENDED)
		{
			if(_delegate && _voucherInput->inputIsValid())
			{
				if(_flowData->getUserType() == UserType::FREE)
				{
					ModalMessages::getInstance()->startLoading();
					HttpRequestCreator* request = API::AddVoucher(ParentDataProvider::getInstance()->getLoggedInParentId(), _voucherInput->getText(), this);
					request->execute();
				}
				else
				{
					_flowData->setVoucherCode(_voucherInput->getText());
					_delegate->moveToState(FlowState::REGISTER);
				}
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
}

void VodacomOnboardingErrorLayer::setupForLoginError()
{
	ui::Button* closeButton = ui::Button::create("res/vodacom/close.png");
	closeButton->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
	closeButton->setNormalizedPosition(Vec2::ANCHOR_TOP_RIGHT);
	closeButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
		if(eType == ui::Widget::TouchEventType::ENDED)
		{
			if(_delegate)
			{
				_delegate->moveToState(FlowState::EXIT);
			}
		}
	});
	
	ui::Button* backButton = ui::Button::create("res/vodacom/back.png");
	backButton->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	backButton->setNormalizedPosition(Vec2::ANCHOR_TOP_LEFT);
	backButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
		if(eType == ui::Widget::TouchEventType::ENDED)
		{
			if(_delegate)
			{
				_delegate->moveToPreviousState();
			}
		}
	});
	
	ui::Layout* buttonHolder = ui::Layout::create();
	buttonHolder->setContentSize(Size(this->getContentSize().width, closeButton->getContentSize().height));
	this->addChild(buttonHolder);
	
	buttonHolder->addChild(closeButton);
	buttonHolder->addChild(backButton);
	
	Label* title = Label::createWithTTF(_("Oops!"), Style::Font::Regular, 96);
	title->setTextColor(Color4B::BLACK);
	title->setHorizontalAlignment(TextHAlignment::CENTER);
	title->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	title->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	
	ui::Layout* titleHolder = ui::Layout::create();
	titleHolder->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
	titleHolder->setContentSize(title->getContentSize());
	titleHolder->addChild(title);
	this->addChild(titleHolder);
	
	Label* subHeading = Label::createWithTTF(_("There was a problem with your email or password."), Style::Font::Regular, 64);
	subHeading->setTextColor(Color4B::BLACK);
	subHeading->setHorizontalAlignment(TextHAlignment::CENTER);
	subHeading->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	subHeading->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	subHeading->setWidth(this->getContentSize().width * 0.7f);
	
	ui::Layout* subHeadingHolder = ui::Layout::create();
	subHeadingHolder->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,100,0,0)));
	subHeadingHolder->setContentSize(subHeading->getContentSize());
	subHeadingHolder->addChild(subHeading);
	this->addChild(subHeadingHolder);
	
	ui::Button* tryAgainButton = ui::Button::create("res/vodacom/main_button.png");
	tryAgainButton->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,100,0,0)));
	tryAgainButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
		if(eType == ui::Widget::TouchEventType::ENDED)
		{
			if(_delegate)
			{
				_delegate->moveToPreviousState();
			}
		}
	});
	this->addChild(tryAgainButton);
	
	Label* tryAgainText = Label::createWithTTF(_("Try again"), Style::Font::Regular, tryAgainButton->getContentSize().height * 0.5f);
	tryAgainText->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	tryAgainText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	tryAgainText->setHorizontalAlignment(TextHAlignment::CENTER);
	tryAgainText->setVerticalAlignment(TextVAlignment::CENTER);
	tryAgainText->setDimensions(tryAgainButton->getContentSize().width, tryAgainButton->getContentSize().height);
	tryAgainButton->addChild(tryAgainText);
	
	ui::Button* resetButton = ui::Button::create("res/vodacom/main_button.png");
	resetButton->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,100,0,0)));
	resetButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
		if(eType == ui::Widget::TouchEventType::ENDED)
		{
			ModalMessages::getInstance()->startLoading();
			HttpRequestCreator* request = API::ResetPaswordRequest(_flowData->getEmail(), this);
			request->execute();
		}
	});
	this->addChild(resetButton);
	
	Label* resetText = Label::createWithTTF(_("Reset password"), Style::Font::Regular, resetButton->getContentSize().height * 0.5f);
	resetText->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	resetText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	resetText->setHorizontalAlignment(TextHAlignment::CENTER);
	resetText->setVerticalAlignment(TextVAlignment::CENTER);
	resetText->setDimensions(resetButton->getContentSize().width, resetButton->getContentSize().height);
	resetButton->addChild(resetText);
}

void VodacomOnboardingErrorLayer::setupForPasswordReset()
{
	ui::Button* closeButton = ui::Button::create("res/vodacom/close.png");
	closeButton->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
	closeButton->setNormalizedPosition(Vec2::ANCHOR_TOP_RIGHT);
	closeButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
		if(eType == ui::Widget::TouchEventType::ENDED)
		{
			if(_delegate)
			{
				_delegate->moveToState(FlowState::EXIT);
			}
		}
	});
	
	ui::Button* backButton = ui::Button::create("res/vodacom/back.png");
	backButton->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	backButton->setNormalizedPosition(Vec2::ANCHOR_TOP_LEFT);
	backButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
		if(eType == ui::Widget::TouchEventType::ENDED)
		{
			if(_delegate)
			{
				_delegate->moveToPreviousState();
			}
		}
	});
	
	ui::Layout* buttonHolder = ui::Layout::create();
	buttonHolder->setContentSize(Size(this->getContentSize().width, closeButton->getContentSize().height));
	this->addChild(buttonHolder);
	
	buttonHolder->addChild(closeButton);
	buttonHolder->addChild(backButton);
	
	Label* title = Label::createWithTTF(_("Reset requested"), Style::Font::Regular, 96);
	title->setTextColor(Color4B::BLACK);
	title->setHorizontalAlignment(TextHAlignment::CENTER);
	title->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	title->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	
	ui::Layout* titleHolder = ui::Layout::create();
	titleHolder->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
	titleHolder->setContentSize(title->getContentSize());
	titleHolder->addChild(title);
	this->addChild(titleHolder);
	
	Label* subHeading = Label::createWithTTF(_("Instructions for resetting your password have been sent to:"), Style::Font::Regular, 64);
	subHeading->setTextColor(Color4B::BLACK);
	subHeading->setHorizontalAlignment(TextHAlignment::CENTER);
	subHeading->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	subHeading->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	subHeading->setWidth(this->getContentSize().width * 0.7f);
	
	ui::Layout* subHeadingHolder = ui::Layout::create();
	subHeadingHolder->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,100,0,0)));
	subHeadingHolder->setContentSize(subHeading->getContentSize());
	subHeadingHolder->addChild(subHeading);
	this->addChild(subHeadingHolder);
	
	Label* email = Label::createWithTTF(_flowData->getEmail(), Style::Font::Regular, 64);
	email->setTextColor(Color4B(Style::Color::skyBlue));
	email->setHorizontalAlignment(TextHAlignment::CENTER);
	email->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	email->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	
	ui::Layout* emailHolder = ui::Layout::create();
	emailHolder->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,100,0,0)));
	emailHolder->setContentSize(email->getContentSize());
	emailHolder->addChild(email);
	this->addChild(emailHolder);
	
	ui::Button* okButton = ui::Button::create("res/vodacom/main_button.png");
	okButton->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,100,0,0)));
	okButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
		if(eType == ui::Widget::TouchEventType::ENDED)
		{
			if(_delegate)
			{
				_delegate->moveToPreviousState();
			}
		}
	});
	this->addChild(okButton);
	
	Label* okText = Label::createWithTTF(_("OK"), Style::Font::Regular, okButton->getContentSize().height * 0.5f);
	okText->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	okText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	okText->setHorizontalAlignment(TextHAlignment::CENTER);
	okText->setVerticalAlignment(TextVAlignment::CENTER);
	okText->setDimensions(okButton->getContentSize().width, okButton->getContentSize().height);
	okButton->addChild(okText);
}

void VodacomOnboardingErrorLayer::setupForAlreadyRegistered()
{
	
}

void VodacomOnboardingErrorLayer::setupForAlreadyPremium()
{
	
}

//Delegate Functions
void VodacomOnboardingErrorLayer::onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body)
{
	if(requestTag == API::TagAddVoucher)
	{
		_flowData->setErrorType(ErrorType::NONE);
		if(_delegate)
		{
			_delegate->moveToState(FlowState::SUCCESS);
		}
		ModalMessages::getInstance()->stopLoading();
	}
	else if(requestTag == API::TagResetPasswordRequest)
	{
		_flowData->setErrorType(ErrorType::RESET);
		if(_delegate)
		{
			_flowData->popState(); // suppress multiple fail states
			_delegate->moveToState(FlowState::ERROR);
		}
		ModalMessages::getInstance()->stopLoading();
	}
	
}
void VodacomOnboardingErrorLayer::onHttpRequestFailed(const std::string& requestTag, long errorCode)
{
	if(requestTag == API::TagAddVoucher)
	{
		_flowData->setErrorType(ErrorType::VOUCHER);
		if(_delegate)
		{
			_flowData->popState(); // suppress multiple fail states
			_delegate->moveToState(FlowState::ERROR);
		}
		ModalMessages::getInstance()->stopLoading();
	}
	else if(requestTag == API::TagResetPasswordRequest)
	{
		ModalMessages::getInstance()->stopLoading();
	}
}

NS_AZOOMEE_END
