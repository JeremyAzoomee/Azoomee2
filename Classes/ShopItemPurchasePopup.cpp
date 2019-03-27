//
//  ShopItemPurchasePopup.cpp
//  azoomee2
//
//  Created by Macauley on 12/03/2019.
//

#include "ShopItemPurchasePopup.h"
#include <AzoomeeCommon/UI/Style.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool ShopItemPurchasePopup::init()
{
	if(!Super::init())
	{
		return false;
	}
	this->setContentSize(Size(1457, 947));
	this->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	
	setBackGroundImage("res/shop/rectangle.png");
	
	_itemAsset = RemoteImageSprite::create();
	_itemAsset->setNormalizedPosition(Vec2(0.32, 0.5));
	_itemAsset->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_itemAsset->setKeepAspectRatio(true);
	this->addChild(_itemAsset);
	
	_closeButton = ui::Button::create("res/shop/close_popup.png");
	_closeButton->setAnchorPoint(Vec2(1.25,1.25));
	_closeButton->setNormalizedPosition(Vec2::ANCHOR_TOP_RIGHT);
	_closeButton->addTouchEventListener([this](Ref* pSender, ui::Widget::TouchEventType eType){
		if(eType == ui::Widget::TouchEventType::ENDED)
		{
			if(_purchaseCallback)
			{
				_purchaseCallback(_itemData, false);
			}
		}
	});
	this->addChild(_closeButton);
	
	_valueLayout = ui::Layout::create();
	
	_cost = Label::createWithTTF("0", Style::Font::Bold(), 96);
	_cost->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
	_cost->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_RIGHT);
	
	_valueLayout->addChild(_cost);
	
	_coin = Sprite::create("res/shop/coin_small.png");
	_coin->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	_coin->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_LEFT);
	
	_valueLayout->addChild(_coin);
	_valueLayout->setContentSize(_cost->getContentSize() + Size(_coin->getContentSize().width * 1.5f,0));
	_valueLayout->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	_valueLayout->setNormalizedPosition(Vec2(0.66,0.55));
	this->addChild(_valueLayout);
	
	_buyButton = ui::Button::create("res/shop/cta.png");
	_buyButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
	_buyButton->setNormalizedPosition(Vec2(0.66,0.45));
	_buyButton->addTouchEventListener([this](Ref* pSender, ui::Widget::TouchEventType eType){
		if(eType == ui::Widget::TouchEventType::ENDED)
		{
			if(_purchaseCallback)
			{
				_purchaseCallback(_itemData, true);
			}
		}
	});
	this->addChild(_buyButton);
	
	return true;
}
void ShopItemPurchasePopup::onEnter()
{
	Super::onEnter();
}
void ShopItemPurchasePopup::onExit()
{
	Super::onExit();
}

void ShopItemPurchasePopup::setItemData(const ShopDisplayItemRef& itemData)
{
	_itemData = itemData;
	refreshUI();
}
void ShopItemPurchasePopup::setPurchaseCallback(const PurchaseCallback& callback)
{
	_purchaseCallback = callback;
}

ShopDisplayItemRef ShopItemPurchasePopup::getItemData() const
{
	return _itemData;
}

void ShopItemPurchasePopup::refreshUI()
{
	if(_itemData)
	{
		_itemAsset->removeLoadedImage();
		_itemAsset->initWithUrlAndSizeWithoutPlaceholder(_itemData->getInventoryItem()->getUri(), this->getContentSize() * 0.4f);
		_itemAsset->startLoadingImage();
		_cost->setString(StringUtils::format("%d",_itemData->getPrice()));
		_valueLayout->setContentSize(_cost->getContentSize() + Size(_coin->getContentSize().width * 1.5f,0));
	}
	else
	{
		_itemAsset->initWithUrlAndSizeWithoutPlaceholder("", this->getContentSize() * 0.4f);
		_cost->setString(StringUtils::format("%d",0));
		_valueLayout->setContentSize(_cost->getContentSize() + Size(_coin->getContentSize().width * 1.5f,0));
	}
}

NS_AZOOMEE_END
