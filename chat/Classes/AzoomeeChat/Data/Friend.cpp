#include "ChatAPI.h"
#include <AzoomeeCommon/UI/ModalMessages.h>
#include <AzoomeeCommon/Data/Parent/ParentDataParser.h>
#include <AzoomeeCommon/Data/Parent/ParentDataProvider.h>
#include <AzoomeeCommon/Data/Child/ChildDataParser.h>
#include <AzoomeeCommon/Data/Child/ChildDataProvider.h>
#include <external/json/document.h>
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

#pragma mark - Login

void ChatAPI::loginUser(const std::string& username, const std::string& password)
{
    HttpRequestCreator* request = API::LoginRequest(username, password, this);
    request->execute();
}

#pragma mark - Get Messages

void ChatAPI::requestMessageHistory(const std::string& friendId)
{
    ChildDataProvider* childData = ChildDataProvider::getInstance();
    const std::string& loggedInChildId = childData->getLoggedInChildId();
    HttpRequestCreator* request = API::GetChatMessagesRequest(loggedInChildId, friendId, this);
    request->execute();
}

#pragma mark - HttpRequestCreatorResponseDelegate

void ChatAPI::onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body)
{
//    cocos2d::log("ChatAPI::onHttpRequestSuccess: %s, body=%s", requestTag.c_str(), body.c_str());
    ParentDataParser* parentDataParser = ParentDataParser::getInstance();
    ParentDataProvider* parentData = ParentDataProvider::getInstance();
    ChildDataParser* childDataParser = ChildDataParser::getInstance();
    ChildDataProvider* childData = ChildDataProvider::getInstance();
    
    // Parent login success
    if(requestTag == API::TagLogin)
    {
        if(parentDataParser->parseParentLoginData(body))
        {
            cocos2d::log("Logged in!");
            
            // Notify observers
            for( auto observer : _observers )
            {
                observer->onChatAPILogin();
            }
            
            // Request children accounts
            HttpRequestCreator* request = API::GetAvailableChildrenRequest(this);
            request->execute();
        }
    }
    // Get children success
    else if(requestTag == API::TagGetAvailableChildren)
    {
        parentDataParser->parseAvailableChildren(body);
        
        // Notify observers
        for( auto observer : _observers )
        {
            observer->onChatAPIGetAvailableChildren();
        }
        
        cocos2d::log("Child profiles:");
        for(int i = 0; i < parentData->getAmountOfAvailableChildren(); ++i)
        {
            const std::string& profileName = parentData->getProfileNameForAnAvailableChildren(i);
            cocos2d::log("profileName=%s", profileName.c_str());
        }
        
        // Login as child 0 automatically for now
        const std::string& profileName = parentData->getProfileNameForAnAvailableChildren(0);
        HttpRequestCreator* request = API::ChildLoginRequest(profileName, this);
        request->execute();
    }
    // Child login success
    else if(requestTag == API::TagChildLogin)
    {
        childDataParser->parseChildLoginData(body);
        cocos2d::log("Logged in as child: %s", childData->getLoggedInChildName().c_str());
        
        // Notify observers
        for( auto observer : _observers )
        {
            observer->onChatAPIChildLogin();
        }
        
        // Get chat list
        HttpRequestCreator* request = API::GetChatListRequest(childData->getLoggedInChildId(), this);
        request->execute();
    }
    // Get chat list success
    else if(requestTag == API::TagGetChatList)
    {
        // Parse the response
        rapidjson::Document response;
        response.Parse(body.c_str());
        // Example response:
        // [{
        //   "friendId":"9d2bb8a5-ab3c-41ce-bfca-bcf2c9e4a3cb",
        //   "friendName":"mitch",
        //   "avatar":null,
        //   "unreadMessages":0
        // },
        // {
        //   "friendId":"42cefe6d-89c5-49a5-a7c9-661c5f1ca72b",
        //   "friendName":"Yoshi",
        //   "avatar":"https://media.azoomee.com/static/thumbs/oomee_05.png",
        //   "unreadMessages":0
        // }]
        
        const std::string& loggedInChildId = childData->getLoggedInChildId();
        
        _contactList.clear();
        _contactList[loggedInChildId] = childData->getLoggedInChildName();
        
        for(auto it = response.Begin(); it != response.End(); ++it)
        {
            const auto& object = *it;
            const std::string& friendId = object["friendId"].GetString();
            const std::string& friendName = object["friendName"].GetString();
            _contactList[friendId] = friendName;
            int unreadMessages = object["unreadMessages"].GetInt();
            cocos2d::log("%d unread messages from %s", unreadMessages, friendName.c_str());
            
            if( unreadMessages > 0 )
            {
                // Get those unread messages
                HttpRequestCreator* request = API::GetChatMessagesRequest(loggedInChildId, friendId, this);
                request->execute();
            }
        }
        
        // Notify observers
        for( auto observer : _observers )
        {
            observer->onChatAPIGetChatList(_contactList);
        }
    }
    // Get chat messages success
    else if(requestTag == API::TagGetChatMessages)
    {
        // Parse the response
        rapidjson::Document response;
        response.Parse(body.c_str());
        // Example response:
        // [{"type":"TEXT","status":"ACTIVE","params":{"text":"Hello!"},"senderId":"9d2bb8a5-ab3c-41ce-bfca-bcf2c9e4a3cb","recipientId":"20bc86d3-4d15-4e6a-9021-77abeb9a8798","timestamp":1493395678099,"id":"21cb24ba-44b5-435e-b5f2-7d87a9c13ea3"}]
        
        // Just grab messages as a string for now
        std::vector<std::string> messages;
        
        for(auto it = response.Begin(); it != response.End(); ++it)
        {
            const auto& object = *it;
            const std::string& type = object["type"].GetString();
            const std::string& senderId = object["senderId"].GetString();
            const std::string& recipientId = object["recipientId"].GetString();
//            uint64_t timestamp = object["timestamp"].GetUint64();
            const std::string& recipientName = _contactList[recipientId];
            const std::string& senderName = _contactList[senderId];
            
            // Text type of message
            if(type == "TEXT")
            {
                const auto& params = object["params"];
                const std::string& text = params["text"].GetString();
                messages.push_back(StringUtils::format("%s >> %s: %s", senderName.c_str(), recipientName.c_str(), text.c_str()));
            }
            // Other type we don't support yet
            else
            {
                messages.push_back(StringUtils::format("%s >> %s: <unsupported message type: %s>", senderName.c_str(), recipientName.c_str(), type.c_str()));
            }
            
            cocos2d::log("%s", messages.back().c_str());
        }
        
        // Notify observers
        for( auto observer : _observers )
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
