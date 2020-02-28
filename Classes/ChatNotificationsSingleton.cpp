#include "ChatNotificationsSingleton.h"
#include <TinizineCommon/Utils/StringFunctions.h>
#include <TinizineCommon/API/HttpRequestCreator.h>
#include <TinizineCommon/Analytics/AnalyticsSingleton.h>
#include <TinizineCommon/Audio/AudioMixer.h>
#include "AzoomeeChat/ChatAPI.h"
#include <external/json/document.h>

using namespace cocos2d;

USING_NS_TZ

NS_AZ_BEGIN

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

ChatNotificationsSingleton::~ChatNotificationsSingleton()
{
    Chat::ChatAPI::getInstance()->removeObserver(this);
}

bool ChatNotificationsSingleton::init()
{
    Chat::ChatAPI::getInstance()->registerObserver(this);
    return true;
}

void ChatNotificationsSingleton::onChatAPIGetFriendList(const Chat::FriendList& friendList, int amountOfNewMessages)
{
    loggedInUserHasNotifications = (amountOfNewMessages != 0);
}

void ChatNotificationsSingleton::forceNotificationsUpdate()
{
    Chat::ChatAPI::getInstance()->requestFriendList();
    Chat::ChatAPI::getInstance()->scheduleFriendListPoll( Chat::ChatAPI::kScheduleRateLow );
}

void ChatNotificationsSingleton::stopNotificationsUpdate()
{
    Chat::ChatAPI::getInstance()->unscheduleFriendListPoll();
}

bool ChatNotificationsSingleton::userHasNotifications()
{
    return loggedInUserHasNotifications;
}

NS_AZ_END
