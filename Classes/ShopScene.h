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
#include "ShopCarousel.h"

NS_AZOOMEE_BEGIN

class ShopScene : public Azoomee::Scene
{
	typedef Azoomee::Scene Super;
private:
	ShopCarousel* _shopCarousel = nullptr;
	
public:
	
	virtual bool init() override;
	virtual void onEnter() override;
	virtual void onExit() override;
	virtual void onSizeChanged() override;
	
	CREATE_FUNC(ShopScene);
};

NS_AZOOMEE_END

#endif /* ShopScene_h */
