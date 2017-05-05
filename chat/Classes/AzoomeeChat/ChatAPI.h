#ifndef AzoomeeChat_ChatAPI_h
#define AzoomeeChat_ChatAPI_h

#include "AzoomeeChat.h"
#include "Data/Friend.h"
#include "Data/Message.h"
#include <AzoomeeCommon/API/API.h>
#include <map>


NS_AZOOMEE_CHAT_BEGIN

// forward decleration
class ChatAPIObserver;

/**
 * Manages communication of chat APIs with the Azoomee server.
 */
class ChatAPI : private HttpRequestCreatorResponseDelegate
{
private:
    
    /// Most recent friend list
    FriendList _friendList;
    /// Index Friends by Id
    std::map<std::string, FriendRef> _friendIndex;
    /// Observers monitoring this API and it's responses
    std::vector<ChatAPIObserver*> _observers;
    
    // - HttpRequestCreatorResponseDelegate
    void onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body) override;
    void onHttpRequestFailed(const std::string& requestTag, long errorCode) override;
    
public:
    
    static ChatAPI* getInstance();
    
    /// Register an observer
    void registerObserver(ChatAPIObserver* observer);
    /// Remove a previously registered observer
    void removeObserver(ChatAPIObserver* observer);
    
    /// Login the user.
    /// TODO: Refactor this so the logic is common and not duplicated across here and BackEndCaller in azoomee2.
    void loginUser(const std::string& username, const std::string& password);
    
    /// Request the friend list from the server
    void requestFriendList();
    /// Get the latest friend list we have on the server
    FriendList getFriendList() const;
    
    /// Get the chat messages for a contact
    void requestMessageHistory(const FriendRef& friendObj);
    
    /// Send a message
    void sendMessage(const FriendRef& friendObj, const MessageRef& message);
    /// Send a message of type text
    void sendMessage(const FriendRef& friendObj, const std::string& message);
};

/**
 * Recieve events relating to the chat API.
 */
struct ChatAPIObserver
{
    virtual void onChatAPILogin() {};
    virtual void onChatAPIGetAvailableChildren() {};
    virtual void onChatAPIChildLogin() {};
    virtual void onChatAPIGetFriendList(const FriendList& friendList) {};
    virtual void onChatAPIGetChatMessages(const MessageList& messageList) {};
};

NS_AZOOMEE_CHAT_END

#endif
