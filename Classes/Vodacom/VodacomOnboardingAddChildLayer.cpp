//
//  VodacomOnboardingAddChildLayer.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 11/10/2018.
//

#include "VodacomOnboardingAddChildLayer.h"
#include <AzoomeeCommon/Strings.h>
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/UI/LayoutParams.h>
#include <AzoomeeCommon/API/API.h>
#include <AzoomeeCommon/UI/ModalMessages.h>
#include <AzoomeeCommon/Data/Parent/ParentDataProvider.h>
#include "../ChildCreator.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool VodacomOnboardingAddChildLayer::init()
{
	if(!Super::init())
	{
		return false;
	}
	
	setLayoutType(ui::Layout::Type::VERTICAL);
	
	return true;
}

void VodacomOnboardingAddChildLayer::onEnter()
{
	_closeButton = ui::Button::create("res/vodacom/close.png");
	_closeButton->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
	_closeButton->setNormalizedPosition(Vec2::ANCHOR_TOP_RIGHT);
	_closeButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
		if(eType == ui::Widget::TouchEventType::ENDED)
		{
			ModalMessages::getInstance()->startLoading();
			HttpRequestCreator* request = API::AddVoucher(ParentDataProvider::getInstance()->getLoggedInParentId(), _flowData->getVoucherCode(), this);
			request->execute();
		}
	});
	
	_backButton = ui::Button::create("res/vodacom/back.png");
	_backButton->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	_backButton->setNormalizedPosition(Vec2::ANCHOR_TOP_LEFT);
	_backButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
		if(eType == ui::Widget::TouchEventType::ENDED)
		{
			ModalMessages::getInstance()->startLoading();
			HttpRequestCreator* request = API::AddVoucher(ParentDataProvider::getInstance()->getLoggedInParentId(), _flowData->getVoucherCode(), this);
			request->execute();
		}
	});
	
	ui::Layout* buttonHolder = ui::Layout::create();
	buttonHolder->setContentSize(Size(this->getContentSize().width, _closeButton->getContentSize().height));
	this->addChild(buttonHolder);
	
	buttonHolder->addChild(_closeButton);
	buttonHolder->addChild(_backButton);
	
	Label* title = Label::createWithTTF(_("Setup a profile"), Style::Font::Regular, 96);
	title->setTextColor(Color4B::BLACK);
	title->setHorizontalAlignment(TextHAlignment::CENTER);
	title->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	title->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	
	ui::Layout* titleHolder = ui::Layout::create();
	titleHolder->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
	titleHolder->setContentSize(title->getContentSize());
	titleHolder->addChild(title);
	this->addChild(titleHolder);
	
	Label* inputTitle = Label::createWithTTF(_("Child's name"), Style::Font::Regular, 64);
	inputTitle->setTextColor(Color4B::BLACK);
	inputTitle->setHorizontalAlignment(TextHAlignment::CENTER);
	inputTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	inputTitle->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	
	ui::Layout* inputTitleHolder = ui::Layout::create();
	inputTitleHolder->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,100,0,0)));
	inputTitleHolder->setContentSize(inputTitle->getContentSize());
	inputTitleHolder->addChild(inputTitle);
	this->addChild(inputTitleHolder);
	
	_nameInput = TextInputLayer::createSettingsRoundedTextInput(this->getContentSize().width * 0.6f, INPUT_IS_CHILD_NAME);
	_nameInput->setCenterPosition(_nameInput->getContentSize() / 2.0f);
	
	ui::Layout* inputLayout = ui::Layout::create();
	inputLayout->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,100,0,0)));
	inputLayout->setContentSize(_nameInput->getContentSize());
	inputLayout->addChild(_nameInput);
	this->addChild(inputLayout);
	
	Label* ageInputTitle = Label::createWithTTF(_("Age"), Style::Font::Regular, 64);
	ageInputTitle->setTextColor(Color4B::BLACK);
	ageInputTitle->setHorizontalAlignment(TextHAlignment::CENTER);
	ageInputTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	ageInputTitle->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	
	ui::Layout* ageInputTitleHolder = ui::Layout::create();
	ageInputTitleHolder->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,100,0,0)));
	ageInputTitleHolder->setContentSize(ageInputTitle->getContentSize());
	ageInputTitleHolder->addChild(ageInputTitle);
	this->addChild(ageInputTitleHolder);
	
	_ageInput = TextInputLayer::createSettingsRoundedTextInput(this->getContentSize().width * 0.6f, INPUT_IS_AGE);
	_ageInput->setCenterPosition(_ageInput->getContentSize() / 2.0f);
	
	ui::Layout* ageInputLayout = ui::Layout::create();
	ageInputLayout->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,100,0,0)));
	ageInputLayout->setContentSize(_ageInput->getContentSize());
	ageInputLayout->addChild(_ageInput);
	this->addChild(ageInputLayout);
	
	Label* subTitle = Label::createWithTTF(_("This is to create a profile with age-appropriate content. You can add more children later."), Style::Font::Regular, 64);
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
	
	_confirmButton = ui::Button::create("res/vodacom/main_button.png");
	_confirmButton->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,100,0,0)));
	_confirmButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
		if(eType == ui::Widget::TouchEventType::ENDED)
		{
			if(_delegate && _nameInput->inputIsValid() && _ageInput->inputIsValid())
			{
				ChildCreatorRef childCreator = ChildCreator::create();
				childCreator->setChildName(_nameInput->getText());
				childCreator->setAge(std::atoi(_ageInput->getText().c_str()));
				childCreator->setFirstTime(true);
				childCreator->setHttpRespnseDelegate(this);
				if(childCreator->addChild())
				{
					ModalMessages::getInstance()->startLoading();
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
	
	Super::onEnter();
}

//Delegate Functions
void VodacomOnboardingAddChildLayer::onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body)
{
	if(requestTag == API::TagRegisterChild)
	{
		HttpRequestCreator* request = API::AddVoucher(ParentDataProvider::getInstance()->getLoggedInParentId(), _flowData->getVoucherCode(), this);
		request->execute();
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
void VodacomOnboardingAddChildLayer::onHttpRequestFailed(const std::string& requestTag, long errorCode)
{
	if(requestTag == API::TagAddVoucher)
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
