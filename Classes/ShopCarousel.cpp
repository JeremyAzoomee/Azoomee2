//
//  ShopCarousel.cpp
//  azoomee2
//
//  Created by Macauley on 08/03/2019.
//

#include "ShopCarousel.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool ShopCarousel::init()
{
	if(!Super::init())
	{
		return false;
	}
	
	const Size& visibleSize = Director::getInstance()->getVisibleSize();
	this->setContentSize(visibleSize);
	
	_shopDisplay = ui::PageView::create();
	_shopDisplay->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_shopDisplay->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	_shopDisplay->setContentSize(Size(visibleSize.height * 1.2f, visibleSize.height * 0.6f));
	_shopDisplay->setBackGroundColor(Color3B::RED);
	_shopDisplay->setBackGroundColorType(BackGroundColorType::SOLID);
	_shopDisplay->setDirection(ui::PageView::Direction::HORIZONTAL);
	_shopDisplay->setTouchEnabled(false);
	_shopDisplay->setSwallowTouches(false);
	_shopDisplay->addEventListener([this](Ref* pSender, ui::PageView::EventType eType){
		if(eType == ui::PageView::EventType::TURNING)
		{
			for(auto child : _pageIndicator->getChildren())
			{
				Sprite* sprite = dynamic_cast<Sprite*>(child);
				if(sprite)
				{
					sprite->setTexture(sprite->getTag() == _shopDisplay->getCurrentPageIndex() ? "res/shop/dot_active.png" : "res/shop/dot_inactive.png" );
				}
			}
			_pageRight->runAction(FadeTo::create(0.5,_shopDisplay->getCurrentPageIndex() == (_shopDisplay->getItems().size() - 1) ? 125 : 255));
			_pageLeft->runAction(FadeTo::create(0.5,_shopDisplay->getCurrentPageIndex() == 0 ? 125 : 255));
		}
	});
	
	std::vector<Color3B> colours = {Color3B::BLUE, Color3B::GREEN, Color3B::YELLOW};
	for(int i = 0; i < colours.size(); i++)
	{
		ui::Layout* page = ui::Layout::create();
		page->setBackGroundColor(colours.at(i));
		page->setBackGroundColorType(BackGroundColorType::SOLID);
		page->setContentSize(_shopDisplay->getContentSize());
		page->setTouchEnabled(true);
		page->addTouchEventListener([this, colours, i, page](Ref* pSender, ui::Widget::TouchEventType eType){
			if(eType == TouchEventType::BEGAN)
			{
				page->setBackGroundColor(Color3B::RED);
			}
			else if(eType == TouchEventType::ENDED)
			{
				page->setBackGroundColor(colours.at(i));
			}
		});
		_shopDisplay->addPage(page);
	}
	
	this->addChild(_shopDisplay);
	
	_pageLeft = ui::Button::create("res/shop/arrow_button.png");
	_pageLeft->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_LEFT);
	_pageLeft->setAnchorPoint(Vec2(-0.5,0.5));
	_pageLeft->setOpacity(_shopDisplay->getCurrentPageIndex() == 0 ? 125 : 255);
	_pageLeft->addTouchEventListener([this](Ref* pSender, ui::Widget::TouchEventType eType){
		if(eType == TouchEventType::ENDED)
		{
			_shopDisplay->scrollToPage(MAX(0,_shopDisplay->getCurrentPageIndex() - 1),1);
		}
	});
	this->addChild(_pageLeft);
	
	_pageRight = ui::Button::create("res/shop/arrow_button.png");
	_pageRight->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_RIGHT);
	_pageRight->setAnchorPoint(Vec2(-0.5,0.5));
	_pageRight->setRotation(180.0f);
	_pageRight->setOpacity(_shopDisplay->getCurrentPageIndex() == (_shopDisplay->getItems().size() - 1) ? 125 : 255);
	_pageRight->addTouchEventListener([this](Ref* pSender, ui::Widget::TouchEventType eType){
		if(eType == TouchEventType::ENDED)
		{
			_shopDisplay->scrollToPage(MIN(_shopDisplay->getItems().size() - 1,_shopDisplay->getCurrentPageIndex() + 1),1);
		}
	});
	this->addChild(_pageRight);
	
	_pageIndicator = ui::Layout::create();
	_pageIndicator->setContentSize(Size(160 * _shopDisplay->getItems().size(), 160));
	_pageIndicator->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
	_pageIndicator->setNormalizedPosition(Vec2(0.5,0.2));
	this->addChild(_pageIndicator);
	
	for(int i = 0; i < colours.size(); i++)
	{
		Sprite* sprite = Sprite::create(i == _shopDisplay->getCurrentPageIndex() ? "res/shop/dot_active.png" : "res/shop/dot_inactive.png");
		sprite->setNormalizedPosition(Vec2((i+0.5)/(float)colours.size(),0.5));
		sprite->setTag(i);
		_pageIndicator->addChild(sprite);
	}
	
	return true;
}
void ShopCarousel::onEnter()
{
	Super::onEnter();
}
void ShopCarousel::onExit()
{
	Super::onExit();
}
void ShopCarousel::update(float deltaT)
{
	Super::update(deltaT);
}

void ShopCarousel::setShopData(const ShopRef& shopData)
{
	_shop = shopData;
}

NS_AZOOMEE_END
