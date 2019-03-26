//
//  CoinDisplay.h
//  azoomee2
//
//  Created by Macauley on 30/01/2019.
//

#ifndef CoinDisplay_h
#define CoinDisplay_h

#include <AzoomeeCommon/Azoomee.h>
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>

NS_AZOOMEE_BEGIN

class CoinDisplay : public cocos2d::ui::Layout
{
	typedef cocos2d::ui::Layout Super;
private:
	static float sCoinCount;
	static bool sAnimating;
	static float sIncPerSec;
	static int sTargetVal;
	
	cocos2d::Label* _coinsLabel = nullptr;
	cocos2d::LayerColor* _valueBG = nullptr;
	cocos2d::ClippingNode* _valueFrame = nullptr;
	cocos2d::Sprite* _coinSprite = nullptr;
	
	void createGlowAnim();
	
	bool _animate = false;
	
public:
	bool init() override;
	void onEnter() override;
	void onExit() override;
	void update(float deltaT) override;
	
	void setAnimate(bool animate);
	
	CREATE_FUNC(CoinDisplay);
};

NS_AZOOMEE_END

#endif /* CoinDisplay_h */
