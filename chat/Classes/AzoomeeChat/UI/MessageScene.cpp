#include "MessageScene.h"
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/UI/ModalMessages.h>
#include <AzoomeeCommon/UI/SplitLayout.h>
#include <AzoomeeCommon/Audio/AudioMixer.h>
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include <AzoomeeCommon/Data/Parent/ParentDataProvider.h>

#include "FriendListScene.h"


using namespace cocos2d;


// Enable this to use polling
// For launch, we don't use this at all
//#define CHAT_MESSAGES_POLL


NS_AZOOMEE_CHAT_BEGIN

#ifdef CHAT_MESSAGES_POLL
// Interval to do an auto get call
const float kAutoGetTimeInterval = 5.0f;
#endif

MessageScene* MessageScene::create(const FriendList& participants)
{
    MessageScene* scene = new(std::nothrow) MessageScene(participants);
    if(scene && scene->init())
    {
        scene->autorelease();
        return scene;
    }
    else
    {
        delete scene;
        return nullptr;
    }
}

MessageScene::MessageScene(const FriendList& participants) :
    _participants(participants)
{
}

bool MessageScene::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    // Create the root layout which fills the whole screen
    _rootLayout = ui::Layout::create();
    _rootLayout->setSizeType(ui::Widget::SizeType::PERCENT);
    _rootLayout->setSizePercent(Vec2(1.0f, 1.0f));
    _rootLayout->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    _rootLayout->setBackGroundColor(Style::Color::black);
    _rootLayout->setLayoutType(ui::Layout::Type::RELATIVE);
    addChild(_rootLayout);
    
    // Content layout underneath titlebar
    _contentLayout = ui::Layout::create();
    _contentLayout->setSizeType(ui::Widget::SizeType::PERCENT);
    _contentLayout->setLayoutParameter(CreateBottomCenterRelativeLayoutParam());
    _rootLayout->addChild(_contentLayout);
    
    // Titlebar at the top
    // We add this last so it sits on top with a drop shadow
    _titleBar = TitleBarWidget::create();
    _titleBar->setTitleAvatar(_participants[1]);
    _titleBar->setSizeType(ui::Widget::SizeType::PERCENT);
    _titleBar->setLayoutParameter(CreateTopCenterRelativeLayoutParam());
    _titleBar->addBackButtonEventListener([this](Ref* button){
        onBackButtonPressed();
    });
    _titleBar->addReportButtonEventListener([this](Ref* button){
        onReportButtonPressed();
    });
    _titleBar->addReportResetButtonEventListener([this](Ref* button){
        onReportResetButtonPressed();
    });

    _rootLayout->addChild(_titleBar);
    
    if(_participants[0]->friendId() == ParentDataProvider::getInstance()->getLoggedInParentId())
    {
        _titleBar->setChatReportingToForbidden();
    }

    
    createContentUI(_contentLayout);
    
    return true;
}

void MessageScene::onEnter()
{
    Super::onEnter();
    
    // Register for API events
    ChatAPI::getInstance()->registerObserver(this);
    
    // Get message list
    getMessageHistory();
    ModalMessages::getInstance()->startLoading();
    
    // Show if message list is inModeration
    if(_participants[1]->inModeration()) _titleBar->setChatToInModeration();
    
    // Get update calls
    scheduleUpdate();
    
    //Get scrollview on top notifications
    this->createEventListenerForRetrievingHistory();
}

void MessageScene::onExit()
{
    Super::onExit();
    
    // Unregister on chat API events
    ChatAPI::getInstance()->removeObserver(this);
    
    // Stop update calls
    unscheduleUpdate();
    
    //Unregister history retrieving listener calls
    Director::getInstance()->getEventDispatcher()->removeEventListener(_listener);
}

void MessageScene::update(float dt)
{
#ifdef CHAT_MESSAGES_POLL
    if(kAutoGetTimeInterval > 0 && _timeTillGet > -1)
    {
        _timeTillGet -= dt;
        if(_timeTillGet <= 0)
        {
            // Wait until we get results before restarting the timer
            _timeTillGet = -1.0f;
            
            // Make the call
            getMessageHistory();
        }
    }
#endif
}

void MessageScene::getMessageHistory()
{
    int calculatedPageNumber = int(_messagesByTime.size() / MessageListView::kMessagesOnPage);
    _historyUpdateInProgress = true;
    ChatAPI::getInstance()->requestMessageHistory(_participants[1], calculatedPageNumber);
}

