//
//  ShopItemPage.h
//  azoomee2
//
//  Created by Macauley on 06/03/2019.
//

#ifndef ShopItemPage_h
#define ShopItemPage_h

// 4x2 grid of shop items, exists as page in Shop carousel. takes ShopDisplayRef
#include <AzoomeeCommon/Azoomee.h>
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include <AzoomeeCommon/Data/Shop/ShopDisplay.h>
#include "ShopItem.h"

NS_AZOOMEE_BEGIN

class ShopItemPage : public cocos2d::ui::Layout
{
	typedef cocos2d::ui::Layout Super;
	typedef std::function<void(const ShopDisplayItemRef&)> ItemSelectedCallback;
private:
	ShopDisplayRef _displayData = nullptr;
	
	std::vector<ShopItem*> _itemTiles;
	
	ItemSelectedCallback _itemSelectedCallback = nullptr;
	
public:
	
	bool init() override;
	void onEnter() override;
	void onExit() override;
	
	void setDisplayData(const ShopDisplayRef& displayData);
	void setItemSelectedCallback(const ItemSelectedCallback& callback);
	
	CREATE_FUNC(ShopItemPage);
};

NS_AZOOMEE_END

#endif /* ShopItemPage_h */
