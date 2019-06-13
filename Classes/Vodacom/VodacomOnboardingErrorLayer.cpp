//
//  VodacomOnboardingErrorLayer.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 11/10/2018.
//
#ifdef AZOOMEE_VODACOM_BUILD
#include "VodacomOnboardingErrorLayer.h"
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/UI/LayoutParams.h>
#include <AzoomeeCommon/Strings.h>
#include <AzoomeeCommon/UI/ModalMessages.h>
#include <AzoomeeCommon/API/API.h>
#include <AzoomeeCommon/Data/Parent/ParentManager.h>
#include <AzoomeeCommon/NativeShare/NativeShare.h>
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include "VodacomMessageBoxExitFlow.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool VodacomOnboardingErrorLayer::init()
{
	if(!Super::init())
	{
		return false;
	}
	
	return true;
}

void VodacomOnboardingErrorLayer::onEnter()
{
	switch(_flowData->getErrorType())
	{
		case ErrorType::NONE:
		{
			if(_delegate)
			{
				_delegate->moveToPreviousState();
			}
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
		case ErrorType::GENERIC:
		{
			setupForGenericError();
			break;
		}
	}
	
	Super::onEnter();
}

void VodacomOnboardingErrorLayer::setupForVoucherError()
{
	const Size& contentSize = getContentSize();
	
	if(_flowData->getUserType() == UserType::FREE)
	{
		ui::Button* closeButton = ui::Button::create("res/vodacom/close.png");
		closeButton->setLayoutParameter(CreateRightLinearLayoutParam());
		closeButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
			if(eType == ui::Widget::TouchEventType::ENDED)
			{
				VodacomMessageBoxExitFlow* messageBox = VodacomMessageBoxExitFlow::create();
				messageBox->setDelegate(this);
				messageBox->setState(ExitFlowState::VOUCHER);
				Director::getInstance()->getRunningScene()->addChild(messageBox);
			}
		});
		_verticalLayout->addChild(closeButton);
	}
	Label* title = Label::createWithTTF(_("Oops!"), Style::Font::Regular(), 96);
	title->setTextColor(Color4B::BLACK);
	title->setHorizontalAlignment(TextHAlignment::CENTER);
	title->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	title->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	
	ui::Layout* titleHolder = ui::Layout::create();
	titleHolder->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,((_flowData->getUserType() == UserType::FREE) ? 0 : 200),0, 0)));
	titleHolder->setContentSize(title->getContentSize());
	titleHolder->addChild(title);
	_verticalLayout->addChild(titleHolder);
	
	Label* subHeading = Label::createWithTTF(_("Your voucher code isn’t valid. Please re-enter it."), Style::Font::Regular(), 64);
	subHeading->setTextColor(Color4B::BLACK);
	subHeading->setHorizontalAlignment(TextHAlignment::CENTER);
	subHeading->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	subHeading->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	subHeading->setWidth(contentSize.width * 0.7f);
	
	ui::Layout* subHeadingHolder = ui::Layout::create();
	subHeadingHolder->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,100,0,0)));
	subHeadingHolder->setContentSize(subHeading->getContentSize());
	subHeadingHolder->addChild(subHeading);
	_verticalLayout->addChild(subHeadingHolder);
	
	Label* inputTitle = Label::createWithTTF(_("Voucher code"), Style::Font::Regular(), 64);
	inputTitle->setTextColor(Color4B::BLACK);
	inputTitle->setHorizontalAlignment(TextHAlignment::CENTER);
	inputTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	inputTitle->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	
	ui::Layout* inputTitleHolder = ui::Layout::create();
	inputTitleHolder->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,100,0,0)));
	inputTitleHolder->setContentSize(inputTitle->getContentSize());
	inputTitleHolder->addChild(inputTitle);
	_verticalLayout->addChild(inputTitleHolder);
	
	_voucherInput = TextInputLayer::createSettingsRoundedTextInput(contentSize.width * 0.6f, INPUT_IS_VOUCHER);
	_voucherInput->setCenterPosition(_voucherInput->getContentSize() / 2.0f);
	_voucherInput->setDelegate(this);
	
	Label* voucherError = Label::createWithTTF(_("*Invalid Voucher"), Style::Font::Regular(), 53);
	voucherError->setTextColor(Color4B(Style::Color::watermelon));
	voucherError->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	voucherError->setNormalizedPosition(Vec2(0.1f,-0.1));
	voucherError->setName("error");
	voucherError->setVisible(false);
	_voucherInput->addChild(voucherError);
	
	ui::Layout* inputLayout = ui::Layout::create();
	inputLayout->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,50,0,0)));
	inputLayout->setContentSize(_voucherInput->getContentSize());
	inputLayout->addChild(_voucherInput);
	_verticalLayout->addChild(inputLayout);
	
	_confirmButton = ui::Button::create("res/vodacom/main_button.png");
	_confirmButton->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,100,0,0)));
	_confirmButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
		if(eType == ui::Widget::TouchEventType::ENDED)
		{
			if(_delegate && _voucherInput->inputIsValid())
			{
				if(_flowData->getUserType() == UserType::FREE || _flowData->getUserType() == UserType::REGISTERED)
				{
					ModalMessages::getInstance()->startLoading();
					HttpRequestCreator* request = API::AddVoucher(ParentManager::getInstance()->getLoggedInParentId(), _voucherInput->getText(), this);
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
	_verticalLayout->addChild(_confirmButton);
	
	Label* confirmText = Label::createWithTTF(_("Confirm"), Style::Font::Regular(), _confirmButton->getContentSize().height * 0.5f);
	confirmText->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	confirmText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	confirmText->setHorizontalAlignment(TextHAlignment::CENTER);
	confirmText->setVerticalAlignment(TextVAlignment::CENTER);
	confirmText->setDimensions(_confirmButton->getContentSize().width * 0.8f, _confirmButton->getContentSize().height);
	_confirmButton->addChild(confirmText);
	
	if(_flowData->getUserType() == UserType::REGISTERED)
	{
		Label* skip = Label::createWithTTF(_("Skip this step"), Style::Font::Regular(), 64);
		skip->setTextColor(Color4B(Style::Color::skyBlue));
		skip->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
		skip->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		
		DrawNode* underline2 = DrawNode::create();
		underline2->drawRect(Vec2(0, -7), Vec2(skip->getContentSize().width, -6), Color4F(Style::Color::skyBlue));
		skip->addChild(underline2);
		
		ui::Layout* skipHolder = ui::Layout::create();
		skipHolder->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,75,0,0)));
		skipHolder->setContentSize(skip->getContentSize());
		skipHolder->setTouchEnabled(true);
		skipHolder->addTouchEventListener([this](Ref* pSender, ui::Widget::TouchEventType eType){
			if(eType == ui::Widget::TouchEventType::ENDED)
			{
				if(_delegate)
				{
					if(_flowData->getUserType() == UserType::REGISTERED)
					{
						_delegate->moveToState(FlowState::SUCCESS);
					}
				}
			}
		});
		skipHolder->addChild(skip);
		_verticalLayout->addChild(skipHolder);
	}
	
	Label* needHelp = Label::createWithTTF(_("Need help?"), Style::Font::Regular(), 64);
	needHelp->setTextColor(Color4B(Style::Color::skyBlue));
	needHelp->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_LEFT);
	needHelp->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	Label* contactUs = Label::createWithTTF(_("Contact us"), Style::Font::Regular(), 64);
	contactUs->setTextColor(Color4B(Style::Color::skyBlue));
	contactUs->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_RIGHT);
	contactUs->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
		
	DrawNode* underline = DrawNode::create();
	underline->drawRect(Vec2(0, -7), Vec2(contactUs->getContentSize().width, -6), Color4F(Style::Color::skyBlue));
	contactUs->addChild(underline);
		
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
	
	_voucherInput->focusAndShowKeyboard();
}

