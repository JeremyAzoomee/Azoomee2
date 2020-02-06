#include "MessageScene.h"
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/UI/ModalMessages.h>
#include <AzoomeeCommon/UI/SplitLayout.h>
#include <AzoomeeCommon/Audio/AudioMixer.h>
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include <AzoomeeCommon/Data/Parent/UserAccountManager.h>
#include <AzoomeeCommon/Utils/LocaleManager.h>
#include "FriendListScene.h"
#include "../../HQHistoryManager.h"

#include "../../PopupMessageBox.h"

#include "../../SceneManagerScene.h"

using namespace cocos2d;


NS_AZOOMEE_CHAT_BEGIN

const Vec2 MessageScene::kPaddingPercent = Vec2(0.02, 0.01);

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
    
    ui::Layout* bgColour = ui::Layout::create();
    bgColour->setSizeType(ui::Widget::SizeType::PERCENT);
    bgColour->setSizePercent(Vec2(1.0f, 1.0f));
    bgColour->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    bgColour->setBackGroundColor(Style::Color::darkIndigo);
    addChild(bgColour);
    
    // Create the root layout which fills the whole screen
    _rootLayout = ui::Layout::create();
    _rootLayout->setSizeType(ui::Widget::SizeType::PERCENT);
    _rootLayout->setSizePercent(Vec2(1.0f, 1.0f));
    _rootLayout->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    _rootLayout->setBackGroundColor(Style::Color::darkIndigo);
    _rootLayout->setLayoutType(ui::Layout::Type::VERTICAL);
    addChild(_rootLayout);
    
    _paddingLayout = ui::Layout::create();
    _paddingLayout->setSizeType(ui::Layout::SizeType::PERCENT);
    _paddingLayout->setSizePercent(Vec2(1.0f, kPaddingPercent.y));
    _rootLayout->addChild(_paddingLayout);
    
    // Titlebar at the top
    // We add this last so it sits on top with a drop shadow
    _titleBar = TitleBarWidget::create();
    _titleBar->setTitleAvatar(_participants[1]);
    _titleBar->setSizeType(ui::Widget::SizeType::PERCENT);
    _titleBar->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
    _titleBar->addBackButtonEventListener([this](Ref* button){
        onBackButtonPressed();
    });
    _titleBar->addReportButtonEventListener([this](Ref* button){
        onReportButtonPressed();
    });
    _titleBar->addReportResetButtonEventListener([this](Ref* button){
        onReportResetButtonPressed();
    });
    _titleBar->underlineTitleBar();
    
    _rootLayout->addChild(_titleBar);
    
    if(_participants[0]->friendId() == UserAccountManager::getInstance()->getLoggedInParentId())
    {
        _titleBar->setChatReportingToForbidden();
    }
    
    // Content layout underneath titlebar
    _contentLayout = ui::Layout::create();
    _contentLayout->setSizeType(ui::Widget::SizeType::PERCENT);
    _contentLayout->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
    _rootLayout->addChild(_contentLayout);

    createContentUI(_contentLayout);
    
    return true;
}

void MessageScene::onEnter()
{
    Super::onEnter();
    
    // Register for API events
    ChatAPI::getInstance()->registerObserver(this);
    
    // Get message list
    retrieveMessagesFromNextPage();
    ModalMessages::getInstance()->startLoading();
    
    // Up the schedule speed of friend list polling
    ChatAPI::getInstance()->scheduleFriendListPoll( ChatAPI::kScheduleRateHigh );
    
    // Show if message list is inModeration
    if(_participants[1]->inModeration())
    {
        _titleBar->setChatToInModeration();
    }
    
    // Get update calls
    scheduleUpdate();
    
    // Get scrollview on top notifications
    createEventListenerForRetrievingHistory();
    
    OnScreenChecker::kUseStrictBoundry = false;
}

