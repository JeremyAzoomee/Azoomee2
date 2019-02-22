//
//  CoinChestLayer.h
//  azoomee2
//
//  Created by Macauley on 06/02/2019.
//

#ifndef CoinChestLayer_h
#define CoinChestLayer_h

#include <AzoomeeCommon/Azoomee.h>
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include "RewardScreen.h"

NS_AZOOMEE_BEGIN

class CoinChestLayer : public RewardScreen
{
	typedef RewardScreen Super;
private:
	
	cocos2d::LayerColor* _bgColour = nullptr;
	cocos2d::ui::Scale9Sprite* _wires = nullptr;
	cocos2d::ui::Layout* _text = nullptr;
	cocos2d::Node* _chestNode = nullptr;
	
	void addText();
	void addChest();
	
public:
	
	bool init() override;
	void onEnter() override;
	void update(float deltaT) override;
	void onSizeChanged() override;
	
	CREATE_FUNC(CoinChestLayer);
};

NS_AZOOMEE_END

#endif /* CoinChestLayer_h */
