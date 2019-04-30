//
//  TutorialGuide.h
//  AzoomeeCommon
//
//  Created by Macauley on 29/04/2019.
//

#ifndef TutorialGuide_h
#define TutorialGuide_h

#include "../Azoomee.h"
#include <cocos/cocos2d.h>

NS_AZOOMEE_BEGIN

class TutorialGuide : public cocos2d::Node
{
	typedef cocos2d::Node Super;
private:
	cocos2d::Sprite* _avatar = nullptr;
	cocos2d::Sprite* _frame = nullptr;
	cocos2d::ClippingNode* _clippingNode = nullptr;
	cocos2d::DrawNode* _stencil = nullptr;
	
public:
	bool init() override;
	void onEnter() override;
	
	CREATE_FUNC(TutorialGuide);
};

NS_AZOOMEE_END

#endif /* TutorialGuide_h */
