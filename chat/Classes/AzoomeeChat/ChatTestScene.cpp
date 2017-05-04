#include "ChatTestScene.h"
#include <AzoomeeCommon/Data/Child/ChildDataProvider.h>
#include <AzoomeeCommon/UI/ElectricDreamsTextStyles.h>
#include <AzoomeeCommon/UI/ModalMessages.h>
#include "../AppDelegate.h"
#include <sstream>

using namespace cocos2d;


NS_AZOOMEE_CHAT_BEGIN
    
ChatTestScene::~ChatTestScene()
{
    CC_SAFE_RELEASE(_rootLayout);
    CC_SAFE_RELEASE(_leftLayout);
    CC_SAFE_RELEASE(_rightLayout);
    CC_SAFE_RELEASE(_contactListView);
    CC_SAFE_RELEASE(_contactListViewItem);
    CC_SAFE_RELEASE(_messageListView);
    CC_SAFE_RELEASE(_messageListViewItem);
    CC_SAFE_RELEASE(_messageEntryField);
    CC_SAFE_RELEASE(_sendButton);
}

bool ChatTestScene::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    const cocos2d::Size& visibleSize = getContentSize();
    cocos2d::log( "visibleSize: %f, %f", visibleSize.width, visibleSize.height );
    
    
    // Create the root layout which fills the whole screen
    _rootLayout = ui::Layout::create();
    _rootLayout->retain();
    _rootLayout->setAnchorPoint(Vec2::ZERO);
    _rootLayout->setContentSize(visibleSize);
    _rootLayout->setPosition(Point::ZERO);
    _rootLayout->setLayoutType(ui::Layout::Type::RELATIVE);
    _rootLayout->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    _rootLayout->setBackGroundColor(Color3B(50, 50, 50));
    addChild(_rootLayout);
    
    // Create the left and right side parent layouts to hold the elements
    _leftLayout = ui::Layout::create();
    _leftLayout->retain();
    _leftLayout->setAnchorPoint(Vec2(0, 1));
    _leftLayout->setLayoutType(ui::Layout::Type::VERTICAL);
    ui::RelativeLayoutParameter* leftLayoutParam = ui::RelativeLayoutParameter::create();
    leftLayoutParam->setAlign(ui::RelativeLayoutParameter::RelativeAlign::PARENT_TOP_LEFT);
    _leftLayout->setLayoutParameter(leftLayoutParam);
    _rootLayout->addChild(_leftLayout);
    
    _rightLayout = ui::Layout::create();
    _rightLayout->retain();
    _rightLayout->setAnchorPoint(Vec2(0, 1));
    _rightLayout->setLayoutType(ui::Layout::Type::VERTICAL);
    ui::RelativeLayoutParameter* rightLayoutParam = ui::RelativeLayoutParameter::create();
    rightLayoutParam->setAlign(ui::RelativeLayoutParameter::RelativeAlign::PARENT_TOP_RIGHT);
    _rightLayout->setLayoutParameter(rightLayoutParam);
    _rootLayout->addChild(_rightLayout);
    
    // Size elements
    float leftWidth = visibleSize.width * 0.3f;
    _leftLayout->setContentSize(Size(leftWidth, visibleSize.height));
    float rightWidth = visibleSize.width - leftWidth;
    _rightLayout->setContentSize(Size(rightWidth, visibleSize.height));
    
    
    // Now we can create the UI for each side
    createLeftSideUI(_leftLayout);
    createRightSideUI(_rightLayout);
  
    return true;
}

void ChatTestScene::onEnter()
{
    Super::onEnter();
    
    // Register for window changes
    auto director = cocos2d::Director::getInstance();
    _windowChangedEvent = director->getEventDispatcher()->addCustomEventListener(AppDelegate::EVENT_WINDOW_SIZE_CHANGED,
                                                                                 std::bind(&ChatTestScene::onWindowChanged, this, std::placeholders::_1));
    _windowChangedEvent->retain();
    
    // Register for API events
    ChatAPI::getInstance()->registerObserver(this);
    
    // Login automatically for this test
    ChatAPI::getInstance()->loginUser("mitch@azoomee.com", "test1234");
    ModalMessages::getInstance()->startLoading();
}

