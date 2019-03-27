//
//  ShopItem.cpp
//  azoomee2
//
//  Created by Macauley on 11/03/2019.
//

#include "ShopItem.h"
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/UI/LayoutParams.h>
#include <AzoomeeCommon/Data/Child/ChildDataProvider.h>
#include <AzoomeeCommon/Data/Parent/ParentDataProvider.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool ShopItem::init()
{
	if(!Super::init())
	{
		return false;
	}
	
	loadTextureNormal("res/shop/square_no_glow.png");
	setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	this->setZoomScale(0);
	
	_bgFrame = Sprite::create("res/shop/square_glow.png");
	_bgFrame->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	_bgFrame->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	this->addChild(_bgFrame, -1);
	
	_assetBg = Sprite::create("res/shop/glow.png");
	_assetBg->setNormalizedPosition(Vec2(0.5,0.6));
	_assetBg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	this->addChild(_assetBg);
	
	_lockedIcon = Sprite::create("res/shop/padlock_2.png");
	_lockedIcon->setNormalizedPosition(Vec2::ANCHOR_TOP_RIGHT);
	_lockedIcon->setAnchorPoint(Vec2(1.35,1.25));
	this->addChild(_lockedIcon);
	
	_newIcon = Sprite::create("res/shop/new_star.png");
	_newIcon->setNormalizedPosition(Vec2::ANCHOR_TOP_LEFT);
	_newIcon->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	this->addChild(_newIcon);
	
	_coin = Sprite::create("res/shop/coin_small.png");
	_coin->setAnchorPoint(Vec2(1.5f,0.35f));
	_coin->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_LEFT);
	
	_costValue = ui::Text::create("", Style::Font::Bold(), 75);
	_costValue->setAnchorPoint(Vec2(0.5,-0.35f));
	_costValue->setNormalizedPosition(Vec2(0.6,0));
	_costValue->setTextVerticalAlignment(TextVAlignment::CENTER);
	_costValue->setTextHorizontalAlignment(TextHAlignment::CENTER);
	_costValue->addChild(_coin);
	this->addChild(_costValue);
	
	_ownedTick = Sprite::create("res/shop/tick.png");
	_ownedTick->setAnchorPoint(Vec2(0.5,-0.75));
	_ownedTick->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_BOTTOM);
	this->addChild(_ownedTick);
	
	_assetImage = RemoteImageSprite::create();
	_assetImage->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_assetImage->setNormalizedPosition(Vec2(0.5,0.6));
	_assetImage->setKeepAspectRatio(true);
	this->addChild(_assetImage);
	
	return true;
}
void ShopItem::onEnter()
{
	if(_itemData)
	{
		if(_itemData->getShape() == "TWO_ONE")
		{
			loadTextureNormal("res/shop/featured.png");
			_bgFrame->setTexture("res/shop/featured_glow.png");
			_assetBg->setTexture("res/shop/glow_big.png");
		}
		addFeaturedAnim();
		_assetImage->initWithUrlAndSizeWithoutPlaceholder(_itemData->getInventoryItem()->getUri(), Size(this->getContentSize().width * 0.8f, this->getContentSize().height - 160));
		_costValue->setString(StringUtils::format("%d",_itemData->getPrice()));
		const auto& tags = _itemData->getTags();
		enableNewIcon(std::find(tags.begin(), tags.end(), "NEW") != tags.end());
		enableFeaturedAnimation(std::find(tags.begin(), tags.end(), "FEATURED") != tags.end());
		const InventoryRef& inv = ChildDataProvider::getInstance()->getLoggedInChild()->getInventory();
		const auto& invItems = inv->getItems();
		enableOwnedIcon(std::find_if(invItems.begin(), invItems.end(), [this](const InventoryItemRef& item){
			return item->getItemId() == _itemData->getInventoryItem()->getItemId();
		}) != invItems.end());
		setAffordable(inv->getCoins() >= _itemData->getPrice());
		enableLockedIcon(!(ParentDataProvider::getInstance()->isPaidUser() || _itemData->getEntitlement() == "AZ_FREE"));
	}
	
	Super::onEnter();
}
void ShopItem::onExit()
{
	Super::onExit();
}

void ShopItem::setShopItemData(const ShopDisplayItemRef& shopItemData)
{
	_itemData = shopItemData;
}

