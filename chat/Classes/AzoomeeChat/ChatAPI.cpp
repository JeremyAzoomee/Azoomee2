#include "ChatAPI.h"
#include <AzoomeeCommon/UI/ModalMessages.h>
#include <AzoomeeCommon/Data/Child/ChildDataProvider.h>
#include <AzoomeeCommon/Data/Json.h>
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include <cocos/cocos2d.h>
#include <memory>

using namespace cocos2d;

NS_AZOOMEE_CHAT_BEGIN

#pragma mark - constants

const char* const kPollScheduleKey = "Azoomee::ChatAPI::requestFriendList";
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
    ChildDataProvider* childData = ChildDataProvider::getInstance();
    _profileNames[childData->getParentOrChildId()] = childData->getLoggedInChildName();
    
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
    ChildDataProvider* childData = ChildDataProvider::getInstance();
    HttpRequestCreator* request = API::GetChatListRequest(childData->getParentOrChildId(), this);
    request->execute();
}

FriendList ChatAPI::getFriendList() const
{
    return _friendList;
}

void ChatAPI::reportChat(const FriendRef &friendObj)
{
    HttpRequestCreator *request = API::SendChatReportRequest(ChildDataProvider::getInstance()->getParentOrChildId(), friendObj->friendId(), this);
    request->execute();
}

void ChatAPI::resetReportedChat(const FriendRef &friendObj)
{
    HttpRequestCreator *request = API::ResetReportedChatRequest(ChildDataProvider::getInstance()->getParentOrChildId(), friendObj->friendId(), this);
    request->execute();
}

#pragma mark - Get Messages

void ChatAPI::requestMessageHistory(const FriendRef& friendObj, int pageNumber)
{
    ChildDataProvider* childData = ChildDataProvider::getInstance();
    HttpRequestCreator* request = API::GetChatMessagesRequest(childData->getParentOrChildId(), friendObj->friendId(), pageNumber, this);
    request->execute();
}

#pragma mark - Send Message

void ChatAPI::sendMessage(const FriendRef& friendObj, const MessageRef& message)
{
    ChildDataProvider* childData = ChildDataProvider::getInstance();
    const JsonObjectRepresentation& asJson = *message.get();
    HttpRequestCreator* request = API::SendChatMessageRequest(childData->getParentOrChildId(), friendObj->friendId(), asJson, this);
    request->execute();
}

#pragma mark - Mark messages

void ChatAPI::markMessagesAsRead(const FriendRef& friendObj, const MessageRef& message)
{
    ChildDataProvider* childData = ChildDataProvider::getInstance();
    HttpRequestCreator* request = API::MarkReadMessageRequest(childData->getParentOrChildId(), friendObj->friendId(), message->timestamp(), this);
    request->execute();
}

void ChatAPI::getTimelineSummary()
{
    HttpRequestCreator* request = API::GetTimelineSummary(ChildDataProvider::getInstance()->getParentOrChildId(), this);
    request->execute();
}

#pragma mark - HttpRequestCreatorResponseDelegate

void ChatAPI::onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body)
{
    cocos2d::log("ChatAPI::onHttpRequestSuccess: %s, body=%s", requestTag.c_str(), body.c_str());
    
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
}

void ChatAPI::onHttpRequestFailed(const std::string& requestTag, long errorCode)
{
    cocos2d::log("ChatAPI::onHttpRequestFailed: %s, errorCode=%ld", requestTag.c_str(), errorCode);
    ModalMessages::getInstance()->stopLoading();
    
    // Pass 401 unauthorized errors on, so they can show a login screen or otherwise take
    // appropriate action.
    if(errorCode == 401)
    {
        if(Azoomee::Chat::delegate)
        {
            Azoomee::Chat::delegate->onChatAuthorizationError(requestTag, errorCode);
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
    const std::string& currentChildID = ChildDataProvider::getInstance()->getParentOrChildId();
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

NS_AZOOMEE_CHAT_END
