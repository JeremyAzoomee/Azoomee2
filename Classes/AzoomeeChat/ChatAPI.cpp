#include "ChatAPI.h"
#include "../ModalMessages.h"
#include <AzoomeeCommon/Data/Child/ChildManager.h>
#include <AzoomeeCommon/Data/Parent/UserAccountManager.h>
#include <AzoomeeCommon/Data/Json.h>
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include <AzoomeeCommon/Data/Cookie/CookieManager.h>
#include "../ErrorCodes.h"
#include <cocos/cocos2d.h>
#include <memory>

using namespace cocos2d;

USING_NS_TZ

NS_AZ_CHAT_BEGIN

#pragma mark - constants

const char* const kPollScheduleKey = "AZ::ChatAPI::requestFriendList";
const char* const kChatStatusActive = "ACTIVE";
const char* const kChatStatusInModeration = "IN_MODERATION";
const float ChatAPI::kScheduleRateLow = 30.0f;
const float ChatAPI::kScheduleRateHigh = 10.0f;

#pragma mark - Init

static std::auto_ptr<ChatAPI> sChatAPISharedInstance;

ChatAPI* ChatAPI::getInstance()
{
    if(!sChatAPISharedInstance.get())
    {
        sChatAPISharedInstance.reset(new ChatAPI());
    }
    return sChatAPISharedInstance.get();
}

ChatAPI::ChatAPI()
{
}

ChatAPI::~ChatAPI()
{
    unscheduleFriendListPoll();
}

#pragma mark - Schedule Polling of Friendlist

void ChatAPI::scheduleFriendListPoll(float interval)
{
    unscheduleFriendListPoll();
    Director::getInstance()->getScheduler()->schedule([&](float dt){
        this->requestFriendList();
    }, this, interval, false, kPollScheduleKey);
}

void ChatAPI::unscheduleFriendListPoll()
{
    Director::getInstance()->getScheduler()->unschedule(kPollScheduleKey, this);
}

bool ChatAPI::isFriendListPollScheduled()
{
    return Director::getInstance()->getScheduler()->isScheduled(kPollScheduleKey, this);
}

#pragma mark - Profile names

void ChatAPI::updateProfileNames()
{
    _profileNames.clear();
    
    // Add the current child user
    if(ChildManager::getInstance()->isChildLoggedIn())
    {
        _profileNames[ChildManager::getInstance()->getLoggedInChild()->getId()] = ChildManager::getInstance()->getLoggedInChild()->getProfileName();
    }
    else
    {
        _profileNames[UserAccountManager::getInstance()->getLoggedInParentId()] = UserAccountManager::getInstance()->getParentDisplayName();
    }
    
    // Add names from friend list
    for(auto friendData : _friendList)
    {
        _profileNames[friendData->friendId()] = friendData->friendName();
    }
}

std::string ChatAPI::getProfileName(const std::string& profileId)
{
    auto it = _profileNames.find(profileId);
    if(it != _profileNames.end())
    {
        return it->second;
    }
    
    // Not found
    return "";
}

#pragma mark - Observers

void ChatAPI::registerObserver(ChatAPIObserver* observer)
{
    auto it = std::find(_observers.begin(), _observers.end(), observer);
    if(it == _observers.end())
    {
        _observers.push_back(observer);
    }
}

void ChatAPI::removeObserver(ChatAPIObserver* observer)
{
    auto it = std::find(_observers.begin(), _observers.end(), observer);
    if(it != _observers.end())
    {
        _observers.erase(it);
    }
}

#pragma mark - FriendList

void ChatAPI::requestFriendList()
{
    std::string userId = UserAccountManager::getInstance()->getLoggedInParentId();
    bool isParent = true;
    if(ChildManager::getInstance()->isChildLoggedIn())
    {
        userId = ChildManager::getInstance()->getLoggedInChild()->getId();
        isParent = false;
    }
    HttpRequestCreator* request = API::GetChatListRequest(userId, isParent, this);
    request->execute();
}

FriendList ChatAPI::getFriendList() const
{
    return _friendList;
}

void ChatAPI::reportChat(const FriendRef &friendObj)
{
    std::string userId = UserAccountManager::getInstance()->getLoggedInParentId();
    if(ChildManager::getInstance()->isChildLoggedIn())
    {
        userId = ChildManager::getInstance()->getLoggedInChild()->getId();
    }
    HttpRequestCreator *request = API::SendChatReportRequest(userId, friendObj->friendId(), this);
    request->execute();
}

void ChatAPI::resetReportedChat(const FriendRef &friendObj)
{
    std::string userId = UserAccountManager::getInstance()->getLoggedInParentId();
    if(ChildManager::getInstance()->isChildLoggedIn())
    {
        userId = ChildManager::getInstance()->getLoggedInChild()->getId();
    }
    HttpRequestCreator *request = API::ResetReportedChatRequest(userId, friendObj->friendId(), this);
    request->execute();
}

