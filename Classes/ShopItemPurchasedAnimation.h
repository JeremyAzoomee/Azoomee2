//
//  ShopItemPurchasedAnimation.h
//  azoomee2
//
//  Created by Macauley on 06/03/2019.
//

#ifndef ShopItemPurchasedAnimation_h
#define ShopItemPurchasedAnimation_h

#include <AzoomeeCommon/Azoomee.h>
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include <AzoomeeCommon/Data/Shop/ShopDisplayItem.h>
#include <AzoomeeCommon/ImageDownloader/RemoteImageSprite.h>

NS_AZOOMEE_BEGIN

class ShopItemPurchasedAnimation : public cocos2d::ui::Layout
{
	typedef cocos2d::ui::Layout Super;
	typedef std::function<void()> OnCompleteCallback;
private:
	
	ShopDisplayItemRef _itemData = nullptr;
	OnCompleteCallback _onCompleteCallback = nullptr;
	
	cocos2d::Sprite* _beam = nullptr;
	RemoteImageSprite* _itemAsset = nullptr;
	cocos2d::ClippingNode* _clippingNode = nullptr;
	cocos2d::ui::Scale9Sprite* _clippingStencil = nullptr;
	
public:
	
	bool init() override;
	void onEnter() override;
	void onExit() override;
	void resizeUI();
	
	void setItemData(const ShopDisplayItemRef& itemData);
	void setOnCompleteCallback(const OnCompleteCallback& callback);
	
	CREATE_FUNC(ShopItemPurchasedAnimation);
	
};

NS_AZOOMEE_END

#endif /* ShopItemPurchasedAnimation_h */
