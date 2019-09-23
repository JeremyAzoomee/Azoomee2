//
//  IAPScene.h
//  Azoomee
//
//  Created by Macauley on 09/07/2019.
//

#ifndef IAPScene_h
#define IAPScene_h

#include <AzoomeeCommon/Azoomee.h>
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include <AzoomeeCommon/UI/Scene.h>
#include "MarketingCarousel.h"
#include "ProductLayout.h"
#include "IAPFooter.h"

NS_AZOOMEE_BEGIN

class IAPScene : public Azoomee::Scene
{
	typedef Azoomee::Scene Super;
private:
	MarketingCarousel* _marketingCarousel = nullptr;
	ProductLayout* _productLayout = nullptr;
	IAPFooter* _footer = nullptr;
	cocos2d::ui::Button* _closeButton = nullptr;
	
	cocos2d::EventListenerCustom* _paymentSuccessListener = nullptr;
	cocos2d::EventListenerCustom* _paymentFailedListener = nullptr;
    cocos2d::EventListenerCustom* _billingDataUpdatedListener = nullptr;
	
public:
	
	bool init() override;
	void onEnter() override;
	void onExit() override;
	void onSizeChanged() override;
	
	CREATE_FUNC(IAPScene);
};


NS_AZOOMEE_END

#endif /* IAPScene_h */
