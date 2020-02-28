//
//  VodacomOnboardingDetailsLayer.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 11/10/2018.
//
#ifdef AZOOMEE_VODACOM_BUILD
#include "VodacomOnboardingDetailsLayer.h"
#include <TinizineCommon/UI/LayoutParams.h>
#include <TinizineCommon/Utils/LocaleManager.h>
#include <TinizineCommon/UI/Colour.h>

using namespace cocos2d;

USING_NS_TZ

NS_AZ_BEGIN

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
	const Size& contentSize = getContentSize();
	
	_flowData->setPurchaseType(PurchaseType::UNKNOWN);
	
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
	
	Label* title = Label::createWithTTF(_("Unlock everything"), Style::Font::Regular(), 96);
	title->setHorizontalAlignment(TextHAlignment::CENTER);
	title->setTextColor(Color4B::BLACK);
	title->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	title->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	title->setWidth(contentSize.width * 0.8f);
	
	ui::Layout* titleHolder = ui::Layout::create();
	titleHolder->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
	titleHolder->setContentSize(title->getContentSize());
	_verticalLayout->addChild(titleHolder);
	titleHolder->addChild(title);
	
	const std::vector<std::string> bulletPoints = {_("Hundreds of games and videos"),_("No in-app purchases or ads"),_("Personalise your profile"),_("Chat safely with friends and family")};
	
	for(const std::string& text : bulletPoints)
	{
		Label* bullet = Label::createWithTTF(text, Style::Font::Regular(), 64);
		bullet->setHorizontalAlignment(TextHAlignment::LEFT);
		bullet->setTextColor(Color4B::BLACK);
		bullet->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		bullet->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
		bullet->setWidth(contentSize.width * 0.7f);
		
		Sprite* tick = Sprite::create("res/vodacom/tick.png");
		tick->setAnchorPoint(Vec2(2.0,0.5));
		tick->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_LEFT);
		bullet->addChild(tick);
		
		ui::Layout* holder = ui::Layout::create();
		holder->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(tick->getContentSize().width * 1.5f,100,0,0)));
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
				_flowData->setPurchaseType(PurchaseType::VOUCHER);
				_delegate->moveToState(FlowState::ADD_VOUCHER);
			}
		}
	});
	_verticalLayout->addChild(_addVoucherButton);
	
	Label* addVoucherText = Label::createWithTTF(_("Add voucher"), Style::Font::Regular(), _addVoucherButton->getContentSize().height * 0.5f);
	addVoucherText->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	addVoucherText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	addVoucherText->setHorizontalAlignment(TextHAlignment::CENTER);
	addVoucherText->setVerticalAlignment(TextVAlignment::CENTER);
	addVoucherText->setOverflow(Label::Overflow::SHRINK);
	addVoucherText->setDimensions(_addVoucherButton->getContentSize().width * 0.8f, _addVoucherButton->getContentSize().height * 0.8f);
	_addVoucherButton->addChild(addVoucherText);
	
	ui::Button* choosePlanButton = ui::Button::create("res/vodacom/main_button.png");
	choosePlanButton->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,100,0,0)));
	choosePlanButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
		if(eType == ui::Widget::TouchEventType::ENDED)
		{
			if(_delegate)
			{
				_flowData->setPurchaseType(PurchaseType::DCB);
				_delegate->moveToState(FlowState::DCB_WEBVIEW);
			}
		}
	});
	_verticalLayout->addChild(choosePlanButton);
	
	Label* choosePlanText = Label::createWithTTF(_("Choose a plan"), Style::Font::Regular(), choosePlanButton->getContentSize().height * 0.5f);
	choosePlanText->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	choosePlanText->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	choosePlanText->setHorizontalAlignment(TextHAlignment::CENTER);
	choosePlanText->setVerticalAlignment(TextVAlignment::CENTER);
	choosePlanText->setOverflow(Label::Overflow::SHRINK);
	choosePlanText->setDimensions(choosePlanButton->getContentSize().width * 0.8f, choosePlanButton->getContentSize().height * 0.9f);
	choosePlanButton->addChild(choosePlanText);
	
	// add this in later when we have a link
	/*Label* voucherLearnMore = Label::createWithTTF(_("Don’t have a voucher?"), Style::Font::Regular(), 64);
	voucherLearnMore->setTextColor(Color4B(Colours::Color_3B::skyBlue));
	voucherLearnMore->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_LEFT);
	voucherLearnMore->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	Label* learnMore = Label::createWithTTF(_("Learn more"), Style::Font::Regular(), 64);
	learnMore->setTextColor(Color4B(Colours::Color_3B::skyBlue));
	learnMore->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_RIGHT);
	learnMore->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
	
	DrawNode* underline = DrawNode::create();
	underline->drawRect(Vec2(0, -7), Vec2(learnMore->getContentSize().width, -6), Color4F(Colours::Color_3B::skyBlue));
	learnMore->addChild(underline);
	
	ui::Layout* learnMoreHolder = ui::Layout::create();
	learnMoreHolder->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,100,0,0)));
	learnMoreHolder->setContentSize(Size(voucherLearnMore->getContentSize().width + learnMore->getContentSize().width + 20, learnMore->getContentSize().height));
	learnMoreHolder->addChild(voucherLearnMore);
	learnMoreHolder->addChild(learnMore);
	_verticalLayout->addChild(learnMoreHolder);*/
	
	Super::onEnter();
}

NS_AZ_END
#endif
