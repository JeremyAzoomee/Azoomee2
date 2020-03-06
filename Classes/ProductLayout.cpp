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
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>

#if defined(AZOOMEE_ENVIRONMENT_CI)
#include "RoutePaymentSingleton.h"
#endif

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
                AnalyticsSingleton::getInstance()->genericButtonPressEvent("IAP_Purchase");
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
	_restoreButton->setTextColor(Color4B(Style::Color::brownGrey));
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
                    AnalyticsSingleton::getInstance()->genericButtonPressEvent("IAP_Restore");
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
    if(ConfigStorage::getInstance()->getOSManufacturer() == ConfigStorage::kOSManufacturerAmazon)
    {
        _restoreButton->setVisible(false);
    }
    
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
			desc->setTextColor(Color4B(Style::Color::brownGrey));
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
			textLayout->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
			textLayout->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_TOP);
            textLayout->setLayoutType(Type::VERTICAL);
			_purchaseButton->addChild(textLayout);
			
            ui::RichText* freeTrialText = ui::RichText::create();
            freeTrialText->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
            freeTrialText->setHorizontalAlignment(ui::RichText::HorizontalAlignment::CENTER);
            
            ui::RichElementText* startYour = ui::RichElementText::create(1, Style::Color::brownGrey, 255, _("Start your"), Style::Font::poppinsRegular, 40);
            ui::RichElementText* freeTrial = ui::RichElementText::create(2, Style::Color::black, 255, _(_productData.at(0).first), Style::Font::poppinsMedium, 40);
            ui::RichElementText* then = ui::RichElementText::create(3, Style::Color::brownGrey, 255, _(", then"), Style::Font::poppinsRegular, 40);
            
            freeTrialText->pushBackElement(startYour);
            freeTrialText->pushBackElement(ui::RichElementNewLine::create(4, Color3B::WHITE, 0));
            freeTrialText->pushBackElement(freeTrial);
            freeTrialText->pushBackElement(then);
            
            freeTrialText->setContentSize(Size(0, 120));
            
            textLayout->addChild(freeTrialText);
            
            ui::Layout* priceLayout = ui::Layout::create();
            priceLayout->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0, 100, 0, 0)));
            priceLayout->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
            textLayout->addChild(priceLayout);
            
            ui::Text* price = ui::Text::create(_productData.at(0).second, Style::Font::poppinsMedium, 134);
            price->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
            price->setTextHorizontalAlignment(TextHAlignment::LEFT);
            price->setTextVerticalAlignment(TextVAlignment::BOTTOM);
            price->setNormalizedPosition(Vec2::ANCHOR_BOTTOM_LEFT);
            price->setTextColor(Color4B::BLACK);
            priceLayout->addChild(price);
            
            ui::Text* perMonth = ui::Text::create(_("per month"), Style::Font::poppinsRegular, 40);
            perMonth->setTextColor(Color4B::BLACK);
            perMonth->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_RIGHT);
            perMonth->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
            perMonth->setTextHorizontalAlignment(TextHAlignment::RIGHT);
            perMonth->setTextVerticalAlignment(TextVAlignment::BOTTOM);
            priceLayout->addChild(perMonth);
            
            priceLayout->setContentSize(Size(price->getContentSize().width + perMonth->getContentSize().width + 20, price->getContentSize().height));
            
            ui::Text* cancelAnyTime = ui::Text::create(_("Cancel anytime"), Style::Font::poppinsRegular, 40);
            cancelAnyTime->setTextColor(Color4B(Style::Color::brownGrey));
            cancelAnyTime->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam(ui::Margin(0, -30, 0, 0)));
            cancelAnyTime->setTextHorizontalAlignment(TextHAlignment::CENTER);
            cancelAnyTime->setTextVerticalAlignment(TextVAlignment::TOP);
            
            textLayout->addChild(cancelAnyTime);
            
            textLayout->setContentSize(Size(0, 350 + price->getContentSize().height));
			
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
