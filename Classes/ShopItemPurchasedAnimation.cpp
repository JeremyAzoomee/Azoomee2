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
	
	_backButton = ui::Button::create("res/shop/back_white_v_2.png");
	_backButton->setNormalizedPosition(Vec2::ANCHOR_TOP_LEFT);
	_backButton->setAnchorPoint(Vec2(-0.25,1.25));
	_backButton->setVisible(false);
	_backButton->addTouchEventListener([this](Ref* pSender, ui::Widget::TouchEventType eType){
		if(eType == ui::Widget::TouchEventType::ENDED)
		{
			if(_onCompleteCallback)
			{
				_onCompleteCallback();
			}
		}
	});
	this->addChild(_backButton,1);
	
	_useButton = ui::Button::create("res/shop/cta.png");
	_useButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
	_useButton->setNormalizedPosition(Vec2(0.5,0.8));
	_useButton->setScale9Enabled(true);
	_useButton->setVisible(false);
	_useButton->addTouchEventListener([this](Ref* pSender, ui::Widget::TouchEventType eType){
		if(eType == ui::Widget::TouchEventType::ENDED)
		{
			if(_itemData)
			{
				//set item meta id in oomee maker delegate
				OomeeMakerDelegate::getInstance()->_newAccessoryId = _itemData->getInventoryItem()->getMeta();
				Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::OomeeMakerEntryPointScene));
			}
		}
	});
	this->addChild(_useButton);
	
	Label* useLabel = Label::createWithTTF(_("USE NOW"), Style::Font::Bold(), 75);
	useLabel->setTextColor(Color4B(0, 245, 246, 255));
	useLabel->setHorizontalAlignment(TextHAlignment::CENTER);
	useLabel->setVerticalAlignment(TextVAlignment::CENTER);
	useLabel->setOverflow(Label::Overflow::SHRINK);
	useLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	useLabel->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	_useButton->addChild(useLabel);
	_useButton->setContentSize(Size(useLabel->getContentSize().width + 150, _useButton->getContentSize().height));
	useLabel->setDimensions(_useButton->getContentSize().width - 150, _useButton->getContentSize().height * 0.8f);
	
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
		AudioMixer::getInstance()->playEffect("Purchase_Animation_Scan.wav");
	}
	
	this->runAction(Sequence::createWithTwoActions(DelayTime::create(5.0f), CallFunc::create([this](){
		//if(_onCompleteCallback)
		//{
		//	_onCompleteCallback();
		//}
		_backButton->setVisible(true);
		_useButton->setVisible(true);
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
