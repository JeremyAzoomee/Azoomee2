//
//  ShopItemPage.cpp
//  azoomee2
//
//  Created by Macauley on 11/03/2019.
//

#include "ShopItemPage.h"
#include "DynamicNodeHandler.h"
#include <AzoomeeCommon/Audio/AudioMixer.h>
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

//	0 2 | 4 6	landscape
//	1 3 | 5 7
//	--------
//  4 6 |
//	5 7 |
//	portrait

bool ShopItemPage::init()
{
	if(!Super::init())
	{
		return false;
	}
	
	return true;
}
void ShopItemPage::onEnter()
{
	const Size& contentSize = this->getContentSize();
	int isPortrait = contentSize.width < contentSize.height;
	
	const float scalefactor = MIN(contentSize.width, contentSize.height) / 1060.0f;
	
	const int numItems = 8;
	bool grid[numItems] = {false,false,false,false,false,false,false,false};
	
	Vec2 anchorPoints[numItems][2] = {
		{Vec2(0.125,0.75), Vec2(0.25,0.875)},
		{Vec2(0.125,0.25), Vec2(0.25,0.625)},
		{Vec2(0.375,0.75), Vec2(0.75,0.875)},
		{Vec2(0.375,0.25), Vec2(0.75,0.625)},
		{Vec2(0.625,0.75), Vec2(0.25,0.375)},
		{Vec2(0.625,0.25), Vec2(0.25,0.125)},
		{Vec2(0.875,0.75), Vec2(0.75,0.375)},
		{Vec2(0.875,0.25), Vec2(0.75,0.125)}
	};
	
	if(_displayData)
	{
		const auto& items = _displayData->getDisplayItems();
		for(int i = 0; i < items.size(); i++)
		{
			float itemScale = scalefactor;
			const ShopDisplayItemRef& item = items.at(i);
			int inc = 1;
			if(item->getShape() == "TWO_ONE")
			{
				inc = 2;
				itemScale *= 1.025f;
			}
			int pos = 0;
			while(pos < numItems && grid[pos])//search grid for free space
			{
				pos += inc;
			}
			
			if(pos >= numItems)
			{
				continue;
			}
			
			Vec2 anchor;
			for(int x = pos; x < pos + inc; x++)
			{
				grid[x] = true;
				anchor += anchorPoints[x][isPortrait];
			}
			
			anchor = anchor / inc;
			
			ShopItem* shopItem = ShopItem::create();
			shopItem->setShopItemData(item);
			shopItem->setNormalizedPosition(anchor);
			shopItem->addTouchEventListener([=](Ref* pSender, ui::Widget::TouchEventType eType){
				if(eType == TouchEventType::BEGAN || eType == TouchEventType::MOVED)
				{
					shopItem->setScale(itemScale * 1.05f);
				}
				else
				{
					shopItem->setScale(itemScale);
				}
				if(eType == TouchEventType::ENDED)
				{
					
					if(shopItem->isLocked())
					{
						AnalyticsSingleton::getInstance()->shopLockedItemPressed(pos,item);
						DynamicNodeHandler::getInstance()->startIAPFlow();
						AudioMixer::getInstance()->playEffect("Unavailable_Shop_Item_Click.mp3");
					}
					else if(shopItem->isAffordable() && !shopItem->isOwned())
					{
						AnalyticsSingleton::getInstance()->shopItemPressed(pos, item);
						AudioMixer::getInstance()->playEffect("Available_Shop_Item_Click.wav");
						if(_itemSelectedCallback)
						{
							_itemSelectedCallback(item);
						}
					}
					else
					{
						AudioMixer::getInstance()->playEffect("Unavailable_Shop_Item_Click.mp3");
						if(!shopItem->isAffordable())
						{
							AnalyticsSingleton::getInstance()->shopUnaffordableItemPressed(pos, item);
						}
					}
				}
			});
			shopItem->setScale(itemScale);
			this->addChild(shopItem);
			_itemTiles.push_back(shopItem);
		}
	}
	
	Super::onEnter();
}
void ShopItemPage::onExit()
{
	Super::onExit();
}

void ShopItemPage::setDisplayData(const ShopDisplayRef& displayData)
{
	_displayData = displayData;
}

void ShopItemPage::setItemSelectedCallback(const ItemSelectedCallback &callback)
{
	_itemSelectedCallback = callback;
}

NS_AZOOMEE_END
