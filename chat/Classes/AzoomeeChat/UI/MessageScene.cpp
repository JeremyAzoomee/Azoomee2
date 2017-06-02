#include "MessageScene.h"
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/UI/ModalMessages.h>
#include <AzoomeeCommon/UI/SplitLayout.h>

// TODO: This needs to be a dynamic hook, so app can deal with it when we're in the main app
#include "FriendListScene.h"


using namespace cocos2d;



NS_AZOOMEE_CHAT_BEGIN

// Interval to do an auto get call
// Temp feature until Pusher is implemented
const float kAutoGetTimeInterval = 0.0f; //5.0f;

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
    _titleBar->setTitleString(_participants[1]->friendName());
    _titleBar->setTitleColor(Style::Color::heliotropeTwo);
    _titleBar->setSizeType(ui::Widget::SizeType::PERCENT);
    _titleBar->setLayoutParameter(CreateTopCenterRelativeLayoutParam());
    _titleBar->addBackButtonEventListener([this](Ref* button){
        onBackButtonPressed();
    });
    _titleBar->showAlertButton(true);
//    _titleBar->addAlertButtonEventListener([this](Ref* button){
//        onAlertButtonPressed();
//    });
    _rootLayout->addChild(_titleBar);
    
    createContentUI(_contentLayout);
    
    return true;
}

void MessageScene::onEnter()
{
    Super::onEnter();
    
    // Register for API events
    ChatAPI::getInstance()->registerObserver(this);
    
    // Get message list
    ChatAPI::getInstance()->requestMessageHistory(_participants[1]);
    ModalMessages::getInstance()->startLoading();
    
    // Get update calls
    scheduleUpdate();
}

void MessageScene::onExit()
{
    Super::onExit();
    
    // Unregister on chat API events
    ChatAPI::getInstance()->removeObserver(this);
    
    // Stop update calls
    unscheduleUpdate();
}

void MessageScene::update(float dt)
{
    if(kAutoGetTimeInterval > 0 && _timeTillGet > -1)
    {
        _timeTillGet -= dt;
        if(_timeTillGet <= 0)
        {
            // Wait until we get results before restarting the timer
            _timeTillGet = -1.0f;
            
            // Make the call
            ChatAPI::getInstance()->requestMessageHistory(_participants[1]);
        }
    }
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
    // Back to friend list
    auto friendListScene = FriendListScene::create();
    Director::getInstance()->replaceScene(TransitionSlideInL::create(0.25f, friendListScene));
}

#pragma mark - ChatAPIObserver

void MessageScene::onChatAPIGetChatMessages(const MessageList& messageList)
{
    _messageListView->setData(_participants, messageList);
    ModalMessages::getInstance()->stopLoading();
    
    // Trigger auto get again
    _timeTillGet = kAutoGetTimeInterval;
}

void MessageScene::onChatAPISendMessage(const MessageRef& sentMessage)
{
    // Auto get new messages until we have a live feed from PUSHER
    ChatAPI::getInstance()->requestMessageHistory(_participants[1]);
}

#pragma mark - MessageComposer::Delegate

void MessageScene::onMessageComposerSendMessage(const MessageRef& message)
{
//    cocos2d::log("Send Message: %s", message.c_str());
    ChatAPI::getInstance()->sendMessage(_participants[1], message);
    _timeTillGet = -1.0f;
    
    // TODO: Add to message list
}

NS_AZOOMEE_CHAT_END
