//
//  TutorialSpeechBubble.cpp
//  AzoomeeCommon
//
//  Created by Macauley on 30/04/2019.
//

#include "TutorialSpeechBubble.h"
#include <cocos/ui/CocosGUI.h>
#include "../UI/Style.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool TutorialSpeechBubble::init()
{
	if(!Super::init())
	{
		return false;
	}
	
	_bubble = ui::Scale9Sprite::create("res/tutorial/speech_bubble.png");
	
	const Size& contentSize = _bubble->getContentSize();
	this->setContentSize(contentSize);
	
	_bubble->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	_bubble->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	this->addChild(_bubble);
	
	_text = Label::createWithTTF("", Style::Font::Regular(), 65);
	_text->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	_text->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_text->setOverflow(Label::Overflow::SHRINK);
	_text->setDimensions(contentSize.width * 0.8f, contentSize.height * 0.8f);
	_text->setHorizontalAlignment(TextHAlignment::CENTER);
	_text->setVerticalAlignment(TextVAlignment::CENTER);
	_text->setTextColor(Color4B::BLACK);
	_bubble->addChild(_text);
	
	return true;
}
void TutorialSpeechBubble::onEnter()
{
	Super::onEnter();
}

void TutorialSpeechBubble::setText(const std::string& text)
{
	_text->setString(text);
}

NS_AZOOMEE_END
