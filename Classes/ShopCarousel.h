//
//  ShopCarousel.h
//  azoomee2
//
//  Created by Macauley on 06/03/2019.
//

#ifndef ShopCarousel_h
#define ShopCarousel_h

// horizontal page view with left and right scroll buttons and page indicator at bottom.  takes ShopRef
#include <AzoomeeCommon/Azoomee.h>
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include <AzoomeeCommon/Data/Shop/Shop.h>

NS_AZOOMEE_BEGIN

class ShopCarousel : public cocos2d::ui::Layout
{
	typedef cocos2d::ui::Layout Super;
private:
	
	ShopRef _shop = nullptr;
	cocos2d::ui::PageView* _shopDisplay = nullptr;
	cocos2d::ui::Button* _pageLeft = nullptr;
	cocos2d::ui::Button* _pageRight = nullptr;
	cocos2d::ui::Layout* _pageIndicator = nullptr;
	
public:
	
	bool init() override;
	void onEnter() override;
	void onExit() override;
	void update(float deltaT) override;
	
	void setShopData(const ShopRef& shopData);
	
	CREATE_FUNC(ShopCarousel);
	
};

NS_AZOOMEE_END

#endif /* ShopCarousel_h */
