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
#include <AzoomeeCommon/Data/Parent/ParentDataParser.h>
#include "../ChildCreator.h"
#include "VodacomMessageBoxInfo.h"
#include "VodacomMessageBoxNotification.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool VodacomOnboardingAddChildLayer::init()
{
	if(!Super::init())
	{
		return false;
	}
	
	return true;
}

void VodacomOnboardingAddChildLayer::onEnter()
{
	Label* title = Label::createWithTTF(_("Setup a profile"), Style::Font::Regular, 96);
	title->setTextColor(Color4B::BLACK);
	title->setHorizontalAlignment(TextHAlignment::CENTER);
	title->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	title->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	
	ui::Layout* titleHolder = ui::Layout::create();
	titleHolder->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,200,0,0)));
	titleHolder->setContentSize(title->getContentSize());
	titleHolder->addChild(title);
	_verticalLayout->addChild(titleHolder);
	
	Label* subTitle = Label::createWithTTF(_("In order to get going you’ll need to set up a child profile. Don’t worry, you can add more children later."), Style::Font::Regular, 64);
	subTitle->setTextColor(Color4B::BLACK);
	subTitle->setWidth(this->getContentSize().width * 0.65f);
	subTitle->setHorizontalAlignment(TextHAlignment::CENTER);
	subTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	subTitle->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	
	ui::Layout* subTitleHolder = ui::Layout::create();
	subTitleHolder->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,100,0,0)));
	subTitleHolder->setContentSize(subTitle->getContentSize());
	subTitleHolder->addChild(subTitle);
	_verticalLayout->addChild(subTitleHolder);
	
	Label* inputTitle = Label::createWithTTF(_("Child's name"), Style::Font::Regular, 64);
	inputTitle->setTextColor(Color4B::BLACK);
	inputTitle->setHorizontalAlignment(TextHAlignment::CENTER);
	inputTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	inputTitle->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	
	ui::Layout* inputTitleHolder = ui::Layout::create();
	inputTitleHolder->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,100,0,0)));
	inputTitleHolder->setContentSize(inputTitle->getContentSize());
	inputTitleHolder->addChild(inputTitle);
	_verticalLayout->addChild(inputTitleHolder);
	
	_nameInput = TextInputLayer::createSettingsRoundedTextInput(this->getContentSize().width * 0.6f, INPUT_IS_CHILD_NAME);
	_nameInput->setCenterPosition(_nameInput->getContentSize() / 2.0f);
	_nameInput->setDelegate(this);
	_nameInput->setText(_flowData->getChildName());
	
	Label* nameError = Label::createWithTTF(_("*Invalid name"), Style::Font::Regular, 53);
	nameError->setTextColor(Color4B(Style::Color::watermelon));
	nameError->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	nameError->setNormalizedPosition(Vec2(0.1f,-0.1));
	nameError->setName("error");
	nameError->setVisible(false);
	_nameInput->addChild(nameError);
	
	ui::Layout* inputLayout = ui::Layout::create();
	inputLayout->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,100,0,0)));
	inputLayout->setContentSize(_nameInput->getContentSize());
	inputLayout->addChild(_nameInput);
	_verticalLayout->addChild(inputLayout);
	
	Label* ageInputTitle = Label::createWithTTF(_("Age"), Style::Font::Regular, 64);
	ageInputTitle->setTextColor(Color4B::BLACK);
	ageInputTitle->setHorizontalAlignment(TextHAlignment::CENTER);
	ageInputTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	ageInputTitle->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	
	ui::Layout* ageInputTitleHolder = ui::Layout::create();
	ageInputTitleHolder->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,100,0,0)));
	ageInputTitleHolder->setContentSize(ageInputTitle->getContentSize());
	ageInputTitleHolder->addChild(ageInputTitle);
	_verticalLayout->addChild(ageInputTitleHolder);
	
	Label* detailsLink = Label::createWithTTF(_("Why do we need this?"), Style::Font::Regular, 64);
	detailsLink->setTextColor(Color4B(Style::Color::skyBlue));
	float textWidth = detailsLink->getContentSize().width;
	if(textWidth > this->getContentSize().width * 0.65f)
	{
		detailsLink->setWidth(this->getContentSize().width * 0.65f);
	}
	detailsLink->setHorizontalAlignment(TextHAlignment::CENTER);
	detailsLink->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	detailsLink->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	
	DrawNode* underline = DrawNode::create();
	underline->drawRect(Vec2(0, -7), Vec2(detailsLink->getContentSize().width, -6), Color4F(Style::Color::skyBlue));
	detailsLink->addChild(underline);
	
	ui::Layout* detailsLinkHolder = ui::Layout::create();
	detailsLinkHolder->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,100,0,0)));
	detailsLinkHolder->setContentSize(detailsLink->getContentSize());
	detailsLinkHolder->setTouchEnabled(true);
	detailsLinkHolder->addTouchEventListener([this](Ref* pSender, ui::Widget::TouchEventType eType){
		if(eType == ui::Widget::TouchEventType::ENDED)
		{
			VodacomMessageBoxInfo* messageBox = VodacomMessageBoxInfo::create();
			messageBox->setDelegate(this);
			messageBox->setHeading(_("We need your child’s age so we can create a profile with age-appropriate content."));
			Director::getInstance()->getRunningScene()->addChild(messageBox);
		}
	});
	detailsLinkHolder->addChild(detailsLink);
	_verticalLayout->addChild(detailsLinkHolder);
	
	_ageInput = TextInputLayer::createSettingsRoundedTextInput(this->getContentSize().width * 0.6f, INPUT_IS_AGE);
	_ageInput->setCenterPosition(_ageInput->getContentSize() / 2.0f);
	_ageInput->setDelegate(this);
	_ageInput->setText(_flowData->getChildAge());
	
	Label* ageError = Label::createWithTTF(_("*Invalid age"), Style::Font::Regular, 53);
	ageError->setTextColor(Color4B(Style::Color::watermelon));
	ageError->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	ageError->setNormalizedPosition(Vec2(0.1f,-0.1));
	ageError->setName("error");
	ageError->setVisible(false);
	_ageInput->addChild(ageError);
	
	ui::Layout* ageInputLayout = ui::Layout::create();
	ageInputLayout->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,100,0,0)));
	ageInputLayout->setContentSize(_ageInput->getContentSize());
	ageInputLayout->addChild(_ageInput);
	_verticalLayout->addChild(ageInputLayout);
	
	_confirmButton = ui::Button::create("res/vodacom/main_button.png");
	_confirmButton->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,100,0,0)));
	_confirmButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
		if(eType == ui::Widget::TouchEventType::ENDED)
		{
			this->onConfirmPressed();
		}
	});
	_verticalLayout->addChild(_confirmButton);
	
	Label* confirmText = Label::createWithTTF(_("Complete"), Style::Font::Regular, _confirmButton->getContentSize().height * 0.5f);
	confirmText->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	confirmText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	confirmText->setHorizontalAlignment(TextHAlignment::CENTER);
	confirmText->setVerticalAlignment(TextVAlignment::CENTER);
	confirmText->setDimensions(_confirmButton->getContentSize().width * 0.8f, _confirmButton->getContentSize().height);
	_confirmButton->addChild(confirmText);
	
	ui::ImageView* progressIcon = ui::ImageView::create("res/vodacom/step_counter_4.png");
	progressIcon->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,200,0,0)));
	_verticalLayout->addChild(progressIcon);
	
	Label* needHelp = Label::createWithTTF(_("Need help?"), Style::Font::Regular, 64);
	needHelp->setTextColor(Color4B(Style::Color::skyBlue));
	needHelp->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_LEFT);
	needHelp->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	Label* contactUs = Label::createWithTTF(_("Contact us"), Style::Font::Regular, 64);
	contactUs->setTextColor(Color4B(Style::Color::skyBlue));
	contactUs->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_RIGHT);
	contactUs->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
	
	DrawNode* underline2 = DrawNode::create();
	underline2->drawRect(Vec2(0, -7), Vec2(contactUs->getContentSize().width, -6), Color4F(Style::Color::skyBlue));
	contactUs->addChild(underline2);
	
	ui::Layout* contactUsHolder = ui::Layout::create();
	contactUsHolder->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_BOTTOM);
	contactUsHolder->setAnchorPoint(Vec2(0.5f,-1.0f));
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
}

