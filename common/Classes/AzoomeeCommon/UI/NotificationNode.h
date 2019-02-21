//
//  NotificationNode.h
//  AzoomeeCommon
//
//  Created by Macauley on 21/02/2019.
//

#ifndef NotificationNode_h
#define NotificationNode_h

#include "../Azoomee.h"
#include <cocos/cocos2d.h>

NS_AZOOMEE_BEGIN

// layers that extend NotificationNode will be used to diaplay visuals on the notifications node, set by Director::getInstance()->setNotificationNode(Node);
// notification nodes exists above all normal scene layers, so isnt impacted by scene transitions, allowing it to work with the async mechanics.
// Notification nodes are rendered outside of the normal pipeline, and thus dont have the normal matrix stack applied, this class scales the node to work like normal.
class NotificationNode : public cocos2d::Node
{
	typedef cocos2d::Node Super;
private:
	
	
public:
	virtual bool init() override;
	virtual void onSizeChanged();
	
	CREATE_FUNC(NotificationNode);
};

NS_AZOOMEE_END

#endif /* NotificationNode_h */
