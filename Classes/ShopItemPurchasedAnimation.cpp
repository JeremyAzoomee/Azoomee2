//
//  ShopItemPurchasedAnimation.cpp
//  azoomee2
//
//  Created by Macauley on 13/03/2019.
//

#include "ShopItemPurchasedAnimation.h"
#include <AzoomeeCommon/Audio/AudioMixer.h>
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/Strings.h>
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include "SceneManagerScene.h"
#include "OomeeMakerDelegate.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool ShopItemPurchasedAnimation::init()
{
	if(!Super::init())
	{
		return false;
	}
	
	const Size& contentSize = Director::getInstance()->getVisibleSize();
	this->setContentSize(contentSize);
	
	_beam = Sprite::create("res/shop/beam.png");
	_beam->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	_beam->setNormalizedPosition(Vec2(0.5,0.0));
	if(_beam->getContentSize().height > (contentSize.height * 0.65))
	{
		_beam->setContentSize(_beam->getContentSize() * ((contentSize.height * 0.65) / _beam->getContentSize().height));
	}
	this->addChild(_beam);
	
	_clippingStencil = ui::Scale9Sprite::create("res/shop/glitch_mask_white.png");
	_clippingStencil->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
	
	_clippingNode = ClippingNode::create(_clippingStencil);
	_clippingNode->setAlphaThreshold(0.5f);
	_clippingNode->setContentSize(contentSize);
	this->addChild(_clippingNode);
	
	_itemAsset = RemoteImageSprite::create();
	_itemAsset->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	_itemAsset->setPosition(Vec2(contentSize.width / 2, _beam->getContentSize().height / 2));
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
		_clippingStencil->runAction(Sequence::create(DelayTime::create(0.5),MoveBy::create(3, Vec2(0, _clippingStencil->getContentSize().height)),NULL));
		AudioMixer::getInstance()->playEffect("Purchase_Animation_Scan.wav");
	}
	
	this->runAction(Sequence::createWithTwoActions(DelayTime::create(5.0f), CallFunc::create([this](){
		if(_itemData)
		{
			//set item meta id in oomee maker delegate
			OomeeMakerDelegate::getInstance()->_newAccessoryId = _itemData->getInventoryItem()->getMeta();
			Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::OomeeMakerEntryPointScene));
		}
	})));
	
	Super::onEnter();
}
void ShopItemPurchasedAnimation::onExit()
{
	Super::onExit();
}

void ShopItemPurchasedAnimation::resizeUI()
{
	const Size& contentSize = Director::getInstance()->getVisibleSize();
	this->setContentSize(contentSize);
	
	_beam->setContentSize(_beam->getTexture()->getContentSize());
	if(_beam->getContentSize().height > (contentSize.height * 0.65))
	{
		_beam->setContentSize(_beam->getContentSize() * ((contentSize.height * 0.65) / _beam->getContentSize().height));
	}
	_clippingNode->setContentSize(contentSize);
	_itemAsset->setContentSize(_beam->getContentSize());
	_itemAsset->resizeImage();
	_itemAsset->setPosition(Vec2(contentSize.width / 2, _beam->getContentSize().height / 2));
	_clippingStencil->setContentSize(_itemAsset->getContentSize() * 1.25);
	_clippingStencil->setPosition(_itemAsset->getPosition() + Vec2(0,_clippingStencil->getContentSize().height));
	_clippingStencil->stopAllActions();
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
