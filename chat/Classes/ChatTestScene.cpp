#include "ChatTestScene.h"
#include <AzoomeeCommon/Data/Child/ChildDataProvider.h>
#include <AzoomeeCommon/UI/ElectricDreamsTextStyles.h>
#include <AzoomeeCommon/UI/ModalMessages.h>
#include <AzoomeeCommon/UI/MessageBox.h>
#include "AppDelegate.h"
#include "ChildSelectorScene.h"
#include <sstream>

using namespace cocos2d;


const float kMessageListMargin = 20.0f;
const float kContactListMargin = 10.0f;


NS_AZOOMEE_CHAT_BEGIN

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
    _rootLayout->setAnchorPoint(Vec2::ZERO);
    _rootLayout->setContentSize(visibleSize);
    _rootLayout->setPosition(Point::ZERO);
    _rootLayout->setLayoutType(ui::Layout::Type::VERTICAL);
    _rootLayout->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    _rootLayout->setBackGroundColor(Color3B(50, 50, 50));
    addChild(_rootLayout);
    
    
    // Create title layout
    const float titleHeight = visibleSize.height * 0.1f;
    _titleLayout = ui::Layout::create();
    _titleLayout->setAnchorPoint(Vec2(0, 1));
    _titleLayout->setContentSize(Size(visibleSize.width, titleHeight));
    _titleLayout->setLayoutType(ui::Layout::Type::RELATIVE);
    _titleLayout->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    _titleLayout->setBackGroundColor(Color3B(120, 120, 120));
    _rootLayout->addChild(_titleLayout);
    
    // Create the content layout
    const cocos2d::Size contentSize = Size(visibleSize.width, visibleSize.height - titleHeight);
    _contentLayout = ui::Layout::create();
    _contentLayout->setAnchorPoint(Vec2::ZERO);
    _contentLayout->setContentSize(contentSize);
    _contentLayout->setPosition(Point::ZERO);
    _contentLayout->setLayoutType(ui::Layout::Type::RELATIVE);
    _contentLayout->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    _contentLayout->setBackGroundColor(Color3B(0, 0, 0));
    _rootLayout->addChild(_contentLayout);
    
    
    // Create the left and right side parent layouts to hold the elements
    _leftLayout = ui::Layout::create();
    _leftLayout->setAnchorPoint(Vec2(0, 1));
    _leftLayout->setLayoutType(ui::Layout::Type::VERTICAL);
    _leftLayout->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    _leftLayout->setBackGroundColor(Color3B(100, 100, 100));
    ui::RelativeLayoutParameter* leftLayoutParam = ui::RelativeLayoutParameter::create();
    leftLayoutParam->setAlign(ui::RelativeLayoutParameter::RelativeAlign::PARENT_TOP_LEFT);
    _leftLayout->setLayoutParameter(leftLayoutParam);
    _contentLayout->addChild(_leftLayout);
    
    _rightLayout = ui::Layout::create();
    _rightLayout->setAnchorPoint(Vec2(0, 1));
    _rightLayout->setLayoutType(ui::Layout::Type::VERTICAL);
    _rightLayout->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    _rightLayout->setBackGroundColor(Color3B(75, 75, 75));
    ui::RelativeLayoutParameter* rightLayoutParam = ui::RelativeLayoutParameter::create();
    rightLayoutParam->setAlign(ui::RelativeLayoutParameter::RelativeAlign::PARENT_TOP_RIGHT);
    _rightLayout->setLayoutParameter(rightLayoutParam);
    _contentLayout->addChild(_rightLayout);
    
    // Size elements
    float leftWidth = contentSize.width * 0.3f;
    _leftLayout->setContentSize(Size(leftWidth, contentSize.height));
    float rightWidth = contentSize.width - leftWidth;
    _rightLayout->setContentSize(Size(rightWidth, contentSize.height));
    
    // Now we can create the UI
    createTitleUI(_titleLayout);
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
    
    // Get friend list
    ChatAPI::getInstance()->requestFriendList();
    ModalMessages::getInstance()->startLoading();
    
    // Dev messages
    showUpdateNotesIfNeeded();
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

