//
//  TutorialSpeechBubble.h
//  AzoomeeCommon
//
//  Created by Macauley on 29/04/2019.
//

#ifndef TutorialSpeechBubble_h
#define TutorialSpeechBubble_h

#include "../Azoomee.h"
#include <cocos/cocos2d.h>

NS_AZOOMEE_BEGIN

class TutorialSpeechBubble : public cocos2d::Node
{
	typedef Super cocos2d::Node;
private:
	cocos2d::ui::Scale9Sprite* _bubble = nullptr;
	coco2d::Label* _text = nullptr;
	
public:
	
	bool init() override;
	void onEnter() override;
	
	void setText(const std::string& text);
	
	CREATE_FUNC(TutorialSpeechBubble);
}

NS_AZOOMEE_END

#endif /* TutorialSpeechBubble_h */