void VodacomOnboardingErrorLayer::setupForLoginError()
{
	const Size& contentSize = getContentSize();
	
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
	buttonHolder->setContentSize(Size(contentSize.width, closeButton->getContentSize().height));
	_verticalLayout->addChild(buttonHolder);
	
	buttonHolder->addChild(closeButton);
	buttonHolder->addChild(backButton);
	
	Label* title = Label::createWithTTF(_("Oops!"), Style::Font::Regular(), 96);
	title->setTextColor(Color4B::BLACK);
	title->setHorizontalAlignment(TextHAlignment::CENTER);
	title->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	title->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	
	ui::Layout* titleHolder = ui::Layout::create();
	titleHolder->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
	titleHolder->setContentSize(title->getContentSize());
	titleHolder->addChild(title);
	_verticalLayout->addChild(titleHolder);
	
	Label* subHeading = Label::createWithTTF(_("There was a problem with your email or password."), Style::Font::Regular(), 64);
	subHeading->setTextColor(Color4B::BLACK);
	subHeading->setHorizontalAlignment(TextHAlignment::CENTER);
	subHeading->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	subHeading->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	subHeading->setWidth(contentSize.width * 0.7f);
	
	ui::Layout* subHeadingHolder = ui::Layout::create();
	subHeadingHolder->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,100,0,0)));
	subHeadingHolder->setContentSize(subHeading->getContentSize());
	subHeadingHolder->addChild(subHeading);
	_verticalLayout->addChild(subHeadingHolder);
	
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
	_verticalLayout->addChild(tryAgainButton);
	
	Label* tryAgainText = Label::createWithTTF(_("Try again"), Style::Font::Regular(), tryAgainButton->getContentSize().height * 0.5f);
	tryAgainText->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	tryAgainText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	tryAgainText->setHorizontalAlignment(TextHAlignment::CENTER);
	tryAgainText->setVerticalAlignment(TextVAlignment::CENTER);
	tryAgainText->setDimensions(tryAgainButton->getContentSize().width * 0.8f, tryAgainButton->getContentSize().height);
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
	_verticalLayout->addChild(resetButton);
	
	Label* resetText = Label::createWithTTF(_("Reset password"), Style::Font::Regular(), resetButton->getContentSize().height * 0.5f);
	resetText->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	resetText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	resetText->setHorizontalAlignment(TextHAlignment::CENTER);
	resetText->setVerticalAlignment(TextVAlignment::CENTER);
	resetText->setDimensions(resetButton->getContentSize().width * 0.8f, resetButton->getContentSize().height * 0.8f);
	resetButton->addChild(resetText);
	
	Label* needHelp = Label::createWithTTF(_("Need help?"), Style::Font::Regular(), 64);
	needHelp->setTextColor(Color4B(Style::Color::skyBlue));
	needHelp->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_LEFT);
	needHelp->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	Label* contactUs = Label::createWithTTF(_("Contact us"), Style::Font::Regular(), 64);
	contactUs->setTextColor(Color4B(Style::Color::skyBlue));
	contactUs->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_RIGHT);
	contactUs->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
	
	DrawNode* underline = DrawNode::create();
	underline->drawRect(Vec2(0, -7), Vec2(contactUs->getContentSize().width, -6), Color4F(Style::Color::skyBlue));
	contactUs->addChild(underline);
	
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
}

