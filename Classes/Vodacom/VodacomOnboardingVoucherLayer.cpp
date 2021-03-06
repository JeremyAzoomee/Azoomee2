//
//  VodacomOnboardingVoucherLayer.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 11/10/2018.
//
#ifdef AZOOMEE_VODACOM_BUILD
#include "VodacomOnboardingVoucherLayer.h"
#include <AzoomeeCommon/UI/LayoutParams.h>
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/Strings.h>
#include <AzoomeeCommon/API/API.h>
#include <AzoomeeCommon/Data/Parent/ParentManager.h>
#include <AzoomeeCommon/UI/ModalMessages.h>
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool VodacomOnboardingVoucherLayer::init()
{
	if(!Super::init())
	{
		return false;
	}
	
	return true;
}

void VodacomOnboardingVoucherLayer::onEnter()
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
				_flowData->setVoucherCode(_voucherInput->getText());
				_delegate->moveToPreviousState();
			}
		}
	});
	
	ui::Layout* buttonHolder = ui::Layout::create();
	buttonHolder->setContentSize(Size(contentSize.width, _closeButton->getContentSize().height));
	_verticalLayout->addChild(buttonHolder);
	
	buttonHolder->addChild(_closeButton);
	buttonHolder->addChild(_backButton);
	
	Label* title = Label::createWithTTF(_("Add a voucher"), Style::Font::Regular(), 96);
	title->setTextColor(Color4B::BLACK);
	title->setHorizontalAlignment(TextHAlignment::CENTER);
	title->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	title->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	
	ui::Layout* titleHolder = ui::Layout::create();
	titleHolder->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
	titleHolder->setContentSize(title->getContentSize());
	titleHolder->addChild(title);
	_verticalLayout->addChild(titleHolder);
	
	const std::string& promotionString = _("vodacomPromotionalVoucherString");
	if(promotionString != "" && promotionString != "vodacomPromotionalVoucherString")
	{
		Label* inputTitle = Label::createWithTTF(promotionString, Style::Font::Regular(), 64);
		inputTitle->setTextColor(Color4B::BLACK);
		inputTitle->setHorizontalAlignment(TextHAlignment::CENTER);
		inputTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		inputTitle->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
		inputTitle->setWidth(contentSize.width * 0.8f);
		
		ui::Layout* inputTitleHolder = ui::Layout::create();
		inputTitleHolder->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,25,0,0)));
		inputTitleHolder->setContentSize(inputTitle->getContentSize());
		inputTitleHolder->addChild(inputTitle);
		_verticalLayout->addChild(inputTitleHolder);
	}
	else
	{
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
	}
	_voucherInput = TextInputLayer::createSettingsRoundedTextInput(contentSize.width * 0.6f, INPUT_IS_VOUCHER);
	_voucherInput->setCenterPosition(_voucherInput->getContentSize() / 2.0f);
	_voucherInput->setDelegate(this);
	_voucherInput->setText(_flowData->getVoucherCode());
	
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
	
	ui::ImageView* oomee = ui::ImageView::create("res/vodacom/voda_oomee.png");
	oomee->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,100,0,0)));
	_verticalLayout->addChild(oomee);
	
	if(_flowData->getUserType() != UserType::FREE)
	{
		ui::ImageView* progress = ui::ImageView::create("res/vodacom/step_counter_1.png");
		progress->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,100,0,0)));
		_verticalLayout->addChild(progress);
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
	
	Super::onEnter();
	
	_voucherInput->focusAndShowKeyboard();
}

void VodacomOnboardingVoucherLayer::onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body)
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
}
void VodacomOnboardingVoucherLayer::onHttpRequestFailed(const std::string& requestTag, long errorCode)
{
	if(_delegate)
	{
		_flowData->setErrorType(ErrorType::VOUCHER);
		_delegate->moveToState(FlowState::ERROR);
	}
	ModalMessages::getInstance()->stopLoading();
}

void VodacomOnboardingVoucherLayer::onConfirmPressed()
{
	auto errorMsg = _voucherInput->getChildByName("error");
	if(errorMsg)
	{
		errorMsg->setVisible(!_voucherInput->inputIsValid());
	}
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
			_delegate->moveToState(FlowState::TERMS);
		}
	}
}

//Delegate Functions

void VodacomOnboardingVoucherLayer::textInputIsValid(TextInputLayer* inputLayer, bool isValid)
{
	auto errorMsg = inputLayer->getChildByName("error");
	if(errorMsg)
	{
		errorMsg->setVisible(!isValid);
	}
	_flowData->setVoucherCode(inputLayer->getText());
}
void VodacomOnboardingVoucherLayer::textInputReturnPressed(TextInputLayer* inputLayer)
{
	_flowData->setVoucherCode(inputLayer->getText());
	this->runAction(Sequence::create(DelayTime::create(0.1f), CallFunc::create([&](){
		this->onConfirmPressed();
	}),NULL));
}
void VodacomOnboardingVoucherLayer::editBoxEditingDidBegin(TextInputLayer* inputLayer)
{
	
}
void VodacomOnboardingVoucherLayer::editBoxEditingDidEnd(TextInputLayer* inputLayer)
{
	
}

NS_AZOOMEE_END
#endif
