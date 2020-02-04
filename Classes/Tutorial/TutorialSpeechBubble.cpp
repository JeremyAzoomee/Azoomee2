//
//  TutorialSpeechBubble.cpp
//  AzoomeeCommon
//
//  Created by Macauley on 30/04/2019.
//

#include "TutorialSpeechBubble.h"
#include <cocos/ui/CocosGUI.h>
#include <AzoomeeCommon/UI/Style.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool TutorialSpeechBubble::init()
{
	if(!Super::init())
	{
		return false;
	}
	
	_bubble = Sprite::create("res/tutorial/speech_bubble.png");
	const Size& contentSize = _bubble->getContentSize();
	
	_bubble->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	_bubble->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	this->addChild(_bubble);
	
	_leftEndCap = Sprite::create("res/tutorial/speech_bubble_edge.png");
	_leftEndCap->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
	_leftEndCap->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_LEFT);
	//_leftEndCap->setContentSize(Size(_leftEndCap->getContentSize().width, contentSize.height));
	_leftEndCap->setScale(contentSize.height / _leftEndCap->getContentSize().height);
	_bubble->addChild(_leftEndCap);
	
	_rightEndCap = Sprite::create("res/tutorial/speech_bubble_edge.png");
	_rightEndCap->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
	_rightEndCap->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_RIGHT);
	//_rightEndCap->setContentSize(Size(_rightEndCap->getContentSize().width, contentSize.height));
	_rightEndCap->setScale(contentSize.height / _rightEndCap->getContentSize().height);
	_rightEndCap->setRotation(180);
	_bubble->addChild(_rightEndCap);
	
	Super::setContentSize(contentSize + Size(2 * _leftEndCap->getContentSize().width,0));
	
	_bubblePoint = Sprite::create("res/tutorial/speech_bubble_point.png");
	_bubblePoint->setAnchorPoint(Vec2(0.65,0.5));
	_bubblePoint->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_LEFT);
	this->addChild(_bubblePoint, -1);
	
	_text = Label::createWithTTF("", Style::Font::Regular(), 60);
	_text->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
	_text->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_text->setOverflow(Label::Overflow::RESIZE_HEIGHT);
	_text->setMaxLineWidth(contentSize.width * 0.8f);
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
	_text->setTextColor(Color4B::BLACK);
	_bubble->setContentSize(Size(MIN(_text->getMaxLineWidth(),_text->getContentSize().width) + (2 * _leftEndCap->getContentSize().width),_text->getLineHeight() * (_text->getStringNumLines() + 1)));
	_leftEndCap->setScale(_bubble->getContentSize().height / _leftEndCap->getContentSize().height);
	_rightEndCap->setScale(_bubble->getContentSize().height / _rightEndCap->getContentSize().height);
	setContentSize(_bubble->getContentSize() + Size(2 * _leftEndCap->getContentSize().width * _leftEndCap->getScale(),0));
}

void TutorialSpeechBubble::setMaxWidth(float maxWidth)
{
	_text->setMaxLineWidth(maxWidth - (4 * _leftEndCap->getContentSize().width));
	_bubble->setContentSize(Size(MIN(_text->getMaxLineWidth(),_text->getContentSize().width) + (2 * _leftEndCap->getContentSize().width),_text->getLineHeight() * (_text->getStringNumLines() + 1)));
	_leftEndCap->setScale(_bubble->getContentSize().height / _leftEndCap->getContentSize().height);
	_rightEndCap->setScale(_bubble->getContentSize().height / _rightEndCap->getContentSize().height);
	setContentSize(_bubble->getContentSize() + Size(2 * _leftEndCap->getContentSize().width * _leftEndCap->getScale(),0));
}

void TutorialSpeechBubble::setBubbleOrigin(const BubbleOrigin& origin)
{
	switch(origin)
	{
		case BubbleOrigin::LEFT:
			_bubblePoint->setRotation(0);
			_bubblePoint->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_LEFT);
			break;
		case BubbleOrigin::RIGHT:
			_bubblePoint->setRotation(180);
			_bubblePoint->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_RIGHT);
			break;
		case BubbleOrigin::TOP:
			_bubblePoint->setRotation(90);
			_bubblePoint->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_TOP);
			break;
		case BubbleOrigin::BOTTOM:
			_bubblePoint->setRotation(-90);
			_bubblePoint->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_BOTTOM);
			break;
	}
}

void TutorialSpeechBubble::animateIn(float delay, const AnimationCompleteCallback& callback)
{
	this->stopAllActions();
	
	this->setScale(0);
	_text->setVisible(false);
	this->runAction(Sequence::create(DelayTime::create(delay), EaseBackOut::create(ScaleTo::create(1, 1)),CallFunc::create([this, callback](){
		_text->setVisible(true);
		animateInText(0,callback);
	}) , NULL));
}

void TutorialSpeechBubble::animateInText(float delay, const AnimationCompleteCallback& callback)
{
	for(int i = 0; i < _text->getStringLength(); i++)
	{
		auto letter = _text->getLetter(i);
		if(letter)
		{
			letter->stopAllActions();
			letter->setOpacity(0);
			letter->runAction(Sequence::createWithTwoActions(DelayTime::create(delay + (i * 0.05f)), FadeIn::create(0.5f)));
		}
	}
	if(callback)
	{
		callback();
	}
}

void TutorialSpeechBubble::animateOut(const AnimationCompleteCallback& callback)
{
	stopAllActions();
	runAction(Sequence::create(EaseBackIn::create(ScaleTo::create(1, 0)), CallFunc::create([callback](){
		if(callback)
		{
			callback();
		}
	}), NULL));
}

void TutorialSpeechBubble::highlightMessageString(const std::string& targetStr, const cocos2d::Color3B& highlightColour)
{
	const std::string& messageString = _text->getString();
	auto pos = messageString.find(targetStr);
	if( pos != std::string::npos )
	{
		const Color4B& baseColour = _text->getTextColor();
		_text->setTextColor(Color4B::WHITE);
		for(int i = 0; i < messageString.size(); i++)
		{
			auto letter = _text->getLetter(i);
			if(letter)
			{
				if(i >= pos && i < (pos + targetStr.size()))
				{
					letter->setColor(highlightColour);
				}
				else
				{
					letter->setColor(Color3B::BLACK);
				}
			}
		}
	}
}

NS_AZOOMEE_END