void ChatTestScene::showUpdateNotesIfNeeded()
{
    // Has the user seen these update notes yet?
    const char* const storageKey = "azoomee.chat.dev.update_notes";
    const std::string& version = Azoomee::Chat::Version;
    const std::string& readVersion = UserDefault::getInstance()->getStringForKey(storageKey);
    if( version != readVersion )
    {
        const std::string& messageBody = StringUtils::format("This is a very early version of the Azoomee Chat app.\n\nPlease be aware that all UI is placeholder. This build is mainly to test the API communication.\n\nv%s", version.c_str());
        UserDefault::getInstance()->setStringForKey(storageKey, version);
        MessageBox::createWith("Welcome", messageBody, "OK", nullptr);
    }
}

void ChatTestScene::onWindowChanged(cocos2d::EventCustom* event)
{
    const cocos2d::Size& visibleSize = getContentSize();
    cocos2d::log("onWindowChanged: %f, %f", visibleSize.width, visibleSize.height);
    
    // Size elements
    _rootLayout->setContentSize(visibleSize);
    _titleLayout->setContentSize(Size(visibleSize.width, visibleSize.height * 0.1f));
    _contentLayout->setContentSize(Size(visibleSize.width, visibleSize.height - _titleLayout->getContentSize().height));
    
    const Size& contentSize = _contentLayout->getContentSize();
    const float leftWidth = contentSize.width * 0.3f;
    _leftLayout->setContentSize(Size(leftWidth, contentSize.height));
    const float rightWidth = contentSize.width - leftWidth;
    _rightLayout->setContentSize(Size(rightWidth, contentSize.height));
    
//    _rootLayout->forceDoLayout();
//    _leftLayout->forceDoLayout();
//    _rightLayout->forceDoLayout();
    
    // TODO: List/Scrollviews need to resize
    // TODO: Animate this change?
}

#pragma mark - UI

void ChatTestScene::createTitleUI(cocos2d::ui::Layout* parent)
{
    // Back button
    _backButton = ui::Button::create("res/login/back_btn.png");
    _backButton->setAnchorPoint(Vec2(0, 1));
    _backButton->addClickEventListener([this](Ref* button){
        onBackButtonPressed();
    });
    ui::RelativeLayoutParameter* backButtonLayout = ui::RelativeLayoutParameter::create();
    backButtonLayout->setAlign(ui::RelativeLayoutParameter::RelativeAlign::PARENT_LEFT_CENTER_VERTICAL);
    backButtonLayout->setMargin(ui::Margin(20.0f, 0.0f, 0.0f, 0.0f));
    _backButton->setLayoutParameter(backButtonLayout);
    parent->addChild(_backButton);
    
    // Title label
    _titleLabel = ui::Text::create();
    _titleLabel->setFontName(Azoomee::FONT_REGULAR);
    _titleLabel->setFontSize(75.0f);
    _titleLabel->setColor(Color3B(255, 255, 255));
    _titleLabel->setString(ChildDataProvider::getInstance()->getLoggedInChildName());
    ui::RelativeLayoutParameter* titleLabelLayout = ui::RelativeLayoutParameter::create();
    titleLabelLayout->setAlign(ui::RelativeLayoutParameter::RelativeAlign::CENTER_IN_PARENT);
    titleLabelLayout->setMargin(ui::Margin(50.0f, 0.0f, 0.0f, 0.0f));
    _titleLabel->setLayoutParameter(titleLabelLayout);
    parent->addChild(_titleLabel);
    
    // Refresh button
    _refreshButton = ui::Button::create("res/buttons/buttonArea.png");
    _refreshButton->setScale9Enabled(true);
    _refreshButton->setCapInsets(Rect(60, 65, 60, 65));
    _refreshButton->setTitleText("refresh");
    _refreshButton->setTitleFontName(Azoomee::FONT_REGULAR);
    _refreshButton->setTitleFontSize(60.0f);
    _refreshButton->setAnchorPoint(Vec2(0, 1));
    _refreshButton->addClickEventListener([this](Ref* button){
        onRefreshButtonPressed();
    });
    ui::RelativeLayoutParameter* refreshButtonLayout = ui::RelativeLayoutParameter::create();
    refreshButtonLayout->setAlign(ui::RelativeLayoutParameter::RelativeAlign::PARENT_RIGHT_CENTER_VERTICAL);
    refreshButtonLayout->setMargin(ui::Margin(0.0f, 0.0f, 20.0f, 0.0f));
    _refreshButton->setLayoutParameter(refreshButtonLayout);
    parent->addChild(_refreshButton);
}