void VodacomOnboardingErrorLayer::setupForPasswordReset()
{
	const Size& contentSize = getContentSize();
	
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
	buttonHolder->setContentSize(Size(contentSize.width, closeButton->getContentSize().height));
	_verticalLayout->addChild(buttonHolder);
	
	buttonHolder->addChild(closeButton);
	buttonHolder->addChild(backButton);
	
	Label* title = Label::createWithTTF(_("Reset requested"), Style::Font::Regular(), 96);
	title->setTextColor(Color4B::BLACK);
	title->setHorizontalAlignment(TextHAlignment::CENTER);
	title->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	title->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	
	ui::Layout* titleHolder = ui::Layout::create();
	titleHolder->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
	titleHolder->setContentSize(title->getContentSize());
	titleHolder->addChild(title);
	_verticalLayout->addChild(titleHolder);
	
	Label* subHeading = Label::createWithTTF(_("Instructions for resetting your password have been sent to:"), Style::Font::Regular(), 64);
	subHeading->setTextColor(Color4B::BLACK);
	subHeading->setHorizontalAlignment(TextHAlignment::CENTER);
	subHeading->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	subHeading->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	subHeading->setWidth(contentSize.width * 0.7f);
	
	ui::Layout* subHeadingHolder = ui::Layout::create();
	subHeadingHolder->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,100,0,0)));
	subHeadingHolder->setContentSize(subHeading->getContentSize());
	subHeadingHolder->addChild(subHeading);
	_verticalLayout->addChild(subHeadingHolder);
	
	Label* email = Label::createWithTTF(_flowData->getEmail(), Style::Font::Regular(), 64);
	email->setTextColor(Color4B(Style::Color::skyBlue));
	email->setHorizontalAlignment(TextHAlignment::CENTER);
	email->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	email->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	
	ui::Layout* emailHolder = ui::Layout::create();
	emailHolder->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,100,0,0)));
	emailHolder->setContentSize(email->getContentSize());
	emailHolder->addChild(email);
	_verticalLayout->addChild(emailHolder);
	
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
	_verticalLayout->addChild(okButton);
	
	Label* okText = Label::createWithTTF(_("OK"), Style::Font::Regular(), okButton->getContentSize().height * 0.5f);
	okText->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	okText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	okText->setHorizontalAlignment(TextHAlignment::CENTER);
	okText->setVerticalAlignment(TextVAlignment::CENTER);
	okText->setDimensions(okButton->getContentSize().width * 0.8f, okButton->getContentSize().height);
	okButton->addChild(okText);
	
	Label* notRecieved = Label::createWithTTF(_("Didn't recieve an email?"), Style::Font::Regular(), 64);
	notRecieved->setTextColor(Color4B(Style::Color::skyBlue));
	notRecieved->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_LEFT);
	notRecieved->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	Label* sendAgain = Label::createWithTTF(_("Send again"), Style::Font::Regular(), 64);
	sendAgain->setTextColor(Color4B(Style::Color::skyBlue));
	sendAgain->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_RIGHT);
	sendAgain->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
	
	DrawNode* underline = DrawNode::create();
	underline->drawRect(Vec2(0, -7), Vec2(sendAgain->getContentSize().width, -6), Color4F(Style::Color::skyBlue));
	sendAgain->addChild(underline);
	
	ui::Layout* sendAgainHolder = ui::Layout::create();
	sendAgainHolder->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,100,0,0)));
	sendAgainHolder->setContentSize(Size(notRecieved->getContentSize().width + sendAgain->getContentSize().width + 20, sendAgain->getContentSize().height));
	sendAgainHolder->setTouchEnabled(true);
	sendAgainHolder->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
		if(eType == ui::Widget::TouchEventType::ENDED)
		{
			ModalMessages::getInstance()->startLoading();
			HttpRequestCreator* request = API::ResetPaswordRequest(_flowData->getEmail(), this);
			request->execute();
		}
	});
	sendAgainHolder->addChild(notRecieved);
	sendAgainHolder->addChild(sendAgain);
	_verticalLayout->addChild(sendAgainHolder);
	
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
}

