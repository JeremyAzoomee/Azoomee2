#include "ChatNotificationsSingleton.h"
#include <AzoomeeCommon/Utils/StringFunctions.h>
#include <AzoomeeCommon/Data/Child/ChildDataProvider.h>
#include <AzoomeeCommon/API/HttpRequestCreator.h>
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include <AzoomeeCommon/Audio/AudioMixer.h>
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

void ChatNotificationsSingleton::getNotificationsForLoggedInUser()
{
    ChildDataProvider* childData = ChildDataProvider::getInstance();
    HttpRequestCreator* request = API::GetChatListRequest(childData->getParentOrChildId(), this);
    request->execute();
}

void ChatNotificationsSingleton::onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body)
{
    scheduleUpdateOfPollingUnreadMessages();
    
    rapidjson::Document contentData;
    contentData.Parse(body.c_str());
    
    if(contentData.HasParseError()) return;
    
    for(int i = 0; i < contentData.Size(); i++)
    {
        if(contentData[i].HasMember("unreadMessages"))
        {
            if(!contentData[i]["unreadMessages"].IsNull())
            {
                if(contentData[i]["unreadMessages"].GetInt() > 0)
                {
                    loggedInUserHasNotifications = true;
                    notifyNavigationLayer();
                    return;
                }
            }
        }
    }
    
    loggedInUserHasNotifications = false;
}

void ChatNotificationsSingleton::onHttpRequestFailed(const std::string& requestTag, long errorCode)
{
    cocos2d::log("NOTIFICATIONS ERROR: %ld", errorCode);
}

void ChatNotificationsSingleton::onChatAPIMessageRecieved(const Chat::MessageRef &sentMessage)
{
    loggedInUserHasNotifications = true;
    AnalyticsSingleton::getInstance()->unreadMessagesNotificationReceived();
    AudioMixer::getInstance()->playEffect("message.mp3");
    notifyNavigationLayer();
}

void ChatNotificationsSingleton::scheduleUpdateOfPollingUnreadMessages()
{
    Director::getInstance()->getScheduler()->unschedule("schedulerKey", this);
    
    Director::getInstance()->getScheduler()->schedule([&](float dt){
        this->getNotificationsForLoggedInUser();
    }, this, 30.0f, false, "schedulerKey");
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
    
    ((NavigationLayer *)navigationLayer)->showNotificationBadge();
}

NS_AZOOMEE_END
