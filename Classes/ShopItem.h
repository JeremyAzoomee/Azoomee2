//
//  ShopItem.h
//  azoomee2
//
//  Created by Macauley on 06/03/2019.
//

#ifndef ShopItem_h
#define ShopItem_h

//shop tile visual object, takes ShopDisplayItemRef
#include <AzoomeeCommon/Azoomee.h>
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include <AzoomeeCommon/Data/Shop/ShopDisplayItem.h>

NS_AZOOMEE_BEGIN

class ShopItem : public cocos2d::ui::Layout
{
	typedef cocos2d::ui::Layout Super;
private:
	ShopDisplayItemRef _itemData = nullptr;
	
public:
	
	bool init() override;
	void onEnter() override;
	void onExit() override;
	
	CREATE_FUNC(ShopItem);
};

NS_AZOOMEE_END

#endif /* ShopItem_h */