void MessageScene::onExit()
{
    Super::onExit();
    
    // Unregister on chat API events
    ChatAPI::getInstance()->removeObserver(this);
    
    // Stop update calls
    unscheduleUpdate();
    
    // Unregister history retrieving listener calls
    if(_listReachedTopEventListener)
    {
        Director::getInstance()->getEventDispatcher()->removeEventListener(_listReachedTopEventListener);
    }
}

void MessageScene::update(float dt)
{
    if(_timeTillGet > -1.0f)
    {
        _timeTillGet -= dt;
        if(_timeTillGet <= 0)
        {
            // Make the call
            checkForNewMessages();
        }
    }
}

void MessageScene::checkForNewMessages(int page)
{
    // Wait until we get results before restarting the timer
    _timeTillGet = -1.0f;
    _checkingForNewMessages = true;
    ChatAPI::getInstance()->requestMessageHistory(_participants[1], page);
}

void MessageScene::retrieveMessagesFromNextPage()
{
    int calculatedPageNumber = int(_messagesByTime.size() / MessageListView::kMessagesOnPage);
    checkForNewMessages(calculatedPageNumber);
}

bool MessageScene::isMessageInHistory(const MessageRef &message)
{
    if(_messagesByTime.empty())
    {
        return false;
    }
    
    for(auto listItem : _messagesByTime)
    {
        if(listItem->messageId() == message->messageId())
        {
            return true;
        }
    }
    
    return false;
}

void MessageScene::createEventListenerForRetrievingHistory()
{
    _listReachedTopEventListener = EventListenerCustom::create(MessageListView::kEventReachedTop, [=](EventCustom* event) {
        // We don't start getting history, if there are less than 20 messages in the container
        // The chat has just started, and the user scrolls to the top, or on the top anyways
        // because of not having enough messages to scroll at all.
        if(_messagesByTime.size() < MessageListView::kMessagesOnPage)
        {
            return;
        }
        
        if(!_checkingForNewMessages)
        {
            retrieveMessagesFromNextPage();
        }
    });
    
    _eventDispatcher->addEventListenerWithFixedPriority(_listReachedTopEventListener, 1);
}

void MessageScene::resetPollingSchedule()
{
    // Reset attempts count
    for(int i = 0; i < _pollingScheduleCount; ++i)
    {
        _pollingSchedule[i].attempts = 0;
    }
    // Start at the first schedule again
    _currentPollSchedule = 0;
}

#pragma mark - Size Changes

void MessageScene::onSizeChanged()
{
    Super::onSizeChanged();
    
    const cocos2d::Size& contentSize = getContentSize();
    const Vec2& origin = Director::getInstance()->getVisibleOrigin();
    // TODO: Grab sizes from config
    const bool isLandscape = contentSize.width > contentSize.height;
    
    // Main layout
    const Vec2& titleBarSize = Vec2(1.0f - kPaddingPercent.x, (isLandscape) ? 0.11f : 0.084f);
    _titleBar->setSizePercent(titleBarSize);
    _contentLayout->setSizePercent(Vec2(1.0f - kPaddingPercent.x, 0.99f - titleBarSize.y));
    
    const Rect& safeAreaRect = Director::getInstance()->getSafeAreaRect();
    const float safeZonePaddingPercentTop = MAX(0, isLandscape ? (contentSize.width - (safeAreaRect.origin.x + safeAreaRect.size.width)) / contentSize.width  : (contentSize.height - (safeAreaRect.origin.y + safeAreaRect.size.height)) / contentSize.height);
    const float safeZonePaddingPercentBottom = MAX(0, isLandscape ? (safeAreaRect.origin.x - origin.x) / contentSize.width  : (safeAreaRect.origin.y - origin.y) / contentSize.height);
    if(isLandscape)
    {
        _rootLayout->setSizePercent(Vec2(1.0f - (2 * MAX(safeZonePaddingPercentTop, safeZonePaddingPercentBottom)), 1.0f));
        _rootLayout->setPosition(Point(this->getContentSize().width * MAX(safeZonePaddingPercentTop, safeZonePaddingPercentBottom) , 0));
    }
    else
    {
        _rootLayout->setSizePercent(Vec2(1.0f, 1.0f - (safeZonePaddingPercentTop + safeZonePaddingPercentBottom)));
        _rootLayout->setPosition(Point(0, this->getContentSize().height * safeZonePaddingPercentBottom));
    }

}

