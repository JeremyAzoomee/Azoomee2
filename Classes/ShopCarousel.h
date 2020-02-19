//
//  ShopCarousel.h
//  azoomee2
//
//  Created by Macauley on 06/03/2019.
//

#ifndef ShopCarousel_h
#define ShopCarousel_h

// horizontal page view with left and right scroll buttons and page indicator at bottom.  takes ShopRef
#include <AzoomeeCommon/Tinizine.h>
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include <AzoomeeCommon/Data/Shop/Shop.h>
#include "ShopItemPage.h"
#include "Azoomee.h"

USING_NS_TZ

NS_AZ_BEGIN

class ShopCarousel : public cocos2d::ui::Layout
{
	typedef cocos2d::ui::Layout Super;
	typedef std::function<void(const ShopDisplayItemRef&)> ItemSelectedCallback;
private:
	
	ShopRef _shop = nullptr;
	cocos2d::ui::PageView* _shopWindow = nullptr;
	cocos2d::ui::Button* _pageLeft = nullptr;
	cocos2d::ui::Button* _pageRight = nullptr;
	cocos2d::ui::Layout* _pageIndicator = nullptr;
	
	std::vector<ShopItemPage*> _shopPages;
	
	ItemSelectedCallback _itemSelectedCallback = nullptr;
	
public:
	
	bool init() override;
	
	void setShopData(const ShopRef& shopData);
	void setItemSelectedCallback(const ItemSelectedCallback& callback);
	
	void refreshUI();
	
	CREATE_FUNC(ShopCarousel);
	
};

NS_AZ_END

#endif /* ShopCarousel_h */
