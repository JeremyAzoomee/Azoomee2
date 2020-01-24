//
//  TutorialMessagingNode.h
//  AzoomeeCommon
//
//  Created by Macauley on 29/04/2019.
//

#ifndef TutorialMessagingNode_h
#define TutorialMessagingNode_h

#include "../Azoomee.h"
#include "../Utils/LocaleManager.h"
#include <cocos/cocos2d.h>
#include "TutorialGuide.h"
#include "TutorialSpeechBubble.h"
#include "../UI/ResizeNode.h"

NS_AZOOMEE_BEGIN

enum class MessageLocation {TOP_LEFT, TOP_RIGHT, BOTTOM_LEFT, BOTTOM_RIGHT};

class TutorialMessagingNode : public ResizeNode
{
	typedef ResizeNode Super;
	typedef std::function<void()> AnimationCompleteCallback;
private:
	
	TutorialGuide* _guide = nullptr;
	TutorialSpeechBubble* _bubble = nullptr;
	
	bool _animatingOut = false;
	
public:
	static TutorialMessagingNode* create(const std::string& message, const MessageLocation& location = MessageLocation::TOP_LEFT);
	
	bool init() override;
	void onEnter() override;
	
	void onSizeChanged() override;
	
	void setMessage(const std::string& message);
	void setLocation(const MessageLocation& location);
	
	void animateInGuideAndMessage(const AnimationCompleteCallback& callback = nullptr);
	void animateInMessage(const AnimationCompleteCallback& callback = nullptr);
	void animateOutGuideAndMessage(const AnimationCompleteCallback& callback = nullptr);
	void animateOutMessage(const AnimationCompleteCallback& callback = nullptr);
	
	void highlightMessageString(const std::string& targetStr, const cocos2d::Color3B& highlightColour);
	
	void setAnimatingOut(bool animatingOut);
	bool isAnimatingOut() const;
	
	CREATE_FUNC(TutorialMessagingNode);
};

NS_AZOOMEE_END

#endif /* TutorialMessagingNode_h */