#pragma mark - UI creation

void MessageScene::createContentUI(cocos2d::ui::Layout* parent)
{
    parent->setLayoutType(ui::Layout::Type::RELATIVE);
    
    // Use a split view to manage the layout of the message list and composer
    SplitLayout* splitLayout = SplitLayout::create();
    splitLayout->setSizePercent(Vec2(1.0f, 1.0f));
    splitLayout->setSizeType(ui::Widget::SizeType::PERCENT);
    splitLayout->setOrientation(Orientation::Portrait);
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
    AnalyticsSingleton::getInstance()->contentItemClosedEvent();
    // Back to friend list
    if(_participants[0]->friendId() == UserAccountManager::getInstance()->getLoggedInParentId())
    {
        Director::getInstance()->replaceScene(TransitionSlideInT::create(0.25f, FriendListScene::create()));
    }
    else
    {
        Director::getInstance()->replaceScene(TransitionSlideInT::create(0.25f, HQHistoryManager::getInstance()->getCachedHQScene()));
    }
}

void MessageScene::onReportButtonPressed()
{
    AudioMixer::getInstance()->playEffect(SETTINGS_BUTTON_AUDIO_EFFECT);
    AnalyticsSingleton::getInstance()->genericButtonPressEvent("ChatWindow - ReportButton");
    
    PopupMessageBox* messageBox = PopupMessageBox::create();
    messageBox->setTitle(_("Report chat"));
    messageBox->setBody(_("Do you really want to report this chat to your parents?"));
    messageBox->setButtonText(_("Report"));
    messageBox->setButtonColour(Style::Color::strongPink);
    messageBox->setPatternColour(Style::Color::azure);
    messageBox->setButtonPressedCallback([this](PopupMessageBox* pSender){
        pSender->removeFromParent();
        ChatAPI::getInstance()->reportChat(_participants[1]);
        ModalMessages::getInstance()->startLoading();
    });
    messageBox->setSecondButtonText(_("Cancel"));
    messageBox->setSecondButtonColour(Style::Color::darkIndigo);
    messageBox->setSecondButtonPressedCallback([this](PopupMessageBox* pSender){
        pSender->removeFromParent();
    });
    this->addChild(messageBox, 1);
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
    MessageList messagesByTime = _messagesByTime;
    
    bool gotNewMessages = false;
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
        if(!isMessageInHistory(message))
        {
            gotNewMessages = true;
            messagesByTime.insert(it.base(), message);
        }
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
    
    // Trigger auto get again
    
    // Update schedule
    if(gotNewMessages)
    {
        // Reset schedule if we got new messages
        resetPollingSchedule();
    }
    else
    {
        // Only update schedule if the current schedule has more than 0 max attempts
        // and we're not already on the last shedule
        if(_pollingSchedule[_currentPollSchedule].maxAttempts > 0 &&
           _currentPollSchedule < _pollingScheduleCount - 1)
        {
            if(++_pollingSchedule[_currentPollSchedule].attempts >= _pollingSchedule[_currentPollSchedule].maxAttempts)
            {
                ++_currentPollSchedule;
            }
        }
    }
    
    _timeTillGet = _pollingSchedule[_currentPollSchedule].interval;
    _checkingForNewMessages = false;
}

void MessageScene::onChatAPISendMessage(const MessageRef& sentMessage)
{
    _checkingForNewMessages = true;
    
    _messagesByTime.push_back(sentMessage);
    _messageListView->setData(_participants, _messagesByTime);
    
    // Trigger auto get again
    // Reset schedule
    resetPollingSchedule();
    _timeTillGet = _pollingSchedule[_currentPollSchedule].interval;
    _checkingForNewMessages = false;
}