void ChatTestScene::createLeftSideUI(cocos2d::ui::Layout* parent)
{
    const cocos2d::Size& visibleSize = parent->getContentSize();
    
    // Contact list container
    _contactListView = ui::ListView::create();
    _contactListView->setAnchorPoint(Vec2(0, 1));
    _contactListView->setDirection(ui::ScrollView::Direction::VERTICAL);
    _contactListView->setBounceEnabled(true);
    _contactListView->setGravity(ui::ListView::Gravity::CENTER_VERTICAL);
    _contactListView->setContentSize(Size(visibleSize.width - (kContactListMargin * 2), visibleSize.height - (kContactListMargin * 2)));
    _contactListView->setItemsMargin(20.0f);
    _contactListView->addEventListener([this](Ref* sender, ui::ListView::EventType type) {
        if(type == ui::ListView::EventType::ON_SELECTED_ITEM_END)
        {
            selectFriend((int)_contactListView->getCurSelectedIndex());
        }
    });
    
    ui::LinearLayoutParameter* contactListLayout = ui::LinearLayoutParameter::create();
    contactListLayout->setGravity(ui::LinearLayoutParameter::LinearGravity::TOP);
    contactListLayout->setMargin(ui::Margin(kContactListMargin, kContactListMargin, kContactListMargin, kContactListMargin));
    _contactListView->setLayoutParameter(contactListLayout);
    parent->addChild(_contactListView);
    
    // Contact list item
    _contactListViewItem = ui::Button::create("res/modal/generic_button_slice_ready.png");
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
    _sendButton->setAnchorPoint(Vec2(0, 1));
    _sendButton->addClickEventListener([this](Ref* sender) {
        const std::string& message = _messageEntryField->getString();
        sendMessage(message);
    });
    
    ui::LinearLayoutParameter* sendButtonLayout = ui::LinearLayoutParameter::create();
    sendButtonLayout->setGravity(ui::LinearLayoutParameter::LinearGravity::LEFT);
    sendButtonLayout->setMargin(ui::Margin(messageEntryPadding, messageEntryPadding, messageEntryPadding, messageEntryPadding));
    _sendButton->setLayoutParameter(sendButtonLayout);
    
    float messageEntryHeight = MIN(200.0f, _sendButton->getContentSize().height) + messageEntryPadding * 2;
    
    // TextField
    _messageEntryField = ui::TextField::create("New message...", Azoomee::FONT_REGULAR, 70.0f);
    _messageEntryField->setAnchorPoint(Vec2(0, 1));
    _messageEntryField->setCursorEnabled(true);
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
    messageEntryParent->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    messageEntryParent->setBackGroundColor(Color3B(85, 85, 85));
    
    ui::LinearLayoutParameter* messageEntryParentLayout = ui::LinearLayoutParameter::create();
    messageEntryParentLayout->setGravity(ui::LinearLayoutParameter::LinearGravity::TOP);
    messageEntryParent->setLayoutParameter(messageEntryParentLayout);
    
    messageEntryParent->addChild(_messageEntryField);
    messageEntryParent->addChild(_sendButton);
    
    
    // Message list container
    _messageListView = ui::ListView::create();
    _messageListView->setAnchorPoint(Vec2(0, 1));
    _messageListView->setDirection(ui::ScrollView::Direction::VERTICAL);
    _messageListView->setBounceEnabled(true);
    _messageListView->setGravity(ui::ListView::Gravity::CENTER_VERTICAL);
    _messageListView->setItemsMargin(20.0f);
    _messageListView->setContentSize(Size(visibleSize.width - (kMessageListMargin * 2), visibleSize.height - messageEntryHeight - (kMessageListMargin * 2)));
    
    ui::LinearLayoutParameter* messageListLayout = ui::LinearLayoutParameter::create();
    messageListLayout->setGravity(ui::LinearLayoutParameter::LinearGravity::TOP);
    messageListLayout->setMargin(ui::Margin(kMessageListMargin, kMessageListMargin, kMessageListMargin, kMessageListMargin));
    _messageListView->setLayoutParameter(messageListLayout);
    
    parent->addChild(_messageListView);
    parent->addChild(messageEntryParent);
}

