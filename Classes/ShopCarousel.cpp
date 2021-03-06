//
//  ShopCarousel.cpp
//  azoomee2
//
//  Created by Macauley on 08/03/2019.
//

#include "ShopCarousel.h"
#include <AzoomeeCommon/Audio/AudioMixer.h>
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool ShopCarousel::init()
{
	if(!Super::init())
	{
		return false;
	}
	
	const Size& visibleSize = Director::getInstance()->getVisibleSize();
	bool isPortrait = visibleSize.width < visibleSize.height;
	this->setContentSize(visibleSize);
	
	float minDimSize = MIN(visibleSize.height, visibleSize.width);
	
	_shopWindow = ui::PageView::create();
	_shopWindow->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_shopWindow->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	_shopWindow->setContentSize(Size(minDimSize * (isPortrait ? 0.6f : 1.2f), minDimSize * (isPortrait ? 1.2f : 0.6f)));
	_shopWindow->setDirection(ui::PageView::Direction::HORIZONTAL);
	_shopWindow->setTouchEnabled(false);
	_shopWindow->setSwallowTouches(false);
	_shopWindow->addEventListener([this](Ref* pSender, ui::PageView::EventType eType){
		if(eType == ui::PageView::EventType::TURNING)
		{
			for(auto child : _pageIndicator->getChildren())
			{
				Sprite* sprite = dynamic_cast<Sprite*>(child);
				if(sprite)
				{
					sprite->setTexture(sprite->getTag() == _shopWindow->getCurrentPageIndex() ? "res/shop/dot_active.png" : "res/shop/dot_inactive.png" );
				}
			}
			_pageRight->runAction(FadeTo::create(0.5,_shopWindow->getCurrentPageIndex() == (_shopWindow->getItems().size() - 1) ? 125 : 255));
			_pageLeft->runAction(FadeTo::create(0.5,_shopWindow->getCurrentPageIndex() == 0 ? 125 : 255));
			AnalyticsSingleton::getInstance()->shopPageTurned((int)_shopWindow->getCurrentPageIndex());
		}
	});
	
	this->addChild(_shopWindow);
	
	_pageLeft = ui::Button::create("res/shop/arrow_button.png");
	_pageLeft->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_LEFT);
	_pageLeft->setAnchorPoint(Vec2((isPortrait ? -0.125f : -0.25),0.5));
	_pageLeft->setOpacity(125);
	_pageLeft->addTouchEventListener([this](Ref* pSender, ui::Widget::TouchEventType eType){
		if(eType == TouchEventType::ENDED)
		{
			AudioMixer::getInstance()->playEffect("CategorySelection_Button.mp3");
			_shopWindow->scrollToPage(MAX(0,_shopWindow->getCurrentPageIndex() - 1),1);
		}
	});
	this->addChild(_pageLeft);
	
	_pageRight = ui::Button::create("res/shop/arrow_button.png");
	_pageRight->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_RIGHT);
	_pageRight->setAnchorPoint(Vec2((isPortrait ? -0.125f : -0.25),0.5));
	_pageRight->setRotation(180.0f);
	_pageRight->setOpacity(125);
	_pageRight->addTouchEventListener([this](Ref* pSender, ui::Widget::TouchEventType eType){
		if(eType == TouchEventType::ENDED)
		{
			AudioMixer::getInstance()->playEffect("CategorySelection_Button.mp3");
			_shopWindow->scrollToPage(MIN(_shopWindow->getItems().size() - 1,_shopWindow->getCurrentPageIndex() + 1),1);
		}
	});
	this->addChild(_pageRight);
	
	_pageIndicator = ui::Layout::create();
	_pageIndicator->setContentSize(Size(160 * _shopWindow->getItems().size(), 160));
	_pageIndicator->setAnchorPoint(Vec2(0.5,1.25));
	//_pageIndicator->setNormalizedPosition(Vec2(0.5,isPortrait? 0.15 : 0.2));
	this->addChild(_pageIndicator);
	
	float maxWidth = visibleSize.width - (_pageLeft->getContentSize().width * (isPortrait? 2.5f : 3.5f));
	float maxHeight = visibleSize.height  - (visibleSize.height * (isPortrait? 0.15 : 0.2)) - _pageLeft->getContentSize().height;
	
	float scale = MIN(maxWidth / _shopWindow->getContentSize().width, maxHeight / _shopWindow->getContentSize().height);
	_shopWindow->setScale(scale);
	_pageIndicator->setPosition(Vec2((visibleSize / 2) - Size(0,_shopWindow->getContentSize().height * scale) / 2));
	return true;
}