#pragma mark - Get Messages

void ChatAPI::requestMessageHistory(const FriendRef& friendObj, int pageNumber)
{
    std::string userId = UserAccountManager::getInstance()->getLoggedInParentId();
    bool isParent = true;
    if(ChildManager::getInstance()->isChildLoggedIn())
    {
        userId = ChildManager::getInstance()->getLoggedInChild()->getId();
        isParent = false;
    }
    HttpRequestCreator* request = API::GetChatMessagesRequest(userId, friendObj->friendId(), pageNumber, isParent, this);
    request->execute();
}

#pragma mark - Send Message

void ChatAPI::sendMessage(const FriendRef& friendObj, const MessageRef& message)
{
    std::string userId = UserAccountManager::getInstance()->getLoggedInParentId();
    bool isParent = true;
    if(ChildManager::getInstance()->isChildLoggedIn())
    {
        userId = ChildManager::getInstance()->getLoggedInChild()->getId();
        isParent = false;
    }
    const JsonObjectRepresentation& asJson = *message.get();
    HttpRequestCreator* request = API::SendChatMessageRequest(userId, friendObj->friendId(), asJson, isParent, this);
    request->execute();
}

#pragma mark - Mark messages

void ChatAPI::markMessagesAsRead(const FriendRef& friendObj, const MessageRef& message)
{
    std::string userId = UserAccountManager::getInstance()->getLoggedInParentId();
    bool isParent = true;
    if(ChildManager::getInstance()->isChildLoggedIn())
    {
        userId = ChildManager::getInstance()->getLoggedInChild()->getId();
        isParent = false;
    }
    HttpRequestCreator* request = API::MarkReadMessageRequest(userId, friendObj->friendId(), message->timestamp(), isParent, this);
    request->execute();
}

void ChatAPI::getTimelineSummary()
{
    std::string userId = UserAccountManager::getInstance()->getLoggedInParentId();
    if(ChildManager::getInstance()->isChildLoggedIn())
    {
        userId = ChildManager::getInstance()->getLoggedInChild()->getId();
    }
    HttpRequestCreator* request = API::GetTimelineSummary(userId, this);
    request->execute();
}

#pragma mark - Session refresh

void ChatAPI::refreshChildSession()
{
	HttpRequestCreator* request = API::RefreshChildCookiesRequest(this);
	request->execute();
}

#pragma mark - HttpRequestCreatorResponseDelegate

void ChatAPI::onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body)
{
//    cocos2d::log("ChatAPI::onHttpRequestSuccess: %s, body=%s", requestTag.c_str(), body.c_str());
    
    // Get chat list success
    if(requestTag == API::TagGetChatList)
    {
        int sumOfUnreadMessages = 0;
        
        // Parse the response
        rapidjson::Document response;
        response.Parse(body.c_str());
        
        FriendList friendList;
        
        for(auto it = response.Begin(); it != response.End(); ++it)
        {
            const auto& jsonEntry = *it;
            const FriendRef& friendData = Friend::createFromJson(jsonEntry);
            if(friendData)
            {
                friendList.push_back(friendData);
                
                int unreadMessages = jsonEntry["unreadMessages"].GetInt();
                sumOfUnreadMessages += unreadMessages;
            }
        }
        
        _friendList = friendList;
        
        // Update indexes
        updateProfileNames();
        _friendIndex.clear();
        for(auto friendData : _friendList)
        {
            _friendIndex[friendData->friendId()] = friendData;
        }
        
        // Notify observers
        for(auto observer : _observers)
        {
            observer->onChatAPIGetFriendList(_friendList, sumOfUnreadMessages);
        }
    }
    // Get chat messages success
    else if(requestTag == API::TagGetChatMessages)
    {
        // Parse the response
        rapidjson::Document response;
        response.Parse(body.c_str());
        
        // Grab the messages
        MessageList messages;
        
        for(auto it = response.Begin(); it != response.End(); ++it)
        {
            const auto& object = *it;
            const MessageRef& message = Message::createFromJson(object);
            if(message)
            {
                // Make sure a text type message has a message, aka ignore blank messages
                if(message->messageType() == Message::MessageTypeText && message->messageText().size() == 0)
                {
                    continue;
                }
                else
                {
                    messages.push_back(message);
                }
            }
        }
        
        // Notify observers
        for(auto observer : _observers)
        {
            observer->onChatAPIGetChatMessages(messages);
        }
    }
    // Send message success
    else if(requestTag == API::TagSendChatMessage)
    {
        // Parse the response
        rapidjson::Document response;
        response.Parse(body.c_str());
        
        const MessageRef& message = Message::createFromJson(response);
        if(message)
        {
            // Notify observers
            for(auto observer : _observers)
            {
                observer->onChatAPISendMessage(message);
            }
        }
    }
    else if(requestTag == API::TagReportChat)
    {
        onModerationStatusResponseSuccess(requestTag, headers, body);
        
        AnalyticsSingleton::getInstance()->chatReportedEvent();
        ModalMessages::getInstance()->stopLoading();
    }
    else if(requestTag == API::TagResetReportedChat)
    {
        onModerationStatusResponseSuccess(requestTag, headers, body);
        
        AnalyticsSingleton::getInstance()->chatResetReportedEvent();
        ModalMessages::getInstance()->stopLoading();
    }
    else if(requestTag == API::TagGetTimelineSummary)
    {
        rapidjson::Document response;
        response.Parse(body.c_str());
        
        if(!response.HasParseError())
        {
            // Grab the messages
            MessageList messages;
            
            for(auto it = response.Begin(); it != response.End(); ++it)
            {
                const auto& object = *it;
                const MessageRef& message = Message::createFromJson(object);
                if(message)
                {
                    // Make sure a text type message has a message, aka ignore blank messages
                    if(message->messageType() == Message::MessageTypeText && message->messageText().size() == 0)
                    {
                        continue;
                    }
                    else
                    {
                        messages.push_back(message);
                    }
                }
            }
            
            // Notify observers
            for(auto observer : _observers)
            {
                observer->onChatAPIGetTimelineSummary(messages);
            }
        }
    }
	else if(requestTag == API::TagChildCookieRefresh)
	{
		ChildManager::getInstance()->parseChildSessionUpdate(body);
		HttpRequestCreator* request = API::GetSessionCookiesRequest(ChildManager::getInstance()->getLoggedInChild()->getId(), ChildManager::getInstance()->getLoggedInChild()->getCDNSessionId(), false, this);
		request->execute();
	}
	else if(requestTag == API::TagGetSessionCookies)
	{
		CookieManager::getInstance()->parseDownloadCookies(headers);
		// Notify observers
		for(auto observer : _observers)
		{
			observer->onChatAPIRefreshChildSession();
		}
	}
}

