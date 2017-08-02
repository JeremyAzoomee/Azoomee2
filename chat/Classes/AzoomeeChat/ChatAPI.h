#ifndef AzoomeeChat_ChatAPI_h
#define AzoomeeChat_ChatAPI_h

#include "AzoomeeChat.h"
#include "Data/Friend.h"
#include "Data/Message.h"
#include <AzoomeeCommon/API/API.h>
#include <AzoomeeCommon/Pusher/PusherSDK.h>
#include <map>


NS_AZOOMEE_CHAT_BEGIN

// forward decleration
class ChatAPIObserver;

/**
 * Manages communication of chat APIs with the Azoomee server.
 */
class ChatAPI : private HttpRequestCreatorResponseDelegate, public PusherEventObserver
{
private:
    /// Most recent friend list
    FriendList _friendList;
    /// Index Friends by Id
    std::map<std::string, FriendRef> _friendIndex;
    /// Observers monitoring this API and it's responses
    std::vector<ChatAPIObserver*> _observers;
    
    /// Index profileId with name
    std::map<std::string, std::string> _profileNames;
    
    // Private construction - Use ::getInstance()
    ChatAPI();
    
    /// Update the profile names based on the current child and their friend's list
    void updateProfileNames();
    
    /// - HttpRequestCreatorResponseDelegate
    void onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body) override;
    void onHttpRequestFailed(const std::string& requestTag, long errorCode) override;
    
    /// - PusherEventObserver
    void onPusherEventRecieved(const PusherEventRef& event) override;
    
    /// - Schedule Poll
    void scheduleFriendListPoll();
    void rescheduleFriendListPoll();
    void unscheduleFriendListPoll();
    bool friendListPollScheduled();
    
public:
    
    static ChatAPI* getInstance();
    virtual ~ChatAPI();
    
    /// Register an observer
    void registerObserver(ChatAPIObserver* observer);
    /// Remove a previously registered observer
    void removeObserver(ChatAPIObserver* observer);
    
    /// Returns the name for profile, if known
    std::string getProfileName(const std::string& profileId);
    
    /// Request the friend list from the server
    /// Response: ChatAPIObserver::onChatAPIGetFriendList
    void requestFriendList();
    /// Get the latest friend list we have on the server
    FriendList getFriendList() const;
    
    /// Get the chat messages for a contact
    /// Response: ChatAPIObserver::onChatAPIGetChatMessages
    void requestMessageHistory(const FriendRef& friendObj);
    
    /// Send a message
    void sendMessage(const FriendRef& friendObj, const MessageRef& message);
    
    /// Mark messages with friend as read
    void markMessagesAsRead(const FriendRef& friendObj, const MessageRef& message);
    
    //Report a problematic chat to parents
    void reportChat(const FriendRef& friendObj);
    
    //Reset a reported chat by the parent
    void resetReportedChat(const FriendRef& friendObj);
    
    /// For azoomee2 notifications we start and schedule polling of friendlist
    void startFriendListManualPoll();

};

/**
 * Recieve events relating to the chat API.
 */
struct ChatAPIObserver
{
    /// Friend List success response
    virtual void onChatAPIGetFriendList(const FriendList& friendList) {};
    /// Get message list success response
    virtual void onChatAPIGetChatMessages(const MessageList& messageList) {};
    /// Send message success response
    virtual void onChatAPISendMessage(const MessageRef& sentMessage) {};
    /// API error from Chat request
    virtual void onChatAPIErrorRecieved(const std::string& requestTag, long errorCode) {};
    /// A chat message was recieved
    virtual void onChatAPIMessageRecieved(const MessageRef& message) {};
<<<<<<< HEAD
    /// A custom (command) message was received
    virtual void onChatAPICustomMessageReceived(const std::string& messageType, std::map<std::string, std::string> messageProperties) {};
    /// Response on reporting a chat received
    virtual void onChatAPIReportChatSuccessful(const std::string& requestTag) {};
=======
    /// Notification about new messages
    virtual void onChatAPINewMessageNotificationReceived(int amountOfNewMessages) {};
>>>>>>> origin/master
};

NS_AZOOMEE_CHAT_END

#endif
