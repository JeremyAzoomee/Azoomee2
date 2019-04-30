//
//  TutorialMessagingNode.h
//  AzoomeeCommon
//
//  Created by Macauley on 29/04/2019.
//

#ifndef TutorialMessagingNode_h
#define TutorialMessagingNode_h

#include "../Azoomee.h"
#include "../Strings.h"
#include <cocos/cocos2d.h>
#include "TutorialGuide.h"
#include "TutorialSpeechBubble.h"
#include "../UI/ResizeNode.h"

NS_AZOOMEE_BEGIN

enum class MessageLocation {TOP_LEFT, TOP_RIGHT, BOTTOM_LEFT, BOTTOM_RIGHT};

class TutorialMessagingNode : public ResizeNode
{
	typedef ResizeNode Super;
private:
	
	TutorialGuide* _guide = nullptr;
	TutorialSpeechBubble* _bubble = nullptr;
	
public:
	static TutorialMessagingNode* create(const std::string& message, const MessageLocation& location = MessageLocation::TOP_LEFT);
	
	bool init() override;
	void onEnter() override;
	
	void onSizeChanged() override;
	
	void setMessage(const std::string& message);
	void setLocation(const MessageLocation& location);
	
	CREATE_FUNC(TutorialMessagingNode);
};

NS_AZOOMEE_END

#endif /* TutorialMessagingNode_h */
