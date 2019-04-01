//
//  NotificationNode.cpp
//  AzoomeeCommon
//
//  Created by Macauley on 21/02/2019.
//

#include "NotificationNode.h"
#include "ResizeNode.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool NotificationNode::init()
{
	if(!Super::init())
	{
		return false;
	}

	this->setContentSize(Director::getInstance()->getVisibleSize());
	this->setScale(Director::getInstance()->getWinSize().width / Director::getInstance()->getVisibleSize().width, Director::getInstance()->getWinSize().height / Director::getInstance()->getVisibleSize().height);
	
	return true;
}

void NotificationNode::onSizeChanged()
{
	this->setContentSize(Director::getInstance()->getVisibleSize());
	this->setScale(Director::getInstance()->getWinSize().width / Director::getInstance()->getVisibleSize().width, Director::getInstance()->getWinSize().height / Director::getInstance()->getVisibleSize().height);
}


NS_AZOOMEE_END