void ShopCarousel::setShopData(const ShopRef& shopData)
{
	_shop = shopData;
	refreshUI();
}

void ShopCarousel::setItemSelectedCallback(const ItemSelectedCallback& callback)
{
	_itemSelectedCallback = callback;
}

void ShopCarousel::refreshUI()
{
	int currentIndex = MAX(0,(int)_shopWindow->getCurrentPageIndex());
	
	const Size& visibleSize = Director::getInstance()->getVisibleSize();
	bool isPortrait = visibleSize.width < visibleSize.height;
	this->setContentSize(visibleSize);
	
	float minDimSize = MIN(visibleSize.height, visibleSize.width);
	
	_shopWindow->setContentSize(Size(minDimSize * (isPortrait ? 0.6f : 1.2f), minDimSize * (isPortrait ? 1.2f : 0.6f)));
	//_pageIndicator->setNormalizedPosition(Vec2(0.5,isPortrait? 0.15 : 0.2));
	_pageLeft->setAnchorPoint(Vec2((isPortrait ? -0.125f : -0.25),0.5));
	_pageRight->setAnchorPoint(Vec2((isPortrait ? -0.125f : -0.25),0.5));
	_shopWindow->removeAllPages();
	_pageIndicator->removeAllChildren();
	if(_shop)
	{
		_pageIndicator->setContentSize(Size(160 * _shop->getDisplays().size(), 160));
		for(int i = 0; i < _shop->getDisplays().size(); i++)
		{
			ShopDisplayRef display = _shop->getDisplays().at(i);
			
			ShopItemPage* page = ShopItemPage::create();
			page->setDisplayData(display);
			page->setItemSelectedCallback(_itemSelectedCallback);
			page->setContentSize(_shopWindow->getContentSize());
			_shopWindow->addPage(page);
			
			Sprite* sprite = Sprite::create(i == currentIndex ? "res/shop/dot_active.png" : "res/shop/dot_inactive.png");
			sprite->setNormalizedPosition(Vec2((i+0.5)/(float)_shop->getDisplays().size(),0.5));
			sprite->setTag(i);
			_pageIndicator->addChild(sprite);
		}
		
		_shopWindow->setCurrentPageIndex(currentIndex);
		
		_pageLeft->setOpacity(_shopWindow->getCurrentPageIndex() == 0 ? 125 : 255);
		_pageRight->setOpacity(_shopWindow->getCurrentPageIndex() == (_shopWindow->getItems().size() - 1) ? 125 : 255);
		
	}
	
	float maxWidth = visibleSize.width - (_pageLeft->getContentSize().width * (isPortrait? 2.5f : 3.5f));
	float maxHeight = visibleSize.height  - (visibleSize.height * (isPortrait? 0.15 : 0.2)) - _pageLeft->getContentSize().height;
	
	float scale = MIN(maxWidth / _shopWindow->getContentSize().width, maxHeight / _shopWindow->getContentSize().height);
	_shopWindow->setScale(scale);
	_pageIndicator->setPosition(Vec2((visibleSize / 2) - Size(0,_shopWindow->getContentSize().height * scale) / 2));
}

NS_AZOOMEE_END
