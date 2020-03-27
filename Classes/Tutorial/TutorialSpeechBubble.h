//
//  TutorialSpeechBubble.h
//  TinizineCommon
//
//  Created by Macauley on 29/04/2019.
//

#ifndef TutorialSpeechBubble_h
#define TutorialSpeechBubble_h

#include <TinizineCommon/Tinizine.h>
#include <cocos/cocos2d.h>
#include "../Azoomee.h"

USING_NS_TZ

NS_AZ_BEGIN

enum class BubbleOrigin {LEFT, RIGHT, TOP, BOTTOM};

class TutorialSpeechBubble : public cocos2d::Node
{
	typedef cocos2d::Node Super;
	typedef std::function<void()> AnimationCompleteCallback;
private:
	cocos2d::Sprite* _bubble = nullptr;
	cocos2d::Sprite* _leftEndCap = nullptr;
	cocos2d::Sprite* _rightEndCap = nullptr;
	cocos2d::Sprite* _bubblePoint = nullptr;
	cocos2d::Label* _text = nullptr;
public:
	
	bool init() override;
	void onEnter() override;
	
	void setText(const std::string& text);
	
	void setMaxWidth(float width);
	
	void setBubbleOrigin(const BubbleOrigin& origin);
	
	void animateIn(float delay, const AnimationCompleteCallback& callback = nullptr);
	void animateInText(float delay, const AnimationCompleteCallback& callback = nullptr);
	void animateOut(const AnimationCompleteCallback& callback = nullptr);
	
	void highlightMessageString(const std::string& targetStr, const cocos2d::Color3B& highlightColour);
	
	CREATE_FUNC(TutorialSpeechBubble);
};

NS_AZ_END

#endif /* TutorialSpeechBubble_h */