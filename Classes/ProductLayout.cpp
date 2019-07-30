//
//  ProductLayout.cpp
//  Azoomee
//
//  Created by Macauley on 11/07/2019.
//

#include "ProductLayout.h"
#include <AzoomeeCommon/UI/LayoutParams.h>
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/Strings.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool ProductLayout::init()
{
	if(!Super::init())
	{
		return false;
	}
	
	setBackGroundColorType(BackGroundColorType::SOLID);
	setBackGroundColor(Color3B::WHITE);
	setLayoutType(Type::VERTICAL);
	
	_productBanner = ui::Layout::create();
	_productBanner->setSizeType(SizeType::PERCENT);
	_productBanner->setSizePercent(Vec2(1.0f,0.62f));
	addChild(_productBanner);
	
	ui::Layout* buttonHolder = ui::Layout::create();
	buttonHolder->setSizeType(SizeType::PERCENT);
	buttonHolder->setSizePercent(Vec2(1.0f,0.38f));
	buttonHolder->setLayoutType(Type::VERTICAL);
	addChild(buttonHolder);
	
	_purchaseButton = CTAButton::create("res/onboarding/rounded_button.png");
	_purchaseButton->ignoreContentAdaptWithSize(false);
	_purchaseButton->setContentSize(Size(700,140));
	_purchaseButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_purchaseButton->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
	_purchaseButton->setColor(Style::Color::darkIndigo);
	_purchaseButton->setTextColour(Color4B::WHITE);
	_purchaseButton->setTextFontInfo(Style::Font::PoppinsBold(), 70);
	_purchaseButton->setTextAreaSizePercent(Vec2(0.9f,0.8f));
	_purchaseButton->addTouchEventListener([this](Ref* pSender, ui::Widget::TouchEventType eType){
		if(eType == ui::Widget::TouchEventType::ENDED)
		{
			if(_callback)
			{
				_callback(IAPAction::PURCHASE);
			}
		}
	});
	buttonHolder->addChild(_purchaseButton);
	
	_restoreButton = DynamicText::create(_("Restore Purchase"), Style::Font::PoppinsBold(), 60);
	_restoreButton->ignoreContentAdaptWithSize(false);
	_restoreButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_restoreButton->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
	_restoreButton->setTextHorizontalAlignment(TextHAlignment::CENTER);
	_restoreButton->setTextVerticalAlignment(TextVAlignment::CENTER);
	_restoreButton->setTextAreaSize(Size(700,140));
	_restoreButton->setTextColor(Color4B(130,130,130,255));
	_restoreButton->setTouchEnabled(true);
	_restoreButton->setOverflow(Label::Overflow::SHRINK);
	_restoreButton->addTouchEventListener([this](Ref* pSender, ui::Widget::TouchEventType eType){
		switch(eType)
		{
			case cocos2d::ui::Widget::TouchEventType::BEGAN:
			{
				_restoreButton->setScale(1.05f);
				break;
			}
			case cocos2d::ui::Widget::TouchEventType::MOVED:
			{
				break;
			}
			case cocos2d::ui::Widget::TouchEventType::ENDED:
			{
				_restoreButton->setScale(1.0f);
				if(_callback)
				{
					_callback(IAPAction::RESTORE);
				}
				break;
			}
			case cocos2d::ui::Widget::TouchEventType::CANCELED:
			{
				_restoreButton->setScale(1.0f);
				break;
			}
		}
	});
	
	buttonHolder->addChild(_restoreButton);
	
	return true;
}
void ProductLayout::onEnter()
{
	Super::onEnter();
	setupProductBanner();
}
void ProductLayout::onExit()
{
	Super::onExit();
}
void ProductLayout::onSizeChanged()
{
	Super::onSizeChanged();
	setupProductBanner();
}

void ProductLayout::setProductLayoutType(const ProductLayoutType& type)
{
	_type = type;
}
void ProductLayout::setProductData(const ProductDataList& dataList)
{
	_productData = dataList;
}
void ProductLayout::setIapActionCallback(const IAPActionCallback& callback)
{
	_callback = callback;
}