cocos2d::ui::Widget* ChatTestScene::createMessageMenuItem(const MessageRef& message)
{
    const std::string& senderId = message->senderId();
    bool isCurrentUser = (senderId == ChildDataProvider::getInstance()->getLoggedInChildId());
    const float itemPadding = 30.0f;
    
    // Create parent layout for the menu item.
    // This fills the whole width of the list
    ui::Layout* parentLayout = ui::Layout::create();
    parentLayout->setAnchorPoint(Vec2(0, 1));
    parentLayout->setLayoutType(ui::Layout::Type::RELATIVE);
    
    // Create inner layout
    // This contains the background image and label
    ui::Layout* innerLayout = ui::Layout::create();
    innerLayout->setLayoutType(ui::Layout::Type::RELATIVE);
    innerLayout->setBackGroundImage("res/buttons/secondaryButton.png");
    innerLayout->setBackGroundImageCapInsets(Rect(103, 67, 1, 1));
    innerLayout->setBackGroundImageScale9Enabled(true);
    ui::RelativeLayoutParameter* innerLayoutParams = ui::RelativeLayoutParameter::create();
    ui::RelativeLayoutParameter::RelativeAlign relativeAlign = (isCurrentUser) ? ui::RelativeLayoutParameter::RelativeAlign::PARENT_LEFT_CENTER_VERTICAL : ui::RelativeLayoutParameter::RelativeAlign::PARENT_RIGHT_CENTER_VERTICAL;
    innerLayoutParams->setAlign(relativeAlign);
    innerLayout->setLayoutParameter(innerLayoutParams);
    parentLayout->addChild(innerLayout);
    
    
    // Create label for the message
    ui::Text* label = ui::Text::create();
    label->setFontName(Azoomee::FONT_REGULAR);
    label->setFontSize(60.0f);
    label->setTextHorizontalAlignment(TextHAlignment::LEFT);
    label->ignoreContentAdaptWithSize(false);
    ui::RelativeLayoutParameter* labelLayoutParams = ui::RelativeLayoutParameter::create();
    labelLayoutParams->setAlign(ui::RelativeLayoutParameter::RelativeAlign::CENTER_IN_PARENT);
    label->setLayoutParameter(labelLayoutParams);
    innerLayout->addChild(label);
    
    
    // Set the text and calculate the element heights
    
    // Create the message string
    // Get the sender's name
    const std::string& senderName = ChatAPI::getInstance()->getProfileName(senderId);
    
    std::stringstream line;
    line << senderName << ": " << message->messageText();
    label->setString(line.str());
    
    // Wordwrap the label and get the height we need for the text
    const float itemWidth = _messageListView->getContentSize().width / 2;
    const float itemLabelWidth = itemWidth - (itemPadding * 2.0f);
    Label* labelRenderer = dynamic_cast<Label*>(label->getVirtualRenderer());
    labelRenderer->setDimensions(itemLabelWidth, 0);
    const float itemLabelHeight = labelRenderer->getContentSize().height;
    float itemHeight = itemLabelHeight + (itemPadding * 2.0f);
    
    // Resize the elements
    label->setContentSize(Size(itemLabelWidth, itemLabelHeight));
    innerLayout->setContentSize(Size(itemWidth, itemHeight));
    parentLayout->setContentSize(Size(_messageListView->getContentSize().width, itemHeight));
    
    return parentLayout;
}

#pragma mark - Interactions

void ChatTestScene::selectFriend(int index)
{
    ChatAPI* chatAPI = ChatAPI::getInstance();
    const FriendList& friendList = chatAPI->getFriendList();
    selectFriend(friendList[index]);
}

void ChatTestScene::selectFriend(const FriendRef& friendObj)
{
    // Clear list if friend has changed
    if(_selectedFriend != friendObj)
    {
        _messageListView->removeAllChildren();
    }
    _selectedFriend = friendObj;
    
    // Update menu
    const Vector<ui::Widget*>& items = _contactListView->getItems();
    ChatAPI* chatAPI = ChatAPI::getInstance();
    const FriendList& friendList = chatAPI->getFriendList();
    for(int i = 0; i < items.size(); ++i)
    {
        ui::Widget* btn = items.at(i);
        btn->setOpacity((friendList[i] == _selectedFriend) ? 255 : 150);
    }
    
    // Request messages for this friend
    chatAPI->requestMessageHistory(_selectedFriend);
    ModalMessages::getInstance()->stopLoading();
    ModalMessages::getInstance()->startLoading();
}

void ChatTestScene::onBackButtonPressed()
{
    // Logout child
    AuthAPI::getInstance()->logoutChild();
    // Back to profile select
    auto childSelectScene = ChildSelectorScene::create();
    Director::getInstance()->replaceScene(childSelectScene);
}

