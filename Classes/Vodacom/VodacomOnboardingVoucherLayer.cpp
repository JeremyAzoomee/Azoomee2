//
//  VodacomOnboardingVoucherLayer.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 11/10/2018.
//

#include "VodacomOnboardingVoucherLayer.h"
#include <AzoomeeCommon/UI/LayoutParams.h>
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/Strings.h>
#include <AzoomeeCommon/API/API.h>
#include <AzoomeeCommon/Data/Parent/ParentDataProvider.h>
#include <AzoomeeCommon/UI/ModalMessages.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool VodacomOnboardingVoucherLayer::init()
{
	if(!Super::init())
	{
		return false;
	}
	
	setLayoutType(ui::Layout::Type::VERTICAL);
	
	return true;
}

void VodacomOnboardingVoucherLayer::onEnter()
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
	
	Label* title = Label::createWithTTF(_("Add a voucher"), Style::Font::Regular, 96);
	title->setTextColor(Color4B::BLACK);
	title->setHorizontalAlignment(TextHAlignment::CENTER);
	title->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	title->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	
	ui::Layout* titleHolder = ui::Layout::create();
	titleHolder->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
	titleHolder->setContentSize(title->getContentSize());
	titleHolder->addChild(title);
	this->addChild(titleHolder);
	
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
	
	ui::ImageView* oomee = ui::ImageView::create("res/vodacom/orange_oomee.png");
	oomee->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,300,0,0)));
	this->addChild(oomee);
	
	Super::onEnter();
}

void VodacomOnboardingVoucherLayer::onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body)
{
	if(_delegate)
	{
		_delegate->moveToState(FlowState::SUCCESS);
	}
	ModalMessages::getInstance()->stopLoading();
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

NS_AZOOMEE_END