void MessageScene::onChatAPIErrorRecieved(const std::string& requestTag, long errorCode)
{
    ModalMessages::getInstance()->stopLoading();
    MessageBox::createWith(ERROR_CODE_SOMETHING_WENT_WRONG, nullptr);
}

void MessageScene::onChatAPIGetFriendList(const FriendList& friendList, int amountOfNewMessages)
{
    AnalyticsSingleton::getInstance()->setNumberOfChatFriends((int)friendList.size());
    
    // Find the friend for the current participant
    bool stillTalkingToAFriend = false;
    for(const FriendRef& frnd : friendList)
    {
        if(frnd->friendId() == _participants[1]->friendId())
        {
            stillTalkingToAFriend = true;
            // Update the participant
            _participants[1] = frnd;
        }
    }
    
    // Update conversation moderation status
    if(_participants[1]->inModeration())
    {
        _titleBar->setChatToInModeration();
    }
    else
    {
        _titleBar->setChatToActive();
    }
    
    ModalMessages::getInstance()->stopLoading();
    
    // If we're not talking to a friend anymore, get out of here
    // TODO: Should we display an error? TBD
    if( !stillTalkingToAFriend )
    {
        onBackButtonPressed();
    }
}

void MessageScene::onChatAPIModerationStatusChanged(const FriendRef& friendObj)
{
    if(_participants[1] == friendObj)
    {
        // Update conversation moderation status
        if(_participants[1]->inModeration())
        {
            _titleBar->setChatToInModeration();
        }
        else
        {
            _titleBar->setChatToActive();
        }
    }
}

#pragma mark - MessageComposer::Delegate

void MessageScene::onMessageComposerSendMessage(const MessageRef& message)
{
    _checkingForNewMessages = true;
    AnalyticsSingleton::getInstance()->chatOutgoingMessageEvent(message->messageType());
    ChatAPI::getInstance()->sendMessage(_participants[1], message);
    _titleBar->onChatActivityHappened();
    
    // Pause schedule until the sent message has been confirmed
    _timeTillGet = -1.0f;
}

#pragma mark - MessageBoxDelegate

void MessageScene::MessageBoxButtonPressed(std::string messageBoxTitle,std::string buttonTitle)
{
    if(buttonTitle == MessageBox::kReport)
    {
        ChatAPI::getInstance()->reportChat(_participants[1]);
        ModalMessages::getInstance()->startLoading();
    }
    else if(buttonTitle == MessageBox::kReset)
    {
        ChatAPI::getInstance()->resetReportedChat(_participants[1]);
        ModalMessages::getInstance()->startLoading();
    }
}

#pragma mark - RequestAdultPinDelegate

void MessageScene::AdultPinCancelled(RequestAdultPinLayer* layer)
{
    
}

void MessageScene::AdultPinAccepted(RequestAdultPinLayer* layer)
{
    std::string replacedText = StringFunctions::stringReplace(_("Reset %s1 & %s2's conversation?"), "%s1", _participants[0]->friendName());
    
    replacedText = StringFunctions::stringReplace(replacedText, "%s2", _participants[1]->friendName());
    
    PopupMessageBox* messageBox = PopupMessageBox::create();
    messageBox->setTitle(_("Reset chat"));
    messageBox->setBody(replacedText);
    messageBox->setButtonText(_("Reset"));
    messageBox->setButtonColour(Style::Color::strongPink);
    messageBox->setPatternColour(Style::Color::azure);
    messageBox->setButtonPressedCallback([this](PopupMessageBox* pSender){
        pSender->removeFromParent();
        ChatAPI::getInstance()->resetReportedChat(_participants[1]);
        ModalMessages::getInstance()->startLoading();
    });
    messageBox->setSecondButtonText(_("Cancel"));
    messageBox->setSecondButtonColour(Style::Color::darkIndigo);
    messageBox->setSecondButtonPressedCallback([this](PopupMessageBox* pSender){
        pSender->removeFromParent();
    });
    this->addChild(messageBox, 1);
}

NS_AZOOMEE_CHAT_END