void ShopItem::addFeaturedAnim()
{
	const Size& contentSize = this->getContentSize();
	
	_featuredAnim = Node::create();
	_featuredAnim->setContentSize(this->getContentSize());
	this->addChild(_featuredAnim);
	
	Sprite* sliderLeft = Sprite::create("res/shop/side_shooter.png");
	sliderLeft->setPosition(Vec2(5,40));
	sliderLeft->setOpacity(0);
	
	FiniteTimeAction* leftSliderAction = RepeatForever::create(Spawn::create(Sequence::create(MoveBy::create(1, Vec2(0,contentSize.height - 80)), FadeOut::create(0.5), MoveTo::create(0,Vec2(5,40)), DelayTime::create(4.5), NULL), FadeIn::create(0.5f) ,NULL));
	sliderLeft->runAction(leftSliderAction);
	_featuredAnim->addChild(sliderLeft);
	
	Sprite* sparkleLeft = Sprite::create("res/shop/star.png");
	sparkleLeft->setPosition(Vec2(5, contentSize.height - 40));
	sparkleLeft->setScale(0);
	sparkleLeft->runAction(RepeatForever::create(RotateBy::create(0.5, 180)));
	
	FiniteTimeAction* leftStarAction = RepeatForever::create(Sequence::create(DelayTime::create(1), ScaleTo::create(0.5, 1), ScaleTo::create(0.5, 0), DelayTime::create(4), NULL));
	sparkleLeft->runAction(leftStarAction);
	_featuredAnim->addChild(sparkleLeft);
	
	Sprite* sliderRight = Sprite::create("res/shop/side_shooter.png");
	sliderRight->setPosition(Vec2(contentSize.width - 5,contentSize.height - 40));
	sliderRight->setOpacity(0);
	
	FiniteTimeAction* rightSliderAction = RepeatForever::create(Spawn::create(Sequence::create(DelayTime::create(2),MoveBy::create(1, Vec2(0,-(contentSize.height - 80))), FadeOut::create(0.5), MoveTo::create(0,Vec2(contentSize.width - 5,contentSize.height - 40)), DelayTime::create(2.5), NULL),Sequence::create(DelayTime::create(2), FadeIn::create(0.5f),NULL) ,NULL));
	sliderRight->runAction(rightSliderAction);
	_featuredAnim->addChild(sliderRight);
	
	Sprite* sparkleRight = Sprite::create("res/shop/star.png");
	sparkleRight->setPosition(Vec2(contentSize.width - 5, 40));
	sparkleRight->setScale(0);
	sparkleRight->runAction(RepeatForever::create(RotateBy::create(0.5, 180)));
	
	FiniteTimeAction* rightStarAction = RepeatForever::create(Sequence::create(DelayTime::create(3), ScaleTo::create(0.5, 1), ScaleTo::create(0.5, 0), DelayTime::create(2), NULL));
	sparkleRight->runAction(rightStarAction);
	_featuredAnim->addChild(sparkleRight);
	
}

void ShopItem::enableNewIcon(bool enable)
{
	if(_newIcon)
	{
		_newIcon->setVisible(enable);
	}
}
void ShopItem::enableLockedIcon(bool enable)
{
	if(_lockedIcon)
	{
		_lockedIcon->setVisible(enable);
	}
	if(_costValue)
	{
		_costValue->setOpacity((enable || !_affordable) ? 127 : 255);
	}
	if(_assetImage)
	{
		_assetImage->setOpacity(enable ? 127 : 255);
	}
	_locked = enable;
}
void ShopItem::enableFeaturedAnimation(bool enable)
{
	if(_featuredAnim)
	{
		_featuredAnim->setVisible(enable);
	}
	if(_bgFrame)
	{
		_bgFrame->setVisible(enable);
	}
}
void ShopItem::enableOwnedIcon(bool enable)
{
	if(_ownedTick)
	{
		_ownedTick->setVisible(enable);
	}
	if(_costValue)
	{
		_costValue->setVisible(!enable);
	}
	_owned = enable;
}
void ShopItem::setAffordable(bool affordable)
{
	if(_costValue)
	{
		_costValue->setOpacity(affordable ? 255 : 127);
	}
	_affordable = affordable;
}

bool ShopItem::isLocked() const
{
	return _locked;
}
bool ShopItem::isAffordable() const
{
	return _affordable;
}
bool ShopItem::isOwned() const
{
	return _owned;
}

NS_AZOOMEE_END