void VodacomOnboardingErrorLayer::setupForAlreadyRegistered()
{
	const Size& contentSize = getContentSize();
	
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
	buttonHolder->setContentSize(Size(contentSize.width, closeButton->getContentSize().height));
	_verticalLayout->addChild(buttonHolder);
	
	buttonHolder->addChild(closeButton);
	buttonHolder->addChild(backButton);
	
	Label* title = Label::createWithTTF(_("Already registered"), Style::Font::Regular(), 96);
	title->setTextColor(Color4B::BLACK);
	title->setHorizontalAlignment(TextHAlignment::CENTER);
	title->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	title->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	
	ui::Layout* titleHolder = ui::Layout::create();
	titleHolder->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
	titleHolder->setContentSize(title->getContentSize());
	titleHolder->addChild(title);
	_verticalLayout->addChild(titleHolder);
	
	Label* subHeading = Label::createWithTTF(_("This email address has already been registered. Please create a new account with a new email address or log in using this email address."), Style::Font::Regular(), 64);
	subHeading->setTextColor(Color4B::BLACK);
	subHeading->setHorizontalAlignment(TextHAlignment::CENTER);
	subHeading->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	subHeading->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	subHeading->setWidth(contentSize.width * 0.7f);
	
	ui::Layout* subHeadingHolder = ui::Layout::create();
	subHeadingHolder->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,100,0,0)));
	subHeadingHolder->setContentSize(subHeading->getContentSize());
	subHeadingHolder->addChild(subHeading);
	_verticalLayout->addChild(subHeadingHolder);
	
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
	_verticalLayout->addChild(okButton);
	
	Label* okText = Label::createWithTTF(_("Sign up"), Style::Font::Regular(), okButton->getContentSize().height * 0.5f);
	okText->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	okText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	okText->setHorizontalAlignment(TextHAlignment::CENTER);
	okText->setVerticalAlignment(TextVAlignment::CENTER);
	okText->setDimensions(okButton->getContentSize().width * 0.8f, okButton->getContentSize().height);
	okButton->addChild(okText);
	
	ui::Button* loginButton = ui::Button::create("res/vodacom/main_button.png");
	loginButton->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,100,0,0)));
	loginButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
		if(eType == ui::Widget::TouchEventType::ENDED)
		{
			if(_delegate)
			{
				_flowData->popState();
				_delegate->moveToState(FlowState::LOGIN);
			}
		}
	});
	_verticalLayout->addChild(loginButton);
	
	Label* loginText = Label::createWithTTF(_("Log in"), Style::Font::Regular(), loginButton->getContentSize().height * 0.5f);
	loginText->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	loginText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	loginText->setHorizontalAlignment(TextHAlignment::CENTER);
	loginText->setVerticalAlignment(TextVAlignment::CENTER);
	loginText->setDimensions(loginButton->getContentSize().width * 0.8f, loginButton->getContentSize().height);
	loginButton->addChild(loginText);
	
	Label* needHelp = Label::createWithTTF(_("Need help?"), Style::Font::Regular(), 64);
	needHelp->setTextColor(Color4B(Style::Color::skyBlue));
	needHelp->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_LEFT);
	needHelp->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	Label* contactUs = Label::createWithTTF(_("Contact us"), Style::Font::Regular(), 64);
	contactUs->setTextColor(Color4B(Style::Color::skyBlue));
	contactUs->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_RIGHT);
	contactUs->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
	
	DrawNode* underline = DrawNode::create();
	underline->drawRect(Vec2(0, -7), Vec2(contactUs->getContentSize().width, -6), Color4F(Style::Color::skyBlue));
	contactUs->addChild(underline);
	
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
}

