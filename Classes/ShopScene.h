//
//  ShopScene.h
//  azoomee2
//
//  Created by Macauley on 06/03/2019.
//

#ifndef ShopScene_h
#define ShopScene_h

#include <AzoomeeCommon/Azoomee.h>
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include <AzoomeeCommon/UI/Scene.h>
#include <AzoomeeCommon/API/API.h>
#include "ShopCarousel.h"
#include "ShopItemPurchasePopup.h"
#include "CoinDisplay.h"

NS_AZOOMEE_BEGIN

class ShopScene : public Azoomee::Scene, HttpRequestCreatorResponseDelegate
{
	typedef Azoomee::Scene Super;
	
private:
	ShopCarousel* _shopCarousel = nullptr;
	ShopItemPurchasePopup* _purchasePopup = nullptr;
	CoinDisplay* _coinDisplay = nullptr;
	
	cocos2d::LayerColor* _bgColour = nullptr;
	cocos2d::Sprite* _wires = nullptr;
	cocos2d::Sprite* _gradient = nullptr;
	
public:
	
	virtual bool init() override;
	virtual void onEnter() override;
	virtual void onExit() override;
	virtual void onSizeChanged() override;
	
	CREATE_FUNC(ShopScene);
	
	void onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body) override;
	void onHttpRequestFailed(const std::string& requestTag, long errorCode) override;
};

NS_AZOOMEE_END

#endif /* ShopScene_h */
