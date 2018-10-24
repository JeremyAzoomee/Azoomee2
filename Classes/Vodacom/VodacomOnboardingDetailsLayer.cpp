//
//  VodacomOnboardingDetailsLayer.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 11/10/2018.
//

#include "VodacomOnboardingDetailsLayer.h"
#include <AzoomeeCommon/UI/LayoutParams.h>
#include <AzoomeeCommon/Strings.h>
#include <AzoomeeCommon/UI/Style.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool VodacomOnboardingDetailsLayer::init()
{
	if(!Super::init())
	{
		return false;
	}
	
	return true;
}

void VodacomOnboardingDetailsLayer::onEnter()
{
	_closeButton = ui::Button::create("res/vodacom/close.png");
	_closeButton->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
	_closeButton->setLayoutParameter(CreateRightLinearLayoutParam());
	_closeButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
		if(eType == ui::Widget::TouchEventType::ENDED)
		{
			if(_delegate)
			{
				_delegate->moveToState(FlowState::EXIT);
			}
		}
	});
	_verticalLayout->addChild(_closeButton);
	
	_image = ui::ImageView::create("res/vodacom/content_img.png");
	_image->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
	_verticalLayout->addChild(_image);
	
	Label* title = Label::createWithTTF(_("Unlock everything"), Style::Font::Regular, 96);
	title->setHorizontalAlignment(TextHAlignment::CENTER);
	title->setTextColor(Color4B::BLACK);
	title->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	title->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	title->setWidth(this->getContentSize().width * 0.8f);
	
	ui::Layout* titleHolder = ui::Layout::create();
	titleHolder->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
	titleHolder->setContentSize(title->getContentSize());
	_verticalLayout->addChild(titleHolder);
	titleHolder->addChild(title);
	
	const std::vector<std::string> bulletPoints = {_("Hundreds of games and videos"),_("No in-app purchases or ads"),_("Personalise your profile"),_("Chat safely with friends and family")};
	
	for(const std::string& text : bulletPoints)
	{
		Label* bullet = Label::createWithTTF(text, Style::Font::Regular, 64);
		bullet->setHorizontalAlignment(TextHAlignment::LEFT);
		bullet->setTextColor(Color4B::BLACK);
		bullet->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		bullet->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
		bullet->setWidth(this->getContentSize().width * 0.7f);
		
		Sprite* tick = Sprite::create("res/vodacom/tick.png");
		tick->setAnchorPoint(Vec2(2.0,0.5));
		tick->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_LEFT);
		bullet->addChild(tick);
		
		ui::Layout* holder = ui::Layout::create();
		holder->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(tick->getContentSize().width,100,0,0)));
		holder->setContentSize(bullet->getContentSize());
		_verticalLayout->addChild(holder);
		holder->addChild(bullet);
	}
	
	_addVoucherButton = ui::Button::create("res/vodacom/main_button.png");
	_addVoucherButton->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,100,0,0)));
	_addVoucherButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
		if(eType == ui::Widget::TouchEventType::ENDED)
		{
			if(_delegate)
			{
				_delegate->moveToState(FlowState::ADD_VOUCHER);
			}
		}
	});
	_verticalLayout->addChild(_addVoucherButton);
	
	Label* addVoucherText = Label::createWithTTF(_("Add voucher"), Style::Font::Regular, _addVoucherButton->getContentSize().height * 0.5f);
	addVoucherText->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	addVoucherText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	addVoucherText->setHorizontalAlignment(TextHAlignment::CENTER);
	addVoucherText->setVerticalAlignment(TextVAlignment::CENTER);
	addVoucherText->setDimensions(_addVoucherButton->getContentSize().width * 0.8f, _addVoucherButton->getContentSize().height);
	_addVoucherButton->addChild(addVoucherText);
	
	Label* voucherLearnMore = Label::createWithTTF(_("Don’t have a voucher?"), Style::Font::Regular, 64);
	voucherLearnMore->setTextColor(Color4B(Style::Color::skyBlue));
	voucherLearnMore->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_LEFT);
	voucherLearnMore->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	Label* learnMore = Label::createWithTTF(_("Learn more"), Style::Font::Regular, 64);
	learnMore->setTextColor(Color4B(Style::Color::skyBlue));
	learnMore->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_RIGHT);
	learnMore->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
	
	DrawNode* underline = DrawNode::create();
	underline->drawRect(Vec2(0, -7), Vec2(learnMore->getContentSize().width, -6), Color4F(Style::Color::skyBlue));
	learnMore->addChild(underline);
	
	ui::Layout* learnMoreHolder = ui::Layout::create();
	learnMoreHolder->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,100,0,0)));
	learnMoreHolder->setContentSize(Size(voucherLearnMore->getContentSize().width + learnMore->getContentSize().width + 20, learnMore->getContentSize().height));
	learnMoreHolder->addChild(voucherLearnMore);
	learnMoreHolder->addChild(learnMore);
	_verticalLayout->addChild(learnMoreHolder);
	
	Super::onEnter();
}

NS_AZOOMEE_END