void VodacomOnboardingErrorLayer::setupForAlreadyPremium()
{
	const Size& contentSize = getContentSize();
	
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
				_delegate->moveToState(FlowState::EXIT);
			}
		}
	});
	
	ui::Layout* buttonHolder = ui::Layout::create();
	buttonHolder->setContentSize(Size(contentSize.width, closeButton->getContentSize().height));
	_verticalLayout->addChild(buttonHolder);
	
	buttonHolder->addChild(closeButton);
	buttonHolder->addChild(backButton);
	
	Label* title = Label::createWithTTF(_("You Can't Do That"), Style::Font::Regular(), 96);
	title->setTextColor(Color4B::BLACK);
	title->setHorizontalAlignment(TextHAlignment::CENTER);
	title->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	title->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	
	ui::Layout* titleHolder = ui::Layout::create();
	titleHolder->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
	titleHolder->setContentSize(title->getContentSize());
	titleHolder->addChild(title);
	_verticalLayout->addChild(titleHolder);
	
	Label* subHeading = Label::createWithTTF(_("This account is already Premium, you can't add another voucher at this time."), Style::Font::Regular(), 64);
	subHeading->setTextColor(Color4B::BLACK);
	subHeading->setHorizontalAlignment(TextHAlignment::CENTER);
	subHeading->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	subHeading->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	subHeading->setWidth(contentSize.width * 0.7f);
	
	ui::Layout* subHeadingHolder = ui::Layout::create();
	subHeadingHolder->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,100,0,0)));
	subHeadingHolder->setContentSize(subHeading->getContentSize());
	subHeadingHolder->addChild(subHeading);
	_verticalLayout->addChild(subHeadingHolder);
	
	ui::Button* okButton = ui::Button::create("res/vodacom/main_button.png");
	okButton->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,100,0,0)));
	okButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
		if(eType == ui::Widget::TouchEventType::ENDED)
		{
			if(_delegate)
			{
				_delegate->moveToState(FlowState::EXIT);
			}
		}
	});
	_verticalLayout->addChild(okButton);
	
	Label* okText = Label::createWithTTF(_("OK"), Style::Font::Regular(), okButton->getContentSize().height * 0.5f);
	okText->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	okText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	okText->setHorizontalAlignment(TextHAlignment::CENTER);
	okText->setVerticalAlignment(TextVAlignment::CENTER);
	okText->setDimensions(okButton->getContentSize().width * 0.8f, okButton->getContentSize().height);
	okButton->addChild(okText);
	
	Label* needHelp = Label::createWithTTF(_("Need help?"), Style::Font::Regular(), 64);
	needHelp->setTextColor(Color4B(Style::Color::skyBlue));
	needHelp->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_LEFT);
	needHelp->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	Label* contactUs = Label::createWithTTF(_("Contact us"), Style::Font::Regular(), 64);
	contactUs->setTextColor(Color4B(Style::Color::skyBlue));
	contactUs->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_RIGHT);
	contactUs->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
	
	DrawNode* underline = DrawNode::create();
	underline->drawRect(Vec2(0, -7), Vec2(contactUs->getContentSize().width, -6), Color4F(Style::Color::skyBlue));
	contactUs->addChild(underline);
	
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
}