void ProductLayout::setupProductBanner()
{
	_productBanner->removeAllChildren();
	if(_productData.size() == 0)
	{
		return;
	}
	switch(_type)
	{
		case ProductLayoutType::PASS:
		{
			ui::Layout* textLayout = ui::Layout::create();
			textLayout->setSizeType(SizeType::PERCENT);
			textLayout->setSizePercent(Vec2(1.0f,0.1f));
			textLayout->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
			textLayout->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
			_productBanner->addChild(textLayout);
			
			ui::Text* desc = ui::Text::create(_productData.at(0).first, Style::Font::PoppinsRegular(), 50);
			desc->setTextColor(Color4B(130,130,130,255));
			desc->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
			desc->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_TOP);
			desc->setTextHorizontalAlignment(TextHAlignment::CENTER);
			desc->setTextVerticalAlignment(TextVAlignment::BOTTOM);
			Label* descLab = dynamic_cast<Label*>(desc->getVirtualRenderer());
			if(descLab)
			{
				descLab->setMaxLineWidth(_productBanner->getContentSize().width * 0.7f);
				descLab->setOverflow(Label::Overflow::RESIZE_HEIGHT);
			}
			textLayout->addChild(desc);
			
			ui::Text* value = ui::Text::create(_productData.at(0).second, Style::Font::PoppinsMedium(), 175);
			value->setTextColor(Color4B::BLACK);
			value->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
			value->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_BOTTOM);
			value->setTextHorizontalAlignment(TextHAlignment::CENTER);
			value->setTextVerticalAlignment(TextVAlignment::TOP);
			Label* valueLab = dynamic_cast<Label*>(value->getVirtualRenderer());
			if(valueLab)
			{
				valueLab->setMaxLineWidth(_productBanner->getContentSize().width * 0.7f);
			}
			textLayout->addChild(value);
			
			_purchaseButton->setText(_("Buy Azoomee Pass"));
			
			break;
		}
		case ProductLayoutType::SUBSCRIPTION:
		{
			ui::Layout* textLayout = ui::Layout::create();
			textLayout->setSizeType(SizeType::PERCENT);
			textLayout->setSizePercent(Vec2(1.0f,0.0f));
			textLayout->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			textLayout->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
			_productBanner->addChild(textLayout);
			
			ui::Text* freeTrial = ui::Text::create(_productData.at(0).first, Style::Font::PoppinsMedium(), 105);
			freeTrial->setTextColor(Color4B::BLACK);
			freeTrial->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
			freeTrial->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_BOTTOM);
			freeTrial->setTextHorizontalAlignment(TextHAlignment::CENTER);
			freeTrial->setTextVerticalAlignment(TextVAlignment::TOP);
			Label* freeTrialLab = dynamic_cast<Label*>(freeTrial->getVirtualRenderer());
			if(freeTrialLab)
			{
				freeTrialLab->setMaxLineWidth(_productBanner->getContentSize().width * 0.7f);
			}
			textLayout->addChild(freeTrial);
			
			ui::Text* startYour = ui::Text::create(_("Start your"), Style::Font::Regular(), 50);
			startYour->setTextColor(Color4B(130,130,130,255));
			startYour->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
			startYour->setNormalizedPosition(Vec2(0.5,1.1));
			startYour->setTextHorizontalAlignment(TextHAlignment::CENTER);
			startYour->setTextVerticalAlignment(TextVAlignment::BOTTOM);
			Label* startYourLab = dynamic_cast<Label*>(startYour->getVirtualRenderer());
			if(startYourLab)
			{
				startYourLab->setMaxLineWidth(_productBanner->getContentSize().width * 0.7f);
				startYourLab->setOverflow(Label::Overflow::RESIZE_HEIGHT);
			}
			freeTrial->addChild(startYour);
			
			ui::Text* desc = ui::Text::create(StringUtils::format(_("Then %s per month. Cancel anytime.").c_str(),_productData.at(0).second.c_str()), Style::Font::PoppinsRegular(), 50);
			desc->setTextColor(Color4B(130,130,130,255));
			desc->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
			desc->setNormalizedPosition(Vec2(0.5,-0.1));
			desc->setTextHorizontalAlignment(TextHAlignment::CENTER);
			desc->setTextVerticalAlignment(TextVAlignment::BOTTOM);
			Label* descLab = dynamic_cast<Label*>(desc->getVirtualRenderer());
			if(descLab)
			{
				descLab->setMaxLineWidth(_productBanner->getContentSize().width * 0.7f);
				descLab->setOverflow(Label::Overflow::RESIZE_HEIGHT);
			}
			freeTrial->addChild(desc);
			
			_purchaseButton->setText(_("Subscribe now"));
			
			break;
		}
		case ProductLayoutType::MULI_PASS:
		{
			break;
		}
	}
}


NS_AZOOMEE_END
