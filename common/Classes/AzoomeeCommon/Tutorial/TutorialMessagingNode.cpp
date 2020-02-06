//
//  TutorialMessagingNode.cpp
//  AzoomeeCommon
//
//  Created by Macauley on 30/04/2019.
//

#include "TutorialMessagingNode.h"
#include "../Device.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

TutorialMessagingNode* TutorialMessagingNode::create(const std::string& message, const MessageLocation& location)
{
	TutorialMessagingNode* node = TutorialMessagingNode::create();
	node->setMessage(message);
	node->setLocation(location);
	
	return node;
}

bool TutorialMessagingNode::init()
{
	if(!Super::init())
	{
		return false;
	}
	
	this->setContentSize(Director::getInstance()->getVisibleSize());
	
	_guide = TutorialGuide::create();
	this->addChild(_guide);
	_bubble = TutorialSpeechBubble::create();
	_guide->addChild(_bubble);
	
	_bubble->setMaxWidth((this->getContentSize().width - _guide->getContentSize().width) * 0.8f);
	
	return true;
}

void TutorialMessagingNode::onEnter()
{
	Super::onEnter();
}

void TutorialMessagingNode::onSizeChanged()
{
	this->setContentSize(Director::getInstance()->getVisibleSize());
	
	if(Device::getInstance()->isDevice18x9())
	{
		_bubble->setMaxWidth((this->getContentSize().width - _guide->getContentSize().width) * 1.1f);
	}
	else
	{
		_bubble->setMaxWidth((this->getContentSize().width - _guide->getContentSize().width) * 0.8f);
	}
}

void TutorialMessagingNode::setMessage(const std::string &message)
{
	_bubble->setText(message);
}

void TutorialMessagingNode::setLocation(const MessageLocation &location)
{
	bool is18x9 = Device::getInstance()->isDevice18x9();
	
	switch(location)
	{
		case MessageLocation::TOP_LEFT:
			_guide->setNormalizedPosition(Vec2::ANCHOR_TOP_LEFT);
			_guide->setAnchorPoint(is18x9 ? Vec2(0,1.25f) : Vec2::ANCHOR_TOP_LEFT);
			_bubble->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
			_bubble->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_RIGHT);
			_bubble->setBubbleOrigin(BubbleOrigin::LEFT);
			break;
		case MessageLocation::TOP_RIGHT:
			_guide->setNormalizedPosition(Vec2::ANCHOR_TOP_RIGHT);
			_guide->setAnchorPoint(is18x9 ? Vec2(0,1.25f) : Vec2::ANCHOR_TOP_RIGHT);
			_bubble->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
			_bubble->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_LEFT);
			_bubble->setBubbleOrigin(BubbleOrigin::RIGHT);
			break;
		case MessageLocation::BOTTOM_LEFT:
			_guide->setNormalizedPosition(Vec2::ANCHOR_BOTTOM_LEFT);
			_guide->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
			_bubble->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
			_bubble->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_RIGHT);
			_bubble->setBubbleOrigin(BubbleOrigin::LEFT);
			break;
		case MessageLocation::BOTTOM_RIGHT:
			_guide->setNormalizedPosition(Vec2::ANCHOR_BOTTOM_RIGHT);
			_guide->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
			_bubble->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
			_bubble->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_LEFT);
			_bubble->setBubbleOrigin(BubbleOrigin::RIGHT);
			break;
	}
	
}

void TutorialMessagingNode::animateInGuideAndMessage(const AnimationCompleteCallback& callback)
{
	_bubble->setVisible(false);
	_guide->animateIn(0,[this,callback](){
		_bubble->setVisible(true);
		_bubble->animateIn(0,callback);
	});
}
void TutorialMessagingNode::animateInMessage(const AnimationCompleteCallback& callback)
{
	_bubble->animateInText(0,callback);
}
void TutorialMessagingNode::animateOutGuideAndMessage(const AnimationCompleteCallback& callback)
{
	_guide->animateOut(callback);
	_bubble->animateOut();
}
void TutorialMessagingNode::animateOutMessage(const AnimationCompleteCallback& callback)
{
	_bubble->animateOut(callback);
}

void TutorialMessagingNode::highlightMessageString(const std::string& targetStr, const cocos2d::Color3B& highlightColour)
{
	_bubble->highlightMessageString(targetStr, highlightColour);
}

void TutorialMessagingNode::setAnimatingOut(bool animatingOut)
{
	_animatingOut = animatingOut;
}
bool TutorialMessagingNode::isAnimatingOut() const
{
	return _animatingOut;
}

NS_AZOOMEE_END
