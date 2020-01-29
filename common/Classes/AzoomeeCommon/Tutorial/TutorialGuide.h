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
#include <cocos/ui/CocosGUI.h>

NS_AZOOMEE_BEGIN

class TutorialGuide : public cocos2d::Node
{
	typedef cocos2d::Node Super;
	typedef std::function<void()> AnimationCompleteCallback;
private:
	cocos2d::ui::ImageView* _avatar = nullptr;
	cocos2d::Sprite* _frame = nullptr;
	cocos2d::ClippingNode* _clippingNode = nullptr;
	cocos2d::DrawNode* _stencil = nullptr;
	
public:
	bool init() override;
	void onEnter() override;
	
	void animateIn(float delay, const AnimationCompleteCallback& callback = nullptr);
	void animateOut(const AnimationCompleteCallback& callback = nullptr);
	
	CREATE_FUNC(TutorialGuide);
};

NS_AZOOMEE_END

#endif /* TutorialGuide_h */
