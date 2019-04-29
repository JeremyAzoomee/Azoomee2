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
#include "TutorialSpeachBubble.h"
#include "../UI/ResizeNode.h"

NS_AZOOMEE_BEGIN

class TutorialMessagingNode : public ResizeNode
{
	typedef Super ResizeNode;
private:
	
	TutorialGuide* _guide = nullptr;
	
	
public:
	bool init() override;
	void onEnter() override;
	
	void onSizeChanged() override;
	
	CREATE_FUNC(TutorialMessagignNode);
}

NS_AZOOMEE_END

#endif /* TutorialMessagingNode_h */