void ChatTestScene::onExit()
{
    Super::onExit();
    
    // Unregister window changes
    auto director = cocos2d::Director::getInstance();
    director->getEventDispatcher()->removeEventListener(_windowChangedEvent);
    CC_SAFE_RELEASE(_windowChangedEvent);
    
    // Unregister on chat API events
    ChatAPI::getInstance()->removeObserver(this);
}

void ChatTestScene::onWindowChanged(cocos2d::EventCustom* event)
{
    cocos2d::Size visibleSize = getContentSize();
    cocos2d::log("onWindowChanged: %f, %f", visibleSize.width, visibleSize.height);
    
    // Size elements
    _rootLayout->setContentSize(visibleSize);
    float leftWidth = visibleSize.width * 0.3f;
    _leftLayout->setContentSize(Size(leftWidth, visibleSize.height));
    float rightWidth = visibleSize.width - leftWidth;
    _rightLayout->setContentSize(Size(rightWidth, visibleSize.height));
    
//    _rootLayout->forceDoLayout();
//    _leftLayout->forceDoLayout();
//    _rightLayout->forceDoLayout();
    
    // TODO: List/Scrollviews need to resize
    // TODO: Animate this change?
}

#pragma mark - UI

void ChatTestScene::createLeftSideUI(cocos2d::ui::Layout* parent)
{
    const cocos2d::Size& visibleSize = parent->getContentSize();
    
    // Contact list container
    _contactListView = ui::ListView::create();
    _contactListView->retain();
    _contactListView->setAnchorPoint(Vec2(0, 1));
    _contactListView->setDirection(ui::ScrollView::Direction::VERTICAL);
    _contactListView->setBounceEnabled(true);
    _contactListView->setGravity(ui::ListView::Gravity::CENTER_VERTICAL);
    _contactListView->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    _contactListView->setBackGroundColor(Color3B(100, 100, 100));
    _contactListView->setContentSize(Size(visibleSize.width, visibleSize.height));
    _contactListView->setItemsMargin(20.0f);
    _contactListView->addEventListener([this](Ref* sender, ui::ListView::EventType type) {
        if(type == ui::ListView::EventType::ON_SELECTED_ITEM_END)
        {
            ModalMessages::getInstance()->startLoading();
            ChatAPI* chatAPI = ChatAPI::getInstance();
            const FriendList& friendList = chatAPI->getFriendList();
            ssize_t index = _contactListView->getCurSelectedIndex();
            const std::string& friendId = friendList[index]->friendId();
            chatAPI->requestMessageHistory(friendId);
        }
    });
    
    ui::LinearLayoutParameter* contactListLayout = ui::LinearLayoutParameter::create();
    contactListLayout->setGravity(ui::LinearLayoutParameter::LinearGravity::TOP);
    _contactListView->setLayoutParameter(contactListLayout);
    parent->addChild(_contactListView);
    
    // Contact list item
    _contactListViewItem = ui::Button::create("res/modal/generic_button_slice_ready.png");
    _contactListViewItem->retain();
    _contactListViewItem->setAnchorPoint(Vec2(0, 1));
    _contactListViewItem->setCapInsets(Rect(98, 98, 1, 1));
    _contactListViewItem->setScale9Enabled(true);
    _contactListViewItem->setTitleFontName(Azoomee::FONT_REGULAR);
    _contactListViewItem->setTitleFontSize(60.0f);
    _contactListViewItem->setContentSize(Size(_contactListView->getContentSize().width - 20.0f, 200.0f));
    _contactListView->setItemModel(_contactListViewItem);
}

