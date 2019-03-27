//
//  ShopItemPurchasedAnimation.cpp
//  azoomee2
//
//  Created by Macauley on 13/03/2019.
//

#include "ShopItemPurchasedAnimation.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool ShopItemPurchasedAnimation::init()
{
	if(!Super::init())
	{
		return false;
	}
	
	this->setContentSize(Director::getInstance()->getVisibleSize());
	
	_beam = Sprite::create("res/shop/beam.png");
	_beam->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	_beam->setNormalizedPosition(Vec2(0.5,0.0));
	this->addChild(_beam);
	
	_clippingStencil = ui::Scale9Sprite::create("res/shop/glitch_mask_white.png");
	_clippingStencil->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
	
	_clippingNode = ClippingNode::create(_clippingStencil);
	_clippingNode->setAlphaThreshold(0.5f);
	_clippingNode->setContentSize(this->getContentSize());
	this->addChild(_clippingNode);
	
	_itemAsset = RemoteImageSprite::create();
	_itemAsset->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	_itemAsset->setPosition(Vec2(this->getContentSize().width / 2, _beam->getContentSize().height / 2));
	_itemAsset->setKeepAspectRatio(true);
	_clippingNode->addChild(_itemAsset);
	
	return true;
}
void ShopItemPurchasedAnimation::onEnter()
{
	if(_itemData)
	{
		_itemAsset->initWithUrlAndSizeWithoutPlaceholder(_itemData->getInventoryItem()->getUri(), _beam->getContentSize());
		_itemAsset->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
		_itemAsset->startLoadingImage();
		_clippingStencil->setContentSize(_itemAsset->getContentSize() * 1.25);
		_clippingStencil->setPosition(_itemAsset->getPosition());
		_clippingStencil->runAction(Sequence::create(DelayTime::create(0.5),MoveBy::create(3, Vec2(0,+ _clippingStencil->getContentSize().height)),NULL));
	}
	
	this->runAction(Sequence::createWithTwoActions(DelayTime::create(5.0f), CallFunc::create([this](){
		if(_onCompleteCallback)
		{
			_onCompleteCallback();
		}
	})));
	
	Super::onEnter();
}
void ShopItemPurchasedAnimation::onExit()
{
	Super::onExit();
}

void ShopItemPurchasedAnimation::setItemData(const ShopDisplayItemRef& itemData)
{
	_itemData = itemData;
}
void ShopItemPurchasedAnimation::setOnCompleteCallback(const OnCompleteCallback& callback)
{
	_onCompleteCallback = callback;
}

NS_AZOOMEE_END