void VodacomOnboardingErrorLayer::setupForGenericError()
{
	const Size& contentSize = getContentSize();
	
	ui::Button* closeButton = ui::Button::create("res/vodacom/close.png");
	closeButton->setLayoutParameter(CreateRightLinearLayoutParam());
	closeButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
		if(eType == ui::Widget::TouchEventType::ENDED)
		{
			if(_delegate)
			{
				_delegate->moveToState(FlowState::EXIT);
			}
		}
	});
	_verticalLayout->addChild(closeButton);

	Label* title = Label::createWithTTF(_("Oops!"), Style::Font::Regular(), 96);
	title->setTextColor(Color4B::BLACK);
	title->setHorizontalAlignment(TextHAlignment::CENTER);
	title->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	title->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	
	ui::Layout* titleHolder = ui::Layout::create();
	titleHolder->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,((_flowData->getUserType() == UserType::FREE) ? 0 : 200),0, 0)));
	titleHolder->setContentSize(title->getContentSize());
	titleHolder->addChild(title);
	_verticalLayout->addChild(titleHolder);
	
	Label* subHeading = Label::createWithTTF(_("There was a problem. Please try again later or contact customer support"), Style::Font::Regular(), 64);
	subHeading->setTextColor(Color4B::BLACK);
	subHeading->setHorizontalAlignment(TextHAlignment::CENTER);
	subHeading->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	subHeading->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	subHeading->setWidth(contentSize.width * 0.7f);
	
	ui::Layout* subHeadingHolder = ui::Layout::create();
	subHeadingHolder->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,100,0,0)));
	subHeadingHolder->setContentSize(subHeading->getContentSize());
	subHeadingHolder->addChild(subHeading);
	_verticalLayout->addChild(subHeadingHolder);
	
	Label* helpLink = Label::createWithTTF(_("help@azoomee.com"), Style::Font::Regular(), 64);
	helpLink->setTextColor(Color4B::BLACK);
	helpLink->setHorizontalAlignment(TextHAlignment::CENTER);
	helpLink->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	helpLink->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	
	DrawNode* contactDrawNode = DrawNode::create();
	contactDrawNode->drawRect(Vec2(0, -7), Vec2(helpLink->getContentSize().width, -6), Color4F::BLACK);
	helpLink->addChild(contactDrawNode);
	
	helpLink->setWidth(contentSize.width * 0.9f);
	
	ui::Layout* helpLinkHolder = ui::Layout::create();
	helpLinkHolder->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,20,0,0)));
	helpLinkHolder->setContentSize(helpLink->getContentSize());
	helpLinkHolder->setTouchEnabled(true);
	helpLinkHolder->addTouchEventListener([](Ref* pSender, ui::Widget::TouchEventType eType){
		if(eType == ui::Widget::TouchEventType::ENDED)
		{
			Application::getInstance()->openURL(_("mailto:help@azoomee.com"));
		}
	});
	helpLinkHolder->addChild(helpLink);
	_verticalLayout->addChild(helpLinkHolder);
}

