#include "ChatNotificationsSingleton.h"
#include <AzoomeeCommon/Utils/StringFunctions.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

static ChatNotificationsSingleton *_sharedChatNotificationsSingleton = NULL;

ChatNotificationsSingleton* ChatNotificationsSingleton::getInstance()
{
    if (! _sharedChatNotificationsSingleton)
    {
        _sharedChatNotificationsSingleton = new ChatNotificationsSingleton();
        _sharedChatNotificationsSingleton->init();
    }
    
    return _sharedChatNotificationsSingleton;
}

ChatNotificationsSingleton::~ChatNotificationsSingleton(void)
{
}

bool ChatNotificationsSingleton::init(void)
{
    return true;
}

void ChatNotificationsSingleton::getNotificationsForUser(std::string userid)
{
    
}

void ChatNotificationsSingleton::getNotificationsForAllUsers()
{
    
}

bool ChatNotificationsSingleton::userHasNotifications(std::string userid)
{
    return true;
}

NS_AZOOMEE_END
