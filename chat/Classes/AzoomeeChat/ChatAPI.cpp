#include "ChatAPI.h"
#include <AzoomeeCommon/UI/ModalMessages.h>
#include <AzoomeeCommon/Data/Child/ChildDataProvider.h>
#include <AzoomeeCommon/Data/Json.h>
#include <cocos/cocos2d.h>
#include <memory>

using namespace cocos2d;

NS_AZOOMEE_CHAT_BEGIN

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
    HttpRequestCreator* request = API::GetChatListRequest(childData->getLoggedInChildId(), this);
    request->execute();
}

FriendList ChatAPI::getFriendList() const
{
    return _friendList;
}

#pragma mark - Get Messages

void ChatAPI::requestMessageHistory(const FriendRef& friendObj)
{
    ChildDataProvider* childData = ChildDataProvider::getInstance();
    HttpRequestCreator* request = API::GetChatMessagesRequest(childData->getLoggedInChildId(), friendObj->friendId(), this);
    request->execute();
}

#pragma mark - Send Message

void ChatAPI::sendMessage(const FriendRef& friendObj, const MessageRef& message)
{
    ChildDataProvider* childData = ChildDataProvider::getInstance();
    const JsonObjectRepresentation& asJson = *message.get();
    HttpRequestCreator* request = API::SendChatMessageRequest(childData->getLoggedInChildId(), friendObj->friendId(), asJson, this);
    request->execute();
}

void ChatAPI::sendMessage(const FriendRef& friendObj, const std::string& message)
{
    // Create the message
    const MessageRef& messageObj = Message::createTextMessage(message);
    sendMessage(friendObj, messageObj);
}

#pragma mark - HttpRequestCreatorResponseDelegate

void ChatAPI::onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body)
{
    cocos2d::log("ChatAPI::onHttpRequestSuccess: %s, body=%s", requestTag.c_str(), body.c_str());
    
    // Get chat list success
    if(requestTag == API::TagGetChatList)
    {
        // Parse the response
        rapidjson::Document response;
        response.Parse(body.c_str());
        
        FriendList friendList;
        
        for(auto it = response.Begin(); it != response.End(); ++it)
        {
            const auto& jsonEntry = *it;
            FriendRef friendData = Friend::createFromJson(jsonEntry);
            friendList.push_back(friendData);
            
            int unreadMessages = jsonEntry["unreadMessages"].GetInt();
            cocos2d::log("%d unread messages from %s", unreadMessages, friendData->friendName().c_str());
        }
        
        _friendList = friendList;
        // Update the index
        _friendIndex.clear();
        for(auto friendData : _friendList)
        {
            _friendIndex[friendData->friendId()] = friendData;
        }
        
        // Notify observers
        for(auto observer : _observers)
        {
            observer->onChatAPIGetFriendList(_friendList);
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
            MessageRef message = Message::createFromJson(object);
            messages.push_back(message);
        }
        
        // Notify observers
        for(auto observer : _observers)
        {
            observer->onChatAPIGetChatMessages(messages);
        }
    }
}

void ChatAPI::onHttpRequestFailed(const std::string& requestTag, long errorCode)
{
    cocos2d::log("ChatAPI::onHttpRequestFailed: %s, errorCode=%ld", requestTag.c_str(), errorCode);
    ModalMessages::getInstance()->stopLoading();
}

NS_AZOOMEE_CHAT_END
