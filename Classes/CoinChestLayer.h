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
#include <AzoomeeCommon/UI/RoundedRectSprite.h>

NS_AZOOMEE_BEGIN

class CoinChestLayer : public RewardScreen
{
	typedef RewardScreen Super;
private:
	
	cocos2d::LayerColor* _bgColour = nullptr;
    RoundedRectSprite* _pattern = nullptr;
	cocos2d::ui::Layout* _text = nullptr;
	cocos2d::Node* _chestNode = nullptr;
	
	cocos2d::ui::Button* _backButton = nullptr;
	cocos2d::ui::Button* _useButton = nullptr;
	
	
	
	void addText();
	void addChest();
	
public:
	
	bool init() override;
	void onEnter() override;
	void onSizeChanged() override;
	
	CREATE_FUNC(CoinChestLayer);
};

NS_AZOOMEE_END

#endif /* CoinChestLayer_h */
