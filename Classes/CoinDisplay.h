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

NS_AZOOMEE_BEGIN

class CoinDisplay : public cocos2d::Node
{
	typedef cocos2d::Node Super;
private:
	static float sCoinCount;
	static bool sAnimating;
	static float sIncPerSec;
	static int sTargetVal;
	
	cocos2d::Label* _coinsLabel = nullptr;
	cocos2d::LayerGradient* _valueBG = nullptr;
	cocos2d::ClippingNode* _valueFrame = nullptr;
	cocos2d::Sprite* _coinSprite = nullptr;
	
public:
	bool init() override;
	void onEnter() override;
	void update(float deltaT) override;
	
	CREATE_FUNC(CoinDisplay);
};

NS_AZOOMEE_END

#endif /* CoinDisplay_h */