void ChatAPI::onHttpRequestFailed(const std::string& requestTag, long errorCode)
{
    cocos2d::log("ChatAPI::onHttpRequestFailed: %s, errorCode=%ld", requestTag.c_str(), errorCode);
    ModalMessages::getInstance()->stopLoading();
    
    // Pass 401 unauthorized errors on, so they can show a login screen or otherwise take
    // appropriate action.
    if(errorCode == 401)
    {
        if(AZ::Chat::delegate)
        {
            AZ::Chat::delegate->onChatAuthorizationError(requestTag, errorCode);
        }
    }
	else if(errorCode == ERROR_CODE_OFFLINE)
	{
		if(AZ::Chat::delegate)
		{
			AZ::Chat::delegate->onChatOfflineError(requestTag);
		}
	}
    else
    {
        // Otherwise pass all other errors to the observers
        for(auto observer : _observers)
        {
            observer->onChatAPIErrorRecieved(requestTag, errorCode);
        }
    }
}

void ChatAPI::onModerationStatusResponseSuccess(const std::string& requestTag, const std::string& headers, const std::string& body)
{
    // body={"id":1152,"userIdA":"8f3caca6-00e4-4ab1-9121-649d53b595c8","userIdB":"70d68eed-63cf-4538-9913-b3fad88b3118","status":"IN_MODERATION"}
    // Parse the response
    rapidjson::Document response;
    response.Parse(body.c_str());
    if(response.HasParseError())
    {
        return;
    }
    
    const std::string& userIdA = getStringFromJson("userIdA", response);
    const std::string& userIdB = getStringFromJson("userIdB", response);
    const std::string& status = getStringFromJson("status", response);
    
    // Make sure the confirmation is for the current user
    std::string currentChildID = UserAccountManager::getInstance()->getLoggedInParentId();
    if(ChildManager::getInstance()->isChildLoggedIn())
    {
        currentChildID = ChildManager::getInstance()->getLoggedInChild()->getId();
    }
    if(userIdA != currentChildID)
    {
        // Not related to the current user
        return;
    }
    
    // Find the friend object
    auto it = _friendIndex.find(userIdB);
    if(it == _friendIndex.end())
    {
        // No friend found
        return;
    }
    
    const FriendRef& friendObj = it->second;
    
    // Update the status for this object
    if(status == kChatStatusInModeration)
    {
        friendObj->markFriendInModeration(true);
    }
    else if(status == kChatStatusActive)
    {
        friendObj->markFriendInModeration(false);
    }
    
    // Notify observers
    for(auto observer : _observers)
    {
        observer->onChatAPIModerationStatusChanged(friendObj);
    }
}

NS_AZ_CHAT_END
