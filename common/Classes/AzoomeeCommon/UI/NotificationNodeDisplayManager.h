//
//  NotificationNodeDisplayManager.h
//  AzoomeeCommon
//
//  Created by Macauley on 18/03/2019.
//

#ifndef NotificationNodeDisplayManager_h
#define NotificationNodeDisplayManager_h

#include "../Azoomee.h"
#include "cocos/cocos2d.h"
#include "NotificationNode.h"

NS_AZOOMEE_BEGIN

class NotificationNodeDisplayManager
{
private:
	
	NotificationNode* _notificationNode = nullptr;
	cocos2d::Node* _rewardLayer = nullptr;
	cocos2d::Node* _messagingLayer = nullptr;
	
	NotificationNodeDisplayManager();
public:
	static NotificationNodeDisplayManager* getInstance();
	virtual ~NotificationNodeDisplayManager();
	
	void init();
	
	void onSizeChanged();
	
	void addRewardNode(cocos2d::Node* rewardNode);
	void addMessagingNode(cocos2d::Node* messagingNode);
	
	void clearRewardLayer();
	void clearMessagingLayer();
	
	
};

NS_AZOOMEE_END

#endif /* NotificationNodeDisplayManager_h */