void ChatTestScene::createRightSideUI(cocos2d::ui::Layout* parent)
{
    const cocos2d::Size& visibleSize = parent->getContentSize();
    
    // Message entry area
    const float messageEntryPadding = 10.0f;
    
    // Send Button
    _sendButton = ui::Button::create("res/login/next_btn.png");
    _sendButton->retain();
    _sendButton->setAnchorPoint(Vec2(0, 1));
    _sendButton->addClickEventListener([this](Ref* sender) {
        const std::string& message = _messageEntryField->getString();
        cocos2d::log("Send Message: %s", message.c_str());
        _messageEntryField->setString("");
    });
    
    ui::LinearLayoutParameter* sendButtonLayout = ui::LinearLayoutParameter::create();
    sendButtonLayout->setGravity(ui::LinearLayoutParameter::LinearGravity::LEFT);
    sendButtonLayout->setMargin(ui::Margin(messageEntryPadding, messageEntryPadding, messageEntryPadding, messageEntryPadding));
    _sendButton->setLayoutParameter(sendButtonLayout);
    
    float messageEntryHeight = MIN(200.0f, _sendButton->getContentSize().height) + messageEntryPadding * 2;
    
    // TextField
    _messageEntryField = ui::TextField::create("New message...", Azoomee::FONT_REGULAR, 40.0f);
    _messageEntryField->retain();
    _messageEntryField->setAnchorPoint(Vec2(0, 1));
    _messageEntryField->ignoreContentAdaptWithSize(false);
    _messageEntryField->setTextHorizontalAlignment(TextHAlignment::LEFT);
    _messageEntryField->setTextVerticalAlignment(TextVAlignment::CENTER);
    _messageEntryField->setContentSize(Size(visibleSize.width - _sendButton->getContentSize().width - (messageEntryPadding * 4), messageEntryHeight));
    _messageEntryField->addEventListener(CC_CALLBACK_2(ChatTestScene::onTextFieldEvent, this));
    
    ui::LinearLayoutParameter* messageEntryFieldLayout = ui::LinearLayoutParameter::create();
    messageEntryFieldLayout->setGravity(ui::LinearLayoutParameter::LinearGravity::LEFT);
    messageEntryFieldLayout->setMargin(ui::Margin(messageEntryPadding, messageEntryPadding, messageEntryPadding, messageEntryPadding));
    _messageEntryField->setLayoutParameter(messageEntryFieldLayout);
    
    
    ui::Layout* messageEntryParent = new ui::Layout();
    messageEntryParent->setAnchorPoint(Vec2(0, 1));
    messageEntryParent->setLayoutType(ui::Layout::Type::HORIZONTAL);
    messageEntryParent->setContentSize(Size(visibleSize.width, messageEntryHeight));
    
    ui::LinearLayoutParameter* messageEntryParentLayout = ui::LinearLayoutParameter::create();
    messageEntryParentLayout->setGravity(ui::LinearLayoutParameter::LinearGravity::TOP);
    messageEntryParent->setLayoutParameter(messageEntryParentLayout);
    
    messageEntryParent->addChild(_messageEntryField);
    messageEntryParent->addChild(_sendButton);
    
    
    // Message list container
    _messageListView = ui::ListView::create();
    _messageListView->retain();
    _messageListView->setAnchorPoint(Vec2(0, 1));
    _messageListView->setDirection(ui::ScrollView::Direction::VERTICAL);
    _messageListView->setBounceEnabled(true);
    _messageListView->setGravity(ui::ListView::Gravity::CENTER_VERTICAL);
    _messageListView->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    _messageListView->setBackGroundColor(Color3B(75, 75, 75));
    _messageListView->setItemsMargin(20.0f);
    _messageListView->setContentSize(Size(visibleSize.width, visibleSize.height - messageEntryHeight));
    
    ui::LinearLayoutParameter* messageListLayout = ui::LinearLayoutParameter::create();
    messageListLayout->setGravity(ui::LinearLayoutParameter::LinearGravity::TOP);
    _contactListView->setLayoutParameter(messageListLayout);
    
    // Message list item
    _messageListViewItem = ui::Button::create("res/buttons/secondaryButton.png");
    _messageListViewItem->retain();
    _messageListViewItem->setAnchorPoint(Vec2(0, 1));
    _messageListViewItem->setCapInsets(Rect(103, 67, 1, 1));
    _messageListViewItem->setScale9Enabled(true);
    _messageListViewItem->setTitleFontName(Azoomee::FONT_REGULAR);
    _messageListViewItem->setTitleFontSize(60.0f);
    _messageListViewItem->setContentSize(Size(_messageListView->getContentSize().width / 2, 100.0f));
    
    parent->addChild(_messageListView);
    parent->addChild(messageEntryParent);
}

#pragma mark - TextField

void ChatTestScene::onTextFieldEvent(cocos2d::Ref* sender, cocos2d::ui::TextField::EventType type)
{
    switch(type)
    {
        case ui::TextField::EventType::INSERT_TEXT:
        {
            // TextField changed
            break;
        }
        case ui::TextField::EventType::ATTACH_WITH_IME:
        case ui::TextField::EventType::DETACH_WITH_IME:
        case ui::TextField::EventType::DELETE_BACKWARD:
        default:
        {
            // Nothing to do
            break;
        }
    }
}

#pragma mark - IMEDelegate

