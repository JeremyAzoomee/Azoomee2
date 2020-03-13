//
//  ShopItem.h
//  azoomee2
//
//  Created by Macauley on 06/03/2019.
//

#ifndef ShopItem_h
#define ShopItem_h

//shop tile visual object, takes ShopDisplayItemRef
#include <TinizineCommon/Tinizine.h>
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include <TinizineCommon/Data/Shop/ShopDisplayItem.h>
#include <TinizineCommon/ImageDownloader/RemoteImageSprite.h>
#include "Azoomee.h"

USING_NS_TZ

NS_AZ_BEGIN

class ShopItem : public cocos2d::ui::Button
{
	typedef cocos2d::ui::Button Super;
private:
	ShopDisplayItemRef _itemData = nullptr;
	
	RemoteImageSprite* _assetImage = nullptr;
	cocos2d::Sprite* _bgFrame = nullptr;
	cocos2d::Sprite* _assetBg = nullptr;
	cocos2d::Sprite* _ownedTick = nullptr;
	cocos2d::Sprite* _coin = nullptr;
	cocos2d::ui::Text* _costValue = nullptr;
	cocos2d::Sprite* _lockedIcon = nullptr;
	cocos2d::Sprite* _newIcon = nullptr;
	
	cocos2d::Node* _featuredAnim = nullptr;
	
	bool _locked = false;
	bool _affordable = false;
	bool _owned = false;
	
	void enableNewIcon(bool enable);
	void enableLockedIcon(bool enable);
	void enableFeaturedAnimation(bool enable);
	void enableOwnedIcon(bool enable);
	void setAffordable(bool affordable);
	
	void addFeaturedAnim();
	
public:
	
	bool init() override;
	void onEnter() override;
	void onExit() override;
	
	void setShopItemData(const ShopDisplayItemRef& shopItemData);
	
	bool isLocked() const;
	bool isAffordable() const;
	bool isOwned() const;
	
	CREATE_FUNC(ShopItem);
};

NS_AZ_END

#endif /* ShopItem_h */
