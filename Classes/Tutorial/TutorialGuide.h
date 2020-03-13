//
//  TutorialGuide.h
//  TinizineCommon
//
//  Created by Macauley on 29/04/2019.
//

#ifndef TutorialGuide_h
#define TutorialGuide_h

#include <TinizineCommon/Tinizine.h>
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include "../Azoomee.h"

USING_NS_TZ

NS_AZ_BEGIN

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

NS_AZ_END

#endif /* TutorialGuide_h */