void ChatTestScene::keyboardWillShow(cocos2d::IMEKeyboardNotificationInfo& info)
{
    // We can get events even when scene is not shown
    if(!isVisible())
    {
        return;
    }
    
    // Animate resize the MessageList so the TextField stays in view
    float keyboardHeight = info.end.size.height;
    float fullHeight = _messageListView->getParent()->getContentSize().height;
    Size messageListSize = _messageListView->getContentSize();
    messageListSize.height = fullHeight - keyboardHeight - _messageEntryField->getContentSize().height;
    _messageListView->runAction(ResizeTo::create(info.duration, messageListSize));
    // Force the layout - it doesn't layout the UI correctly otherwise
    _messageListView->runAction(ActionFloat::create(info.duration, 0.0f, 1.0f, [this](float dt){
        _rightLayout->forceDoLayout();
    }));
}

void ChatTestScene::keyboardDidShow(cocos2d::IMEKeyboardNotificationInfo& info)
{
    // We can get events even when scene is not shown
    if(!isVisible())
    {
        return;
    }
    
    // Resize the MessageList so the TextField stays in view
    float keyboardHeight = info.end.size.height;
    float fullHeight = _messageListView->getParent()->getContentSize().height;
    Size messageListSize = _messageListView->getContentSize();
    messageListSize.height = fullHeight - keyboardHeight - _messageEntryField->getContentSize().height;
    _messageListView->setContentSize(messageListSize);
    _rightLayout->forceDoLayout();
}

void ChatTestScene::keyboardWillHide(cocos2d::IMEKeyboardNotificationInfo& info)
{
    // We can get events even when scene is not shown
    if(!isVisible())
    {
        return;
    }
    
    // Animate resize the MessageList back to normal
    float fullHeight = _messageListView->getParent()->getContentSize().height;
    Size messageListSize = _messageListView->getContentSize();
    messageListSize.height = fullHeight - _messageEntryField->getContentSize().height;
    _messageListView->runAction(ResizeTo::create(info.duration, messageListSize));
    // Force the layout - it doesn't layout the UI correctly otherwise
    _messageListView->runAction(ActionFloat::create(info.duration, 0.0f, 1.0f, [this](float dt){
        _rightLayout->forceDoLayout();
    }));
}

void ChatTestScene::keyboardDidHide(cocos2d::IMEKeyboardNotificationInfo& info)
{
    // We can get events even when scene is not shown
    if(!isVisible())
    {
        return;
    }
    
    // Resize the MessageList back to normal
    float fullHeight = _messageListView->getParent()->getContentSize().height;
    Size messageListSize = _messageListView->getContentSize();
    messageListSize.height = fullHeight - _messageEntryField->getContentSize().height;
    _messageListView->setContentSize(messageListSize);
    _rightLayout->forceDoLayout();
}

#pragma mark - ChatAPIObserver

void ChatTestScene::onChatAPIGetFriendList(const FriendList& friendList)
{
    // Update contact list
    _contactListView->removeAllItems();
    
    for(FriendRef friendData : friendList)
    {
        auto clonedItem = (ui::Button*) _contactListViewItem->clone();
        clonedItem->setTitleText(friendData->friendName());
        _contactListView->pushBackCustomItem(clonedItem);
    }
    
    ModalMessages::getInstance()->stopLoading();
}

void ChatTestScene::onChatAPIGetChatMessages(const MessageList& messageList)
{
    // Update message list
    _messageListView->removeAllItems();
    
    // For now display messages as a single line of text
    // For now, index Ids to names until we update this to use data structures
    std::map<std::string, std::string> names;
    ChildDataProvider* childData = ChildDataProvider::getInstance();
    names[childData->getLoggedInChildId()] = childData->getLoggedInChildName();
    const FriendList& friendsList = ChatAPI::getInstance()->getFriendList();
    for(auto friendData : friendsList)
    {
        names[friendData->friendId()] = friendData->friendName();
    }
    
    // The message list is in ascending timestamp order, so we can just add them in this order
    for(auto entry : messageList)
    {
        auto clonedItem = (ui::Button*) _messageListViewItem->clone();
        
        std::stringstream line;
        line << names[entry->senderId()] << ": " << entry->messageText();
        
        clonedItem->setTitleText(line.str());
        _messageListView->pushBackCustomItem(clonedItem);
    }
    
    ModalMessages::getInstance()->stopLoading();
}


NS_AZOOMEE_CHAT_END
