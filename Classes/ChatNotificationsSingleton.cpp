#include "ChatNotificationsSingleton.h"
#include <AzoomeeCommon/Utils/StringFunctions.h>
#include <AzoomeeCommon/API/HttpRequestCreator.h>
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include <AzoomeeCommon/Audio/AudioMixer.h>
#include "AzoomeeChat/ChatAPI.h"
#include <external/json/document.h>

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

NS_AZOOMEE_END
