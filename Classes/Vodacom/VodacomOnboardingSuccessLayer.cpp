//
//  VodacomOnboardingSuccessLayer.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 11/10/2018.
//

#include "VodacomOnboardingSuccessLayer.h"
#include <AzoomeeCommon/UI/LayoutParams.h>
#include <AzoomeeCommon/Data/Parent/ParentDataProvider.h>
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/Strings.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool VodacomOnboardingSuccessLayer::init()
{
	if(!Super::init())
	{
		return false;
	}
	
	setLayoutType(ui::Layout::Type::VERTICAL);
	
	return true;
}

void VodacomOnboardingSuccessLayer::onEnter()
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
	this->addChild(_closeButton);
	
	Label* title = Label::createWithTTF(_("Success!"), Style::Font::Regular, 96);
	title->setTextColor(Color4B::BLACK);
	title->setHorizontalAlignment(TextHAlignment::CENTER);
	title->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	title->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	
	ui::Layout* titleHolder = ui::Layout::create();
	titleHolder->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
	titleHolder->setContentSize(title->getContentSize());
	titleHolder->addChild(title);
	this->addChild(titleHolder);
	
	Label* subHeading = Label::createWithTTF(_("Your voucher has been added,"), Style::Font::Regular, 64);
	subHeading->setTextColor(Color4B::BLACK);
	subHeading->setHorizontalAlignment(TextHAlignment::CENTER);
	subHeading->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	subHeading->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	
	ui::Layout* subHeadingHolder = ui::Layout::create();
	subHeadingHolder->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
	subHeadingHolder->setContentSize(subHeading->getContentSize());
	subHeadingHolder->addChild(subHeading);
	this->addChild(subHeadingHolder);
	
	Label* validUntil = Label::createWithTTF(_("valid until") + " " + ParentDataProvider::getInstance()->getBillingDate() , Style::Font::Regular, 64);
	validUntil->setTextColor(Color4B::BLACK);
	validUntil->setHorizontalAlignment(TextHAlignment::CENTER);
	validUntil->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	validUntil->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	
	ui::Layout* validUntilHolder = ui::Layout::create();
	validUntilHolder->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
	validUntilHolder->setContentSize(validUntil->getContentSize());
	validUntilHolder->addChild(validUntil);
	this->addChild(validUntilHolder);
	
	ui::ImageView* oomee = ui::ImageView::create("res/vodacom/success_image.png");
	oomee->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,200,0,0)));
	this->addChild(oomee);
	
	Super::onEnter();
}

NS_AZOOMEE_END
