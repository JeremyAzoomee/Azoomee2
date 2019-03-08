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

class ShopItemPage : cocos2d::ui::Layout
{
	typedef cocos2d::ui::Layout Super;
private:
	ShopDisplayRef _displayData = nullptr;
	
	std::vector<ShopItem> _itemTiles;
	
	
	
public:
	
	bool init() override;
	void onEnter() override;
	void onExit() override;
	
	CREATE_FUNC(ShopItemPage);
};

NS_AZOOMEE_END

#endif /* ShopItemPage_h */
