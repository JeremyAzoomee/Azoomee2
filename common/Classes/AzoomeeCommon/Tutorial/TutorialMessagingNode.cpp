//
//  TutorialMessagingNode.cpp
//  AzoomeeCommon
//
//  Created by Macauley on 30/04/2019.
//

#include "TutorialMessagingNode.h"

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
	return true;
}

void TutorialMessagingNode::onEnter()
{
	Super::onEnter();
}

void TutorialMessagingNode::onSizeChanged()
{
	this->setContentSize(Director::getInstance()->getVisibleSize());
}

void TutorialMessagingNode::setMessage(const std::string &message)
{
	_bubble->setText(message);
}

void TutorialMessagingNode::setLocation(const MessageLocation &location)
{
	switch(location)
	{
		case MessageLocation::TOP_LEFT:
			_guide->setNormalizedPosition(Vec2::ANCHOR_TOP_LEFT);
			_guide->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
			_bubble->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
			_bubble->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_RIGHT);
			break;
		case MessageLocation::TOP_RIGHT:
			_guide->setNormalizedPosition(Vec2::ANCHOR_TOP_RIGHT);
			_guide->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
			_bubble->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
			_bubble->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_LEFT);
			break;
		case MessageLocation::BOTTOM_LEFT:
			_guide->setNormalizedPosition(Vec2::ANCHOR_BOTTOM_LEFT);
			_guide->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
			_bubble->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
			_bubble->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_RIGHT);
			break;
		case MessageLocation::BOTTOM_RIGHT:
			_guide->setNormalizedPosition(Vec2::ANCHOR_BOTTOM_RIGHT);
			_guide->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
			_bubble->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
			_bubble->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_LEFT);
			break;
	}
}

NS_AZOOMEE_END