void ChatTestScene::onRefreshButtonPressed()
{
    selectFriend(_selectedFriend);
}

void ChatTestScene::sendMessage(const std::string& message)
{
    // TODO: Trim message
    if(message.length() > 0)
    {
        cocos2d::log("Send Message: %s", message.c_str());
        ChatAPI::getInstance()->sendMessage(_selectedFriend, message);
    }
    
    // Clear the message field
    _messageEntryField->setString("");
    
    // Dismiss the keyboard
    ui::UICCTextField* textFieldRenderer = dynamic_cast<ui::UICCTextField*>(_messageEntryField->getVirtualRenderer());
    textFieldRenderer->closeIME();
}

#pragma mark - TextField

void ChatTestScene::onTextFieldEvent(cocos2d::Ref* sender, cocos2d::ui::TextField::EventType type)
{
    switch(type)
    {
        case ui::TextField::EventType::DETACH_WITH_IME:
        {
            // Keyboard closed
            // For now we'll assume we should send the message when keyboard dismissed
            const std::string& message = _messageEntryField->getString();
            if(message.length() > 0)
            {
                sendMessage(message);
            }
            break;
        }
        case ui::TextField::EventType::INSERT_TEXT:
        {
            // TextField changed
            break;
        }
        case ui::TextField::EventType::ATTACH_WITH_IME:
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
    messageListSize.height = fullHeight - (kMessageListMargin * 2) - keyboardHeight - _messageEntryField->getContentSize().height;
    _messageListView->runAction(ResizeTo::create(info.duration, messageListSize));
    // Force the layout - it doesn't layout the UI correctly otherwise
    _messageListView->runAction(ActionFloat::create(info.duration, 0.0f, 1.0f, [this](float dt){
        _rightLayout->forceDoLayout();
        // Keep forcing to the bottom as we resize
        _messageListView->jumpToBottom();
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
    messageListSize.height = fullHeight - (kMessageListMargin * 2) - keyboardHeight - _messageEntryField->getContentSize().height;
    _messageListView->setContentSize(messageListSize);
    _rightLayout->forceDoLayout();
    
    // Make sure bottom is visible
    _messageListView->jumpToBottom();
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
    messageListSize.height = fullHeight - (kMessageListMargin * 2) - _messageEntryField->getContentSize().height;
    _messageListView->runAction(ResizeTo::create(info.duration, messageListSize));
    // Force the layout - it doesn't layout the UI correctly otherwise
    _messageListView->runAction(ActionFloat::create(info.duration, 0.0f, 1.0f, [this](float dt){
        _rightLayout->forceDoLayout();
        // Keep forcing to the bottom as we resize
        _messageListView->jumpToBottom();
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
    messageListSize.height = fullHeight - (kMessageListMargin * 2) - _messageEntryField->getContentSize().height;
    _messageListView->setContentSize(messageListSize);
    _rightLayout->forceDoLayout();
    
    // Make sure bottom is visible
    _messageListView->jumpToBottom();
}

#pragma mark - ChatAPIObserver

void ChatTestScene::onChatAPIGetFriendList(const FriendList& friendList)
{
    // Update contact list
    _contactListView->removeAllItems();
    
    for(auto friendData : friendList)
    {
        auto clonedItem = (ui::Button*) _contactListViewItem->clone();
        clonedItem->setTitleText(friendData->friendName());
        _contactListView->pushBackCustomItem(clonedItem);
    }
    
    // Auto select first friend
    selectFriend(0);
}

void ChatTestScene::onChatAPIGetChatMessages(const MessageList& messageList)
{
    // Update message list
    _messageListView->removeAllItems();
    
    // The message list is in ascending timestamp order, so we can just add them in this order
    for(auto entry : messageList)
    {
        ui::Widget* menuItem = createMessageMenuItem(entry);
        _messageListView->pushBackCustomItem(menuItem);
    }
    
    _messageListView->scrollToBottom(0, false);
    
    ModalMessages::getInstance()->stopLoading();
}

void ChatTestScene::onChatAPISendMessage(const MessageRef& sentMessage)
{
    // For now, just assume we can add the new message to the end
    // This is only a problem if someone sent a message before this which we haven't got yet
    // But we'll worry about that later
    ui::Widget* menuItem = createMessageMenuItem(sentMessage);
    _messageListView->pushBackCustomItem(menuItem);
    _messageListView->scrollToBottom(0.5f, true);
}


NS_AZOOMEE_CHAT_END
