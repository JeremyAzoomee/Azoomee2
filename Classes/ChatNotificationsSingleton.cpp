#include "ChatNotificationsSingleton.h"
#include <AzoomeeCommon/Utils/StringFunctions.h>
#include <AzoomeeCommon/Data/Child/ChildDataProvider.h>
#include <AzoomeeCommon/API/HttpRequestCreator.h>
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include <AzoomeeCommon/Audio/AudioMixer.h>
#include <AzoomeeChat/ChatAPI.h>
#include <external/json/document.h>
#include "NavigationLayer.h"

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
    Chat::ChatAPI::getInstance()->removeObserver(this);
}

bool ChatNotificationsSingleton::init(void)
{
    Chat::ChatAPI::getInstance()->registerObserver(this);
    return true;
}

void ChatNotificationsSingleton::onChatAPINewMessageNotificationReceived(int sumOfUnreadMessages)
{
    loggedInUserHasNotifications = (sumOfUnreadMessages != 0);
    notifyNavigationLayer();
}

void ChatNotificationsSingleton::onChatAPIMessageRecieved(const Chat::MessageRef &sentMessage)
{
    loggedInUserHasNotifications = true;
    AnalyticsSingleton::getInstance()->unreadMessagesNotificationReceived();
    AudioMixer::getInstance()->playEffect("message.mp3");
    removeBadgeFromNavigationLayer();
    notifyNavigationLayer();
}

void ChatNotificationsSingleton::forceNotificationsUpdate()
{
    Chat::ChatAPI::getInstance()->startFriendListManualPoll();
}

bool ChatNotificationsSingleton::userHasNotifications()
{
    return loggedInUserHasNotifications;
}

void ChatNotificationsSingleton::setNavigationLayer(cocos2d::Layer* navLayer)
{
    navigationLayer = navLayer;
}

cocos2d::Layer* ChatNotificationsSingleton::getNavigationLayer()
{
    return navigationLayer;
}

void ChatNotificationsSingleton::notifyNavigationLayer()
{
    if(!navigationLayer) return;
    
    if(loggedInUserHasNotifications) ((NavigationLayer *)navigationLayer)->showNotificationBadge();
    else ((NavigationLayer *)navigationLayer)->hideNotificationBadge();
}

void ChatNotificationsSingleton::removeBadgeFromNavigationLayer()
{
    if(!navigationLayer) return;
    ((NavigationLayer *)navigationLayer)->hideNotificationBadge();
}

NS_AZOOMEE_END
