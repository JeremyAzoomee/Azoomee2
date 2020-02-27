//
//  ShopItemPurchasePopup.h
//  azoomee2
//
//  Created by Macauley on 06/03/2019.
//

#ifndef ShopItemPurchasePopup_h
#define ShopItemPurchasePopup_h

#include <TinizineCommon/Tinizine.h>
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include <TinizineCommon/Data/Shop/ShopDisplayItem.h>
#include <TinizineCommon/ImageDownloader/RemoteImageSprite.h>
#include "Azoomee.h"

USING_NS_TZ

NS_AZ_BEGIN

class ShopItemPurchasePopup : public cocos2d::ui::Layout
{
	typedef cocos2d::ui::Layout Super;
	typedef std::function<void(const ShopDisplayItemRef&, bool)> PurchaseCallback;
private:
	ShopDisplayItemRef _itemData = nullptr;
	
	PurchaseCallback _purchaseCallback = nullptr;
	
	RemoteImageSprite* _itemAsset = nullptr;
	cocos2d::ui::Button* _buyButton = nullptr;
	cocos2d::ui::Button* _closeButton = nullptr;
	
	cocos2d::ui::Layout* _valueLayout = nullptr;
	cocos2d::Sprite* _coin = nullptr;
	cocos2d::Label* _cost = nullptr;
	
public:
	
	bool init() override;
	void onEnter() override;
	void onExit() override;
	
	void setItemData(const ShopDisplayItemRef& displayData);
	void setPurchaseCallback(const PurchaseCallback& callback);
	
	void refreshUI();
	
	ShopDisplayItemRef getItemData() const;
	
	CREATE_FUNC(ShopItemPurchasePopup);
};

NS_AZ_END

#endif /* ShopItemPurchasePopup_h */
