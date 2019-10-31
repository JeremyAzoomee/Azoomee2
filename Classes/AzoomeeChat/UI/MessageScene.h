#ifndef AzoomeeChat_MessageScene_h
#define AzoomeeChat_MessageScene_h

#include "../AzoomeeChat.h"
#include "../ChatAPI.h"
#include "MessageListView.h"
#include "TitleBarWidget.h"
#include "MessageComposer.h"
#include <AzoomeeCommon/UI/Scene.h>
#include <AzoomeeCommon/UI/MessageBox.h>
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include <AzoomeeCommon/UI/RequestAdultPinLayer.h>

NS_AZOOMEE_CHAT_BEGIN
    
class MessageScene : public Azoomee::Scene, public ChatAPIObserver, public MessageComposer::Delegate, public MessageBoxDelegate, public RequestAdultPinLayerDelegate
{
    typedef Azoomee::Scene Super;
private:
    
    static const cocos2d::Vec2 kPaddingPercent;
    
    /// Participants in the conversation
    FriendList _participants;
    /// Time for next auto get messages call
    float _timeTillGet = -1.0f;
    /// Recent messages sorted by timestamp
    MessageList _messagesByTime;
    
    /// Root layout for all elements
    cocos2d::ui::Layout* _rootLayout = nullptr;
    
    /// Titlebar at the top of the screen
    TitleBarWidget* _titleBar = nullptr;
    /// Container layout for the content (always sits below the titlebar)
    cocos2d::ui::Layout* _contentLayout = nullptr;
    /// padding layout
    cocos2d::ui::Layout* _paddingLayout = nullptr;
    
    /// Message List
    MessageListView* _messageListView = nullptr;
    /// Message composer
    MessageComposer* _messageComposer = nullptr;
    
    /// Listener which is triggered on the message list reaching the top
    cocos2d::EventListenerCustom* _listReachedTopEventListener = nullptr;
    /// Are we currently checking for new messages?
    bool _checkingForNewMessages = false;
    
    /// Polling schedule
    struct PollingSchedule {
        /// Number of zero attempts (i.e no new messages) before moving to next stage
        int maxAttempts;
        /// Interval in seconds
        float interval;
        /// Current attempts count
        int attempts;
    };
    PollingSchedule _pollingSchedule[4] = {
        /// 5 attempts @ 5s
        { 5, 5.0f, 0 },
        /// 5 attempts @ 10s
        { 5, 10.0f, 0 },
        /// 5 attempts @ 20s
        { 5, 20.0f, 0 },
        /// Continue forever @ 30s
        { 0, 30.0f, 0 }
    };
    const int _pollingScheduleCount = sizeof(_pollingSchedule) / sizeof(_pollingSchedule[0]);
    int _currentPollSchedule = 0;
    
    
    /// Private construction: Use ::create
    MessageScene(const FriendList& participants);
    
    /// Create the content UI
    void createContentUI(cocos2d::ui::Layout* parent);
    
    /// Back button was pressed
    void onBackButtonPressed();
    
    /// Alert button was pressed
    void onReportButtonPressed();
    
    /// Report reset button was pressed
    void onReportResetButtonPressed();
    
    // - ChatAPIObserver
    void onChatAPIGetChatMessages(const MessageList& messageList) override;
    void onChatAPISendMessage(const MessageRef& sentMessage) override;
    void onChatAPIErrorRecieved(const std::string& requestTag, long errorCode) override;
    void onChatAPIGetFriendList(const FriendList& friendList, int amountOfNewMessages) override;
    void onChatAPIModerationStatusChanged(const FriendRef& friendObj) override;
    
    // - MessageComposer::Delegate
    void onMessageComposerSendMessage(const MessageRef& message) override;
    
    // - MessageBoxDelegate
    void MessageBoxButtonPressed(std::string messageBoxTitle,std::string buttonTitle) override;
    void AdultPinCancelled(RequestAdultPinLayer* layer) override;
    void AdultPinAccepted(RequestAdultPinLayer* layer) override;

    // - Retrieve history when message list view reached top
    void createEventListenerForRetrievingHistory();
    
    /// Check for new messages
    void checkForNewMessages(int page = 0);
    /// Retrieve older messages (trigged by scrolling to top)
    void retrieveMessagesFromNextPage();
    /// Is message already in the list of messages
    bool isMessageInHistory(const MessageRef& message);
    /// Reset the polling schedule
    void resetPollingSchedule();

protected:
    
    /// Called when the content size of this scene has changed
    virtual void onSizeChanged() override;
    
public:
    
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;
    virtual void update(float dt) override;
    
    static MessageScene* create(const FriendList& participants);
};

NS_AZOOMEE_CHAT_END

#endif
