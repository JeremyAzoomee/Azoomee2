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
    
    /// Index profileId with name
    std::map<std::string, std::string> _profileNames;
    
    // Private construction - Use ::getInstance()
    ChatAPI();
    
    /// Update the profile names based on the current child and their friend's list
    void updateProfileNames();
    
    /// Handle a moderation status change response
    void onModerationStatusResponseSuccess(const std::string& requestTag, const std::string& headers, const std::string& body);
    
    /// - HttpRequestCreatorResponseDelegate
    void onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body) override;
    void onHttpRequestFailed(const std::string& requestTag, long errorCode) override;
    
public:
    
    /// A low schedule rate, suitable for notifications outside of chat
    static const float kScheduleRateLow;
    /// A higher schedule rate, used when user is inside chat
    static const float kScheduleRateHigh;
    
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
    void requestMessageHistory(const FriendRef& friendObj, int pageNumber);
    
    /// Send a message
    void sendMessage(const FriendRef& friendObj, const MessageRef& message);
    
    /// Mark messages with friend as read
    void markMessagesAsRead(const FriendRef& friendObj, const MessageRef& message);
    
    /// Report a problematic chat to parents
    void reportChat(const FriendRef& friendObj);
    
    /// Reset a reported chat by the parent
    void resetReportedChat(const FriendRef& friendObj);
    
    /// Get timeline summary
    void getTimelineSummary();
    
	/// Refresh Child session
	void refreshChildSession();
	
    /// - Schedule Poll
    bool isFriendListPollScheduled();
    void scheduleFriendListPoll(float interval = kScheduleRateLow);
    void unscheduleFriendListPoll();

};

/**
 * Recieve events relating to the chat API.
 */
struct ChatAPIObserver
{
    /// Friend List success response
    virtual void onChatAPIGetFriendList(const FriendList& friendList, int amountOfNewMessages) {};
    /// Get message list success response
    virtual void onChatAPIGetChatMessages(const MessageList& messageList) {};
    /// Send message success response
    virtual void onChatAPISendMessage(const MessageRef& sentMessage) {};
    /// Get Timeline Summary response
    virtual void onChatAPIGetTimelineSummary(const MessageList& messageList) {};
    /// Conversation moderation status changed
    virtual void onChatAPIModerationStatusChanged(const FriendRef& friendObj) {};
    /// API error from Chat request
    virtual void onChatAPIErrorRecieved(const std::string& requestTag, long errorCode) {};
	/// Refresh child session response
	virtual void onChatAPIRefreshChildSession() {};
};

NS_AZOOMEE_CHAT_END

#endif
