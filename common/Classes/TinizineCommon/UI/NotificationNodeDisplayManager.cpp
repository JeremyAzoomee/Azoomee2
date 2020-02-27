//
//  NotificationNodeDisplayManager.cpp
//  TinizineCommon
//
//  Created by Macauley on 18/03/2019.
//

#include "NotificationNodeDisplayManager.h"
#include "ResizeNode.h"

using namespace cocos2d;

NS_TZ_BEGIN

static std::auto_ptr<NotificationNodeDisplayManager> sNotificationNodeDisplayManagerSharedInstance;

NotificationNodeDisplayManager* NotificationNodeDisplayManager::getInstance()
{
	if(!sNotificationNodeDisplayManagerSharedInstance.get())
	{
		sNotificationNodeDisplayManagerSharedInstance.reset(new NotificationNodeDisplayManager());
		sNotificationNodeDisplayManagerSharedInstance->init();
	}
	return sNotificationNodeDisplayManagerSharedInstance.get();
}
NotificationNodeDisplayManager::~NotificationNodeDisplayManager()
{
	//Director::getInstance()->setNotificationNode(nullptr); dont clear node as application will be closing when this is called.
}
NotificationNodeDisplayManager::NotificationNodeDisplayManager()
{
	
}

void NotificationNodeDisplayManager::init()
{
	_notificationNode = NotificationNode::create();
	Director::getInstance()->setNotificationNode(_notificationNode);
	
	_rewardLayer = Node::create();
	_rewardLayer->setContentSize(Director::getInstance()->getVisibleSize());
	_notificationNode->addChild(_rewardLayer);
	
	_messagingLayer = Node::create();
	_messagingLayer->setContentSize(Director::getInstance()->getVisibleSize());
	_notificationNode->addChild(_messagingLayer);
}

void NotificationNodeDisplayManager::onSizeChanged()
{
	_notificationNode->onSizeChanged();
	_rewardLayer->setContentSize(Director::getInstance()->getVisibleSize());
	_messagingLayer->setContentSize(Director::getInstance()->getVisibleSize());
	for(auto* child : _rewardLayer->getChildren())
	{
		ResizeNode* resizeNode = dynamic_cast<ResizeNode*>(child);
		if(resizeNode)
		{
			resizeNode->onSizeChanged();
		}
	}
	for(auto* child : _messagingLayer->getChildren())
	{
		ResizeNode* resizeNode = dynamic_cast<ResizeNode*>(child);
		if(resizeNode)
		{
			resizeNode->onSizeChanged();
		}
	}
}

void NotificationNodeDisplayManager::addRewardNode(Node* rewardNode)
{
	_rewardLayer->addChild(rewardNode);
}
void NotificationNodeDisplayManager::addMessagingNode(Node* messagingNode)
{
	_messagingLayer->addChild(messagingNode);
}

void NotificationNodeDisplayManager::clearRewardLayer()
{
	_rewardLayer->removeAllChildren();
}
void NotificationNodeDisplayManager::clearMessagingLayer()
{
	_messagingLayer->removeAllChildren();
}

NS_TZ_END