bool MessageScene::isMessageInHistory(const MessageRef &message)
{
    if(_messagesByTime.empty()) return false;
    
    for(auto listItem : _messagesByTime)
    {
        if(listItem->messageId() == message->messageId()) return true;
    }
    
    return false;
}

void MessageScene::createEventListenerForRetrievingHistory()
{
    _listener = EventListenerCustom::create(MessageListView::kEventListenerFlag, [=](EventCustom* event){
        if(!_historyUpdateInProgress)
        {
            this->getMessageHistory();
        }
    });
    
    _eventDispatcher->addEventListenerWithFixedPriority(_listener, 1);
}

#pragma mark - Size Changes

void MessageScene::onSizeChanged()
{
    Super::onSizeChanged();
    
    const cocos2d::Size& contentSize = getContentSize();
    // TODO: Grab sizes from config
    const bool isLandscape = contentSize.width > contentSize.height;
    
    // Main layout
    const Vec2& titleBarSize = Vec2(1.0f, (isLandscape) ? 0.131f : 0.084f);
    _titleBar->setSizePercent(titleBarSize);
    _contentLayout->setSizePercent(Vec2(1.0f, 1.0f - titleBarSize.y));
}

#pragma mark - UI creation

void MessageScene::createContentUI(cocos2d::ui::Layout* parent)
{
    parent->setLayoutType(ui::Layout::Type::RELATIVE);
    
    // Use a split view to manage the layout of the message list and composer
    SplitLayout* splitLayout = SplitLayout::create();
    splitLayout->setSizePercent(Vec2(1.0f, 1.0f));
    splitLayout->setSizeType(ui::Widget::SizeType::PERCENT);
    splitLayout->setMode(SplitLayout::Mode::Vertical);
    // Message list area = fill to size, composer area = FixedSize
    splitLayout->setSplitBehaviour(SplitLayout::FillSize, SplitLayout::FixedSize);
    parent->addChild(splitLayout);
    
    // Message list
    ui::Layout* messageAreaLayout = splitLayout->firstLayout();
    _messageListView = MessageListView::create();
    _messageListView->setSizePercent(Vec2(1.0f, 1.0f));
    _messageListView->setSizeType(ui::Widget::SizeType::PERCENT);
    messageAreaLayout->addChild(_messageListView);
    
    // Composer
    // Note the composer controls it's own size so we don't set it here
    ui::Layout* composerAreaLayout = splitLayout->secondLayout();
    _messageComposer = MessageComposer::create();
    _messageComposer->setDelegate(this);
    _messageComposer->addSizeChangedEventListener([this, composerAreaLayout](ui::Layout* composer){
        // Make sure the parent height matches the height of the composer
        Size composerAreaSize = composerAreaLayout->getContentSize();
        const Size& composerSize = composer->getContentSize();
        if(composerAreaSize.height != composerSize.height)
        {
            composerAreaSize.height = composerSize.height;
            composerAreaLayout->setContentSize(composerAreaSize);
        }
    });
    composerAreaLayout->addChild(_messageComposer);
    Size composerAreaSize = composerAreaLayout->getContentSize();
    composerAreaSize.height = _messageComposer->getContentSize().height;
    composerAreaLayout->setContentSize(composerAreaSize);
}

#pragma mark - Interaction

void MessageScene::onBackButtonPressed()
{
    AudioMixer::getInstance()->playEffect(BACK_BUTTON_AUDIO_EFFECT);
    AnalyticsSingleton::getInstance()->genericButtonPressEvent("ChatWindow - BackButton");
    
    // Back to friend list
    auto friendListScene = FriendListScene::create();
    Director::getInstance()->replaceScene(TransitionSlideInL::create(0.25f, friendListScene));
}

void MessageScene::onReportButtonPressed()
{
    AudioMixer::getInstance()->playEffect(SETTINGS_BUTTON_AUDIO_EFFECT);
    AnalyticsSingleton::getInstance()->genericButtonPressEvent("ChatWindow - ReportButton");
    
    MessageBox::createWithLayer(ChatReportForModeration, this);
}

void MessageScene::onReportResetButtonPressed()
{
    AudioMixer::getInstance()->playEffect(SETTINGS_BUTTON_AUDIO_EFFECT);
    RequestAdultPinLayer::create()->setDelegate(this);
}

