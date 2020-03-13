//
//  VodacomOnboardingLoginLayer.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 11/10/2018.
//
#ifdef AZOOMEE_VODACOM_BUILD
#include "VodacomOnboardingLoginLayer.h"
#include <TinizineCommon/Utils/LocaleManager.h>
#include <TinizineCommon/UI/Colour.h>
#include <TinizineCommon/UI/LayoutParams.h>
#include <TinizineCommon/API/API.h>
#include "ModalMessages.h"
#include <TinizineCommon/Data/Parent/UserAccountManager.h>
#include <TinizineCommon/Analytics/AnalyticsSingleton.h>
#include "VodacomMessageBoxNotification.h"

using namespace cocos2d;

USING_NS_TZ

NS_AZ_BEGIN

bool VodacomOnboardingLoginLayer::init()
{
	if(!Super::init())
	{
		return false;
	}
	
	return true;
}

void VodacomOnboardingLoginLayer::onEnter()
{
	const Size& contentSize = getContentSize();
	
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
	buttonHolder->setContentSize(Size(contentSize.width, _closeButton->getContentSize().height));
	_verticalLayout->addChild(buttonHolder);
	
	buttonHolder->addChild(_closeButton);
	buttonHolder->addChild(_backButton);
	
	Label* title = Label::createWithTTF(_("Log in"), Style::Font::Regular(), 96);
	title->setTextColor(Color4B::BLACK);
	title->setHorizontalAlignment(TextHAlignment::CENTER);
	title->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	title->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	
	ui::Layout* titleHolder = ui::Layout::create();
	titleHolder->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
	titleHolder->setContentSize(title->getContentSize());
	titleHolder->addChild(title);
	_verticalLayout->addChild(titleHolder);
	
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
	emailError->setTextColor(Color4B(Colours::Color_3B::watermelon));
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
	
	_passwordInput = TextInputLayer::createSettingsRoundedTextInput(contentSize.width * 0.6f, INPUT_IS_PASSWORD);
	_passwordInput->setCenterPosition(_passwordInput->getContentSize() / 2.0f);
	_passwordInput->setDelegate(this);
	_passwordInput->setText(_flowData->getPassword());
	
	Label* pwError = Label::createWithTTF(_("*Invalid password"), Style::Font::Regular(), 53);
	pwError->setTextColor(Color4B(Colours::Color_3B::watermelon));
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
	
	Label* confirmText = Label::createWithTTF(_("Confirm"), Style::Font::Regular(), _confirmButton->getContentSize().height * 0.5f);
	confirmText->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	confirmText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	confirmText->setHorizontalAlignment(TextHAlignment::CENTER);
	confirmText->setVerticalAlignment(TextVAlignment::CENTER);
	confirmText->setDimensions(_confirmButton->getContentSize().width * 0.8f, _confirmButton->getContentSize().height);
	_confirmButton->addChild(confirmText);
	
	Label* needHelp = Label::createWithTTF(_("Need help?"), Style::Font::Regular(), 64);
	needHelp->setTextColor(Color4B(Colours::Color_3B::skyBlue));
	needHelp->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_LEFT);
	needHelp->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	Label* contactUs = Label::createWithTTF(_("Contact us"), Style::Font::Regular(), 64);
	contactUs->setTextColor(Color4B(Colours::Color_3B::skyBlue));
	contactUs->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_RIGHT);
	contactUs->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
	
	DrawNode* underline = DrawNode::create();
	underline->drawRect(Vec2(0, -7), Vec2(contactUs->getContentSize().width, -6), Color4F(Colours::Color_3B::skyBlue));
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
	
	Super::onEnter();
	
	_emailInput->focusAndShowKeyboard();
}

void VodacomOnboardingLoginLayer::onConfirmPressed()
{
	auto emailErrorMsg = _emailInput->getChildByName("error");
	if(emailErrorMsg)
	{
		emailErrorMsg->setVisible(!_emailInput->inputIsValid());
	}
	auto pwErrorMsg = _passwordInput->getChildByName("error");
	if(pwErrorMsg)
	{
		pwErrorMsg->setVisible(!_passwordInput->inputIsValid());
	}
	
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
		if(UserAccountManager::getInstance()->parseParentLoginData(body))
		{
			UserAccountManager::getInstance()->setHasLoggedInOnDevice(true);
			UserAccountManager::getInstance()->setLoggedInParentCountryCode(getValueFromHttpResponseHeaderForKey(API::kAZCountryCodeKey, headers));
			AnalyticsSingleton::getInstance()->signInSuccessEvent();
			AnalyticsSingleton::getInstance()->setIsUserAnonymous(false);
			_flowData->setUserType(UserType::FREE);
			UserDefault* def = UserDefault::getInstance();
			def->setStringForKey(UserAccountManager::kStoredUsernameKey, _flowData->getEmail());
			def->flush();
			HttpRequestCreator* request = API::UpdateBillingDataRequest(UserAccountManager::getInstance()->getLoggedInParentId(), this);
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
		UserAccountManager::getInstance()->parseParentBillingData(body);
		if(!UserAccountManager::getInstance()->isPaidUser())
		{
			ModalMessages::getInstance()->stopLoading();
			VodacomMessageBoxNotification* messageBox = VodacomMessageBoxNotification::create();
			messageBox->setHeading(_("Logged in"));
			Director::getInstance()->getRunningScene()->addChild(messageBox);
			this->runAction(Sequence::createWithTwoActions(DelayTime::create(4.0f), CallFunc::create([messageBox, this](){
				messageBox->removeFromParent();
				ModalMessages::getInstance()->startLoading();
				if(_flowData->getPurchaseType() == PurchaseType::VOUCHER)
				{
					HttpRequestCreator* request = API::AddVoucher(UserAccountManager::getInstance()->getLoggedInParentId(), _flowData->getVoucherCode(), this);
					request->execute();
				}
				else
				{
					HttpRequestCreator* request = API::GetVodacomTransactionId(UserAccountManager::getInstance()->getLoggedInParentId(), this);
					request->execute();
				}
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
		AnalyticsSingleton::getInstance()->vodacomOnboardingVoucherAdded(_flowData->getVoucherCode());
		HttpRequestCreator* request = API::UpdateBillingDataRequest(UserAccountManager::getInstance()->getLoggedInParentId(), this);
		request->_requestTag = "billingAfterVoucher";
		request->execute();
	}
	else if(requestTag == "billingAfterVoucher")
	{
		UserAccountManager::getInstance()->parseParentBillingData(body);
		ModalMessages::getInstance()->stopLoading();
		if(_delegate)
		{
			_delegate->moveToState(FlowState::SUCCESS);
		}
	}
	else if(requestTag == API::TagGetVodacomTransactionId)
	{
		ModalMessages::getInstance()->stopLoading();
		if(_delegate)
		{
			rapidjson::Document data;
			data.Parse(body.c_str());
			if(data.HasParseError())
			{
				return;
			}
			_flowData->setTransactionId(getStringFromJson("id", data));
			_delegate->moveToState(FlowState::DCB_WEBVIEW);
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
	else if(requestTag == API::TagGetVodacomTransactionId)
	{
		log("transaction id request failed");
		_flowData->setErrorType(ErrorType::GENERIC);
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

NS_AZ_END
#endif
