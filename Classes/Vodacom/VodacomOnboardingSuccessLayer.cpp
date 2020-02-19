//
//  VodacomOnboardingSuccessLayer.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 11/10/2018.
//
#ifdef AZOOMEE_VODACOM_BUILD
#include "VodacomOnboardingSuccessLayer.h"
#include <AzoomeeCommon/UI/LayoutParams.h>
#include <AzoomeeCommon/Data/Parent/UserAccountManager.h>
#include <AzoomeeCommon/UI/Colour.h>
#include <AzoomeeCommon/Utils/LocaleManager.h>

using namespace cocos2d;

USING_NS_TZ

NS_AZ_BEGIN

bool VodacomOnboardingSuccessLayer::init()
{
	if(!Super::init())
	{
		return false;
	}
	
	return true;
}

void VodacomOnboardingSuccessLayer::onEnter()
{
	const Size& contentSize = getContentSize();
	
	Label* title = Label::createWithTTF(_("Success!"), Style::Font::Regular(), 96);
	title->setTextColor(Color4B::BLACK);
	title->setHorizontalAlignment(TextHAlignment::CENTER);
	title->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	title->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	
	ui::Layout* titleHolder = ui::Layout::create();
	titleHolder->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,200,0,0)));
	titleHolder->setContentSize(title->getContentSize());
	titleHolder->addChild(title);
	_verticalLayout->addChild(titleHolder);
	
	if(UserAccountManager::getInstance()->isPaidUser())
	{
		Label* validUntil = Label::createWithTTF(_("Set up complete. Your voucher has been added, valid until") + " " + UserAccountManager::getInstance()->getBillingDate() , Style::Font::Regular(), 64);
		validUntil->setTextColor(Color4B::BLACK);
		validUntil->setHorizontalAlignment(TextHAlignment::CENTER);
		validUntil->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		validUntil->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
		validUntil->setWidth(contentSize.width * 0.75f);
		
		ui::Layout* validUntilHolder = ui::Layout::create();
		validUntilHolder->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,100,0,0)));
		validUntilHolder->setContentSize(validUntil->getContentSize());
		validUntilHolder->addChild(validUntil);
		_verticalLayout->addChild(validUntilHolder);
	}
	else
	{
		Label* subHeading = Label::createWithTTF(_("Set up complete. You’ll need to add a valid voucher later for full access."), Style::Font::Regular(), 64);
		subHeading->setTextColor(Color4B::BLACK);
		subHeading->setHorizontalAlignment(TextHAlignment::CENTER);
		subHeading->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
		subHeading->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
		subHeading->setWidth(contentSize.width * 0.75f);
		
		ui::Layout* subHeadingHolder = ui::Layout::create();
		subHeadingHolder->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,100,0,0)));
		subHeadingHolder->setContentSize(subHeading->getContentSize());
		subHeadingHolder->addChild(subHeading);
		_verticalLayout->addChild(subHeadingHolder);
	}
	
	ui::Button* letsGoButton = ui::Button::create("res/vodacom/main_button.png");
	letsGoButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	letsGoButton->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,100,0,0)));
	letsGoButton->addTouchEventListener([&](Ref* pSender, ui::Widget::TouchEventType eType){
		if(eType == ui::Widget::TouchEventType::ENDED)
		{
			if(_delegate)
			{
				_delegate->moveToState(FlowState::EXIT);
			}
		}
	});
	_verticalLayout->addChild(letsGoButton);
	
	Label* letsGoLabel = Label::createWithTTF(_("Let's go"), Style::Font::Regular(), letsGoButton->getContentSize().height * 0.5f);
	letsGoLabel->setTextColor(Color4B::WHITE);
	letsGoLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	letsGoLabel->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	letsGoLabel->setHorizontalAlignment(TextHAlignment::CENTER);
	letsGoLabel->setVerticalAlignment(TextVAlignment::CENTER);
	letsGoLabel->setOverflow(Label::Overflow::SHRINK);
	letsGoLabel->setDimensions(letsGoButton->getContentSize().width * 0.8f, letsGoButton->getContentSize().height);
	letsGoButton->addChild(letsGoLabel);
	
	ui::ImageView* oomee = ui::ImageView::create("res/vodacom/success_image.png");
	oomee->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0,200,0,0)));
	_verticalLayout->addChild(oomee);
	
	Super::onEnter();
}

NS_AZ_END
#endif