#pragma mark - ChatAPIObserver

void MessageScene::onChatAPIGetChatMessages(const MessageList& messageList)
{
    // Make a copy of the messageList and sort it in order of timestamp
    MessageList messagesByTime;
    if(!_messagesByTime.empty()) messagesByTime = _messagesByTime;
    
    for(const MessageRef& message : messageList)
    {
        // Find first item where this message is newer
        MessageList::const_reverse_iterator it = messagesByTime.rbegin();
        for(; it != messagesByTime.rend(); ++it)
        {
            if(message->timestamp() > (*it)->timestamp())
            {
                break;
            }
        }
        if(!isMessageInHistory(message)) messagesByTime.insert(it.base(), message);
    }
    
    _messagesByTime = messagesByTime;
    
    _messageListView->setData(_participants, _messagesByTime);
    
    ModalMessages::getInstance()->stopLoading();
    
    if(messageList.size() > 0)
    {
        // Mark messages as read and enable reporting
        _titleBar->onChatActivityHappened();
        ChatAPI::getInstance()->markMessagesAsRead(_participants[1], _messagesByTime.back());
    }
    
#ifdef CHAT_MESSAGES_POLL
    // Trigger auto get again
    _timeTillGet = kAutoGetTimeInterval;
#endif
    
    if(messageList.size() >= MessageListView::kMessagesOnPage) _historyUpdateInProgress = false; //if downloaded messages are less than kMessagesOnPage (20 on server) in length, then we got to the beginning of the conversation, no further retrievals are required.
}

void MessageScene::onChatAPISendMessage(const MessageRef& sentMessage)
{
    _historyUpdateInProgress = true;
    _messagesByTime.push_back(sentMessage);
    _messageListView->setData(_participants, _messagesByTime);
    _historyUpdateInProgress = false;
}

void MessageScene::onChatAPIMessageRecieved(const MessageRef& message)
{
    _historyUpdateInProgress = true;
    AnalyticsSingleton::getInstance()->chatIncomingMessageEvent(message->messageType());
    _messagesByTime.push_back(message);
    _messageListView->setData(_participants, _messagesByTime);
    _titleBar->onChatActivityHappened();
    
    // Mark messages as read
    ChatAPI::getInstance()->markMessagesAsRead(_participants[1], message);
    
    _historyUpdateInProgress = false;
}

void MessageScene::onChatAPICustomMessageReceived(const std::string& messageType, std::map<std::string, std::string> messageProperties)
{
    if(messageType != "MODERATION") return;
    if(messageProperties["otherChildId"] != _participants[1]->friendId()) return;
    
    if(messageProperties["status"] == "IN_MODERATION") _titleBar->setChatToInModeration();
    if(messageProperties["status"] == "ACTIVE") _titleBar->setChatToActive();
}

void MessageScene::onChatAPIErrorRecieved(const std::string& requestTag, long errorCode)
{
    ModalMessages::getInstance()->stopLoading();
    MessageBox::createWith(ERROR_CODE_SOMETHING_WENT_WRONG, nullptr);
}

#pragma mark - MessageComposer::Delegate

void MessageScene::onMessageComposerSendMessage(const MessageRef& message)
{
    AnalyticsSingleton::getInstance()->chatOutgoingMessageEvent(message->messageType());
    ChatAPI::getInstance()->sendMessage(_participants[1], message);
    _titleBar->onChatActivityHappened();
    
#ifdef CHAT_MESSAGES_POLL
    _timeTillGet = -1.0f;
#endif
}

#pragma mark - MessageBoxDelegate

void MessageScene::MessageBoxButtonPressed(std::string messageBoxTitle,std::string buttonTitle)
{
    if(buttonTitle == "Report")
    {
        ChatAPI::getInstance()->reportChat(_participants[1]);
    }
    
    if(buttonTitle == "Reset")
    {
        ChatAPI::getInstance()->resetReportedChat(_participants[1]);
    }
}

#pragma mark - RequestAdultPinDelegate

void MessageScene::AdultPinCancelled(RequestAdultPinLayer* layer)
{
    
}

void MessageScene::AdultPinAccepted(RequestAdultPinLayer* layer)
{
    MessageBox::createWithLayer(ChatResetModeration,{ {"Child1", _participants[0]->friendName()},{"Child2",  _participants[1]->friendName()} },this);
}

NS_AZOOMEE_CHAT_END