void VodacomOnboardingAddChildLayer::onConfirmPressed()
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

//Delegate Functions
void VodacomOnboardingAddChildLayer::onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body)
{
	if(requestTag == API::TagRegisterChild)
	{
		HttpRequestCreator* request = API::GetAvailableChildrenRequest(this);
		request->execute();
	}
	else if(requestTag == API::TagGetAvailableChildren)
	{
		ModalMessages::getInstance()->stopLoading();
		ParentDataParser::getInstance()->parseAvailableChildren(body);
		VodacomMessageBoxNotification* messageBox = VodacomMessageBoxNotification::create();
		messageBox->setHeading(StringUtils::format("%s %s",_("Profile created for").c_str(),_flowData->getChildName().c_str()));
		messageBox->setDelegate(this);
		Director::getInstance()->getRunningScene()->addChild(messageBox);
		this->runAction(Sequence::createWithTwoActions(DelayTime::create(4.0f), CallFunc::create([messageBox, this](){
			messageBox->removeFromParent();
			if(_delegate)
			{
				_delegate->moveToState(FlowState::SUCCESS);
			}
		})));
	}
}
void VodacomOnboardingAddChildLayer::onHttpRequestFailed(const std::string& requestTag, long errorCode)
{
	ModalMessages::getInstance()->stopLoading();
}

void VodacomOnboardingAddChildLayer::textInputIsValid(TextInputLayer* inputLayer, bool isValid)
{
	auto errorMsg = inputLayer->getChildByName("error");
	if(errorMsg)
	{
		errorMsg->setVisible(!isValid);
	}
	if(inputLayer == _nameInput)
	{
		_flowData->setChildName(inputLayer->getText());
	}
	else if(inputLayer == _ageInput)
	{
		_flowData->setChildAge(inputLayer->getText());
	}
}
void VodacomOnboardingAddChildLayer::textInputReturnPressed(TextInputLayer* inputLayer)
{
	if(inputLayer == _nameInput)
	{
		_flowData->setChildName(inputLayer->getText());
		_ageInput->focusAndShowKeyboard();
	}
	else if(inputLayer == _ageInput)
	{
		_flowData->setChildAge(_ageInput->getText());
		this->runAction(Sequence::create(DelayTime::create(0.1f), CallFunc::create([&](){
			this->onConfirmPressed();
		}),NULL));
	}
}
void VodacomOnboardingAddChildLayer::editBoxEditingDidBegin(TextInputLayer* inputLayer)
{
	
}
void VodacomOnboardingAddChildLayer::editBoxEditingDidEnd(TextInputLayer* inputLayer)
{
	
}

void VodacomOnboardingAddChildLayer::onButtonPressed(SettingsMessageBox *pSender, SettingsMessageBoxButtonType type)
{
	pSender->removeFromParent();
}

NS_AZOOMEE_END