void VodacomOnboardingErrorLayer::onVoucherEntered()
{
	if(_delegate && _voucherInput->inputIsValid())
	{
		if(_flowData->getUserType() == UserType::FREE)
		{
			ModalMessages::getInstance()->startLoading();
			HttpRequestCreator* request = API::AddVoucher(ParentManager::getInstance()->getLoggedInParentId(), _voucherInput->getText(), this);
			request->execute();
		}
		else
		{
			_flowData->setVoucherCode(_voucherInput->getText());
			_delegate->moveToState(FlowState::REGISTER);
		}
	}
}

//Delegate Functions
void VodacomOnboardingErrorLayer::onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body)
{
	if(requestTag == API::TagAddVoucher)
	{
		AnalyticsSingleton::getInstance()->vodacomOnboardingVoucherAdded(_flowData->getVoucherCode());
		HttpRequestCreator* request = API::UpdateBillingDataRequest(ParentManager::getInstance()->getLoggedInParentId(), this);
		request->execute();
	}
	else if(requestTag == API::TagUpdateBillingData)
	{
		ParentManager::getInstance()->parseParentBillingData(body);
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

void VodacomOnboardingErrorLayer::textInputIsValid(TextInputLayer* inputLayer, bool isValid)
{
	auto errorMsg = inputLayer->getChildByName("error");
	if(errorMsg)
	{
		errorMsg->setVisible(!isValid);
	}
	_flowData->setVoucherCode(inputLayer->getText());
}
void VodacomOnboardingErrorLayer::textInputReturnPressed(TextInputLayer* inputLayer)
{
	if(inputLayer == _voucherInput)
	{
		_flowData->setVoucherCode(inputLayer->getText());
		this->runAction(Sequence::create(DelayTime::create(0.1f), CallFunc::create([&](){
			this->onVoucherEntered();
		}),NULL));
	}
}
void VodacomOnboardingErrorLayer::editBoxEditingDidBegin(TextInputLayer* inputLayer)
{
	
}
void VodacomOnboardingErrorLayer::editBoxEditingDidEnd(TextInputLayer* inputLayer)
{
	
}

void VodacomOnboardingErrorLayer::onButtonPressed(SettingsMessageBox *pSender, SettingsMessageBoxButtonType type)
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
