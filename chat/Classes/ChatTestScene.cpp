#include "ChatTestScene.h"
#include <AzoomeeCommon/Data/Child/ChildDataProvider.h>
#include <AzoomeeCommon/UI/ElectricDreamsTextStyles.h>
#include <AzoomeeCommon/UI/ModalMessages.h>
#include <AzoomeeCommon/Strings.h>
#include <AzoomeeCommon/Utils/CocosSharedPtr.h>
#include "AppDelegate.h"
#include "ChildSelectorScene.h"
#include "OrientationFunctions.h"
#include <sstream>

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include <cocos/platform/android/jni/JniHelper.h>
#endif

using namespace cocos2d;


const float kContactItemMargin = 20.0f;
const float kContactItemHeight = 200.0f;
const float kContactListWidthRatio = 0.3f;
const float kMessageItemMargin = 20.0f;
const float kMessageListWidthRatio = 1.0f - kContactListWidthRatio;

// Interval to do an auto get call
// Temp feature until Pusher is implemented
const float kAutoGetTimeInterval = 5.0f;

// For dev, some messages that display in a MessageBox
const char* const kUpdateLastSeenStorageKey = "azoomee.chat.dev.update_notes";
const char* const kUpdateWelcomeTitle = "Welcome";
const char* const kUpdateWhatsNewTitle = "What's new";


NS_AZOOMEE_CHAT_BEGIN

bool ChatTestScene::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    // Create the root layout which fills the whole screen
    _rootLayout = ui::Layout::create();
    _rootLayout->setAnchorPoint(Vec2::ZERO);
    _rootLayout->setLayoutType(ui::Layout::Type::VERTICAL);
    _rootLayout->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    _rootLayout->setBackGroundColor(Color3B(50, 50, 50));
    _rootLayout->setSizePercent(Vec2::ONE);
    _rootLayout->setSizeType(ui::Widget::SizeType::PERCENT);
    addChild(_rootLayout);
    
    
    // Create title layout
    _titleLayout = ui::Layout::create();
    _titleLayout->setAnchorPoint(Vec2(0, 1));
    _titleLayout->setSizePercent(Vec2(1.0f, 0.1f));
    _titleLayout->setSizeType(ui::Widget::SizeType::PERCENT);
    _titleLayout->setLayoutType(ui::Layout::Type::RELATIVE);
    _titleLayout->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    _titleLayout->setBackGroundColor(Color3B(120, 120, 120));
    _rootLayout->addChild(_titleLayout);
    
    // Create the content layout
    _contentLayout = ui::Layout::create();
    _contentLayout->setAnchorPoint(Vec2::ZERO);
    _contentLayout->setSizePercent(Vec2(1.0f, 0.9f));
    _contentLayout->setSizeType(ui::Widget::SizeType::PERCENT);
    _contentLayout->setLayoutType(ui::Layout::Type::RELATIVE);
    _contentLayout->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    _contentLayout->setBackGroundColor(Color3B(0, 0, 0));
    _rootLayout->addChild(_contentLayout);
    
    
    // Create the left and right side parent layouts to hold the elements
    _leftLayout = ui::Layout::create();
    _leftLayout->setAnchorPoint(Vec2(0, 1));
    _leftLayout->setSizePercent(Vec2(kContactListWidthRatio, 1.0f));
    _leftLayout->setSizeType(ui::Widget::SizeType::PERCENT);
    _leftLayout->setLayoutType(ui::Layout::Type::VERTICAL);
    _leftLayout->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    _leftLayout->setBackGroundColor(Color3B(100, 100, 100));
    ui::RelativeLayoutParameter* leftLayoutParam = ui::RelativeLayoutParameter::create();
    leftLayoutParam->setAlign(ui::RelativeLayoutParameter::RelativeAlign::PARENT_TOP_LEFT);
    _leftLayout->setLayoutParameter(leftLayoutParam);
    _contentLayout->addChild(_leftLayout);
    
    _rightLayout = ui::Layout::create();
    _rightLayout->setAnchorPoint(Vec2(0, 1));
    _rightLayout->setSizePercent(Vec2(kMessageListWidthRatio, 1.0f));
    _rightLayout->setSizeType(ui::Widget::SizeType::PERCENT);
    _rightLayout->setLayoutType(ui::Layout::Type::VERTICAL);
    _rightLayout->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    _rightLayout->setBackGroundColor(Color3B(75, 75, 75));
    ui::RelativeLayoutParameter* rightLayoutParam = ui::RelativeLayoutParameter::create();
    rightLayoutParam->setAlign(ui::RelativeLayoutParameter::RelativeAlign::PARENT_TOP_RIGHT);
    _rightLayout->setLayoutParameter(rightLayoutParam);
    _contentLayout->addChild(_rightLayout);
    
    // Now we can create the UI
    createTitleUI(_titleLayout);
    createLeftSideUI(_leftLayout);
    createRightSideUI(_rightLayout);
  
    return true;
}

void ChatTestScene::onEnter()
{
    Super::onEnter();
    
    // Register for API events
    ChatAPI::getInstance()->registerObserver(this);
    
    // Get friend list
    ChatAPI::getInstance()->requestFriendList();
    ModalMessages::getInstance()->startLoading();
    
    // Dev messages
    showUpdateNotesIfNeeded();
    
    // Get update calls
    scheduleUpdate();
}

void ChatTestScene::onExit()
{
    Super::onExit();
    
    _keyboardVisible = false;
    
    // Unregister on chat API events
    ChatAPI::getInstance()->removeObserver(this);
    
    // Make sure keyboard dismissed before we exit screen
    _messageEntryField->didNotSelectSelf();
    ui::UICCTextField* textFieldRenderer = dynamic_cast<ui::UICCTextField*>(_messageEntryField->getVirtualRenderer());
    textFieldRenderer->closeIME();
    
    // Stop update calls
    unscheduleUpdate();
}

void ChatTestScene::update(float dt)
{
    if(kAutoGetTimeInterval > 0 && _timeTillGet > -1 && _selectedFriend)
    {
        _timeTillGet -= dt;
        if(_timeTillGet <= 0)
        {
            // Wait until we get results before restarting the timer
            _timeTillGet = -1.0f;
            
            // Make the call
            ChatAPI* chatAPI = ChatAPI::getInstance();
            chatAPI->requestMessageHistory(_selectedFriend);
        }
    }
}

void ChatTestScene::showUpdateNotesIfNeeded()
{
    // Has the user seen these update notes yet?
    const std::string& version = Azoomee::Chat::Version;
    const std::string& readVersion = UserDefault::getInstance()->getStringForKey(kUpdateLastSeenStorageKey);
    if(version != readVersion)
    {
        const std::string& messageBody = StringUtils::format("This is a very early version of the Azoomee Chat app."
                                                             "\n\nPlease be aware that all UI is placeholder. This build is mainly to test the API communication."
                                                             "\n\nv%s", version.c_str());
        MessageBox::createWith(kUpdateWelcomeTitle, messageBody, "OK", this);
    }
}

#pragma mark - Size changes

void ChatTestScene::onSizeChanged()
{
    Super::onSizeChanged();
    
    const cocos2d::Size& contentSize = getContentSize();
    cocos2d::log("onSizeChanged: %f, %f", contentSize.width, contentSize.height);
    
    // Resize some elements which are using absolute sizing
    
    // The elements for the contact and message list can't use % because they sit inside an inner
    // layer (part of the ScrollView) which changes size to match the content.
    Size contactListViewItemSize(_contactListView->getContentSize().width - (kContactItemMargin * 2), kContactItemHeight);
    _contactListViewItem->setContentSize(contactListViewItemSize);
    const cocos2d::Vector<ui::Widget*>& contactItems = _contactListView->getItems();
    for(ui::Widget* item : contactItems)
    {
        item->setContentSize(contactListViewItemSize);
    }
    
    float messageListViewItemWidth = _messageListView->getContentSize().width;
    const cocos2d::Vector<ui::Widget*>& messageItems = _messageListView->getItems();
    for(ui::Widget* item : messageItems)
    {
        Size itemSize = item->getContentSize();
        itemSize.width = messageListViewItemWidth;
        item->setContentSize(itemSize);
    }
    
    
    // Force message list to bottom
    // TODO: Fix this so instead we scroll so that exactly the same items are visible as they were pre re-size
    _messageListView->jumpToBottom();
}

void ChatTestScene::resizeUIForKeyboard(float keyboardHeight, float duration)
{
    cocos2d::log("ChatTestScene::resizeUIForKeyboard: %f", keyboardHeight);
    _keyboardVisible = (keyboardHeight > 0);
    
    // Take into account screen cropping
    if(keyboardHeight > 0)
    {
        keyboardHeight -= _rootLayout->getPosition().y;
    }
    
    // Animate resize the MessageList so the send message TextField stays in view
    
    // Get normal height of the view when keyboard is not shown
    const float messageEntryAreaHeight = _messageEntryField->getParent()->getContentSize().height;
    const float parentHeight = _messageListView->getParent()->getContentSize().height;
    const float normalAvailableMessageListHeight = parentHeight - messageEntryAreaHeight;
    
    // Calculate what height we need, as a %
    const float wantHeight = normalAvailableMessageListHeight - keyboardHeight;
    const float wantHeightPercent = wantHeight / parentHeight;
    
    const Vec2& messageListSizePercent = _messageListView->getSizePercent();
    
    // Animate or not?
    if(duration > 0)
    {
        // Animate
        _messageListView->runAction(ActionFloat::create(duration, messageListSizePercent.y, wantHeightPercent, [=](float y) {
            // Resize the message list
            _messageListView->setSizePercent(Vec2(messageListSizePercent.x, y));
            // Re-layout to get the message box to move up with the list view
            _rightLayout->forceDoLayout();
            // Keep forcing to the bottom as we resize
            _messageListView->jumpToBottom();
        }));
    }
    else
    {
        // Immediate
        // Resize the message list
        _messageListView->setSizePercent(Vec2(messageListSizePercent.x, wantHeightPercent));
        // Re-layout to get the message box to move up with the list view
        _rightLayout->forceDoLayout();
        // Keep forcing to the bottom as we resize
        _messageListView->jumpToBottom();
    }
}

#pragma mark - UI creation

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
    _titleLabel->setFontName(Style::Font::Regular);
    _titleLabel->setFontSize(75.0f);
    _titleLabel->setTextColor(Color4B::WHITE);
    _titleLabel->setString(ChildDataProvider::getInstance()->getLoggedInChildName());
    ui::RelativeLayoutParameter* titleLabelLayout = ui::RelativeLayoutParameter::create();
    titleLabelLayout->setAlign(ui::RelativeLayoutParameter::RelativeAlign::CENTER_IN_PARENT);
    titleLabelLayout->setMargin(ui::Margin(50.0f, 0.0f, 0.0f, 0.0f));
    _titleLabel->setLayoutParameter(titleLabelLayout);
    parent->addChild(_titleLabel);
}

void ChatTestScene::createLeftSideUI(cocos2d::ui::Layout* parent)
{
    // Contact list container
    _contactListView = ui::ListView::create();
    _contactListView->setAnchorPoint(Vec2(0, 1));
    _contactListView->setSizePercent(Vec2(1.0f, 1.0f));
    _contactListView->setSizeType(ui::Widget::SizeType::PERCENT);
    _contactListView->setDirection(ui::ScrollView::Direction::VERTICAL);
    _contactListView->setBounceEnabled(true);
    _contactListView->setGravity(ui::ListView::Gravity::CENTER_HORIZONTAL);
    _contactListView->setItemsMargin(kContactItemMargin);
    _contactListView->addEventListener([this](Ref* sender, ui::ListView::EventType type) {
        if(type == ui::ListView::EventType::ON_SELECTED_ITEM_END)
        {
            selectFriend((int)_contactListView->getCurSelectedIndex());
        }
    });
    
    ui::LinearLayoutParameter* contactListLayout = ui::LinearLayoutParameter::create();
    contactListLayout->setGravity(ui::LinearLayoutParameter::LinearGravity::TOP);
    _contactListView->setLayoutParameter(contactListLayout);
    parent->addChild(_contactListView);
    
    // Contact list item
    _contactListViewItem = ui::Button::create("res/modal/generic_button_slice_ready.png");
    _contactListViewItem->setAnchorPoint(Vec2(0, 1));
    _contactListViewItem->setCapInsets(Rect(98, 98, 1, 1));
    _contactListViewItem->setScale9Enabled(true);
    _contactListViewItem->setContentSize(Size(0, kContactItemHeight));
    _contactListViewItem->setTitleFontName(Style::Font::Regular);
    _contactListViewItem->setTitleFontSize(60.0f);
    _contactListView->setItemModel(_contactListViewItem);
}

void ChatTestScene::createRightSideUI(cocos2d::ui::Layout* parent)
{
    // Send Button
    _sendButton = ui::Button::create("res/login/next_btn.png");
    _sendButton->setAnchorPoint(Vec2(0, 1));
    _sendButton->setSizePercent(Vec2(1.0f, 1.0f));
    _sendButton->setSizeType(ui::Widget::SizeType::PERCENT);
    _sendButton->addClickEventListener([this](Ref* sender) {
        const std::string& message = _messageEntryField->getString();
        sendMessage(message);
    });
    
    ui::RelativeLayoutParameter* sendButtonLayout = ui::RelativeLayoutParameter::create();
    sendButtonLayout->setAlign(ui::RelativeLayoutParameter::RelativeAlign::PARENT_RIGHT_CENTER_VERTICAL);
    sendButtonLayout->setMargin(ui::Margin(0, 0, 10.0f, 0));
    _sendButton->setLayoutParameter(sendButtonLayout);
    
    // TextField
    _messageEntryField = ui::TextField::create("New message...", Style::Font::Regular, 70.0f);
    _messageEntryField->setAnchorPoint(Vec2(0, 1));
    _messageEntryField->setCursorEnabled(true);
    _messageEntryField->ignoreContentAdaptWithSize(false);
    _messageEntryField->setTextHorizontalAlignment(TextHAlignment::LEFT);
    _messageEntryField->setTextVerticalAlignment(TextVAlignment::CENTER);
    _messageEntryField->setSizePercent(Vec2(0.8f, 1.0f));
    _messageEntryField->setSizeType(ui::Widget::SizeType::PERCENT);
    _messageEntryField->addEventListener(CC_CALLBACK_2(ChatTestScene::onTextFieldEvent, this));
    ui::RelativeLayoutParameter* messageEntryFieldLayout = ui::RelativeLayoutParameter::create();
    messageEntryFieldLayout->setAlign(ui::RelativeLayoutParameter::RelativeAlign::PARENT_LEFT_CENTER_VERTICAL);
    messageEntryFieldLayout->setMargin(ui::Margin(10.0f, 0, 0, 0));
    _messageEntryField->setLayoutParameter(messageEntryFieldLayout);
    
    
    ui::Layout* messageEntryParent = new ui::Layout();
    messageEntryParent->setAnchorPoint(Vec2(0, 1));
    messageEntryParent->setLayoutType(ui::Layout::Type::RELATIVE);
    messageEntryParent->setSizePercent(Vec2(1.0f, 0.1f));
    messageEntryParent->setSizeType(ui::Widget::SizeType::PERCENT);
    messageEntryParent->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    messageEntryParent->setBackGroundColor(Color3B(75, 75, 75));
    ui::LinearLayoutParameter* messageEntryParentLayout = ui::LinearLayoutParameter::create();
    messageEntryParentLayout->setGravity(ui::LinearLayoutParameter::LinearGravity::TOP);
    messageEntryParent->setLayoutParameter(messageEntryParentLayout);
    
    messageEntryParent->addChild(_messageEntryField);
    messageEntryParent->addChild(_sendButton);
    
    
    // Message list container
    _messageListView = ui::ListView::create();
    _messageListView->setAnchorPoint(Vec2(0, 1));
    _messageListView->setSizePercent(Vec2(1.0f, 0.9f));
    _messageListView->setSizeType(ui::Widget::SizeType::PERCENT);
    _messageListView->setDirection(ui::ScrollView::Direction::VERTICAL);
    _messageListView->setBounceEnabled(true);
    _messageListView->setGravity(ui::ListView::Gravity::CENTER_HORIZONTAL);
    _messageListView->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    _messageListView->setBackGroundColor(Color3B(85, 85, 85));
    ui::LinearLayoutParameter* messageListLayout = ui::LinearLayoutParameter::create();
    messageListLayout->setGravity(ui::LinearLayoutParameter::LinearGravity::TOP);
    _messageListView->setLayoutParameter(messageListLayout);
    
    parent->addChild(_messageListView);
    parent->addChild(messageEntryParent);
}

cocos2d::ui::Widget* ChatTestScene::createMessageMenuItem(const MessageRef& message)
{
    // Create parent layout for the menu item.
    // This always fills the whole width of the list
    ui::Layout* parentLayout = ui::Layout::create();
    parentLayout->setAnchorPoint(Vec2(0, 1));
    parentLayout->setLayoutType(ui::Layout::Type::RELATIVE);
    ui::LinearLayoutParameter* parentLayoutParams = ui::LinearLayoutParameter::create();
    parentLayoutParams->setGravity(ui::LinearLayoutParameter::LinearGravity::CENTER_HORIZONTAL);
    parentLayout->setLayoutParameter(parentLayoutParams);
    
    // Create inner layout
    // This contains the background image and label
    ui::Layout* innerLayout = ui::Layout::create();
    innerLayout->setName("innerLayout");
    innerLayout->setLayoutType(ui::Layout::Type::RELATIVE);
    innerLayout->setBackGroundImage("res/buttons/secondaryButton.png");
    innerLayout->setBackGroundImageCapInsets(Rect(103, 67, 1, 1));
    innerLayout->setBackGroundImageScale9Enabled(true);
    ui::RelativeLayoutParameter* innerLayoutParams = ui::RelativeLayoutParameter::create();
    innerLayoutParams->setAlign(ui::RelativeLayoutParameter::RelativeAlign::PARENT_LEFT_CENTER_VERTICAL);
    innerLayoutParams->setMargin(ui::Margin(0, kMessageItemMargin, 0, kMessageItemMargin));
    innerLayout->setLayoutParameter(innerLayoutParams);
    parentLayout->addChild(innerLayout);
    
    
    // Create label for the message
    ui::Text* label = ui::Text::create();
    label->setName("label");
    label->setFontName(Style::Font::Regular);
    label->setFontSize(60.0f);
    label->setTextHorizontalAlignment(TextHAlignment::LEFT);
    label->ignoreContentAdaptWithSize(false); // means fixed size (don't resize with text)
    ui::RelativeLayoutParameter* labelLayoutParams = ui::RelativeLayoutParameter::create();
    labelLayoutParams->setAlign(ui::RelativeLayoutParameter::RelativeAlign::CENTER_IN_PARENT);
    label->setLayoutParameter(labelLayoutParams);
    innerLayout->addChild(label);
    
    
    // Set the content
    updateMessageMenuItem(parentLayout, message);
    
    return parentLayout;
}

void ChatTestScene::updateMessageMenuItem(cocos2d::ui::Widget* item, const MessageRef& message)
{
    // Set message as userData on parentlayout
    CocosSharedPtr<Message>* messageCocosPtr = new CocosSharedPtr<Message>(message);
    item->setUserObject(messageCocosPtr);
    
    // Grab info from the message
    const std::string& senderId = message->senderId();
    const bool isCurrentUser = (senderId == ChildDataProvider::getInstance()->getLoggedInChildId());
    const std::string& senderName = ChatAPI::getInstance()->getProfileName(senderId);
    
    // Get the inner elements
    ui::Layout* innerLayout = (ui::Layout*) item->getChildByName("innerLayout");
    ui::Text* label = (ui::Text*) innerLayout->getChildByName("label");
    
    // Set the alignment of the innerLayout
    ui::RelativeLayoutParameter* innerLayoutParams = (ui::RelativeLayoutParameter*) innerLayout->getLayoutParameter();
    ui::RelativeLayoutParameter::RelativeAlign relativeAlign = (isCurrentUser) ? ui::RelativeLayoutParameter::RelativeAlign::PARENT_RIGHT_CENTER_VERTICAL : ui::RelativeLayoutParameter::RelativeAlign::PARENT_LEFT_CENTER_VERTICAL;
    innerLayoutParams->setAlign(relativeAlign);
    const float leftMargin = (relativeAlign == ui::RelativeLayoutParameter::RelativeAlign::PARENT_LEFT_CENTER_VERTICAL) ? kMessageItemMargin : 0;
    const float rightMargin = (relativeAlign == ui::RelativeLayoutParameter::RelativeAlign::PARENT_RIGHT_CENTER_VERTICAL) ? kMessageItemMargin : 0;
    innerLayoutParams->setMargin(ui::Margin(leftMargin, kMessageItemMargin, rightMargin, kMessageItemMargin));
    
    // Create the message body string
    std::stringstream line;
    line << senderName << ": " << message->messageText();
    label->setString(line.str());
    
    // Wordwrap the label and get the height we need for the text
    // Note we always use the landscape resolution width for item calculation
    // The items will overlap slightly on portrait which is what we want
    const float labelPadding = 30.0f;
    Label* labelRenderer = dynamic_cast<Label*>(label->getVirtualRenderer());
    
    // Calc and set max width we want
    auto glview = Director::getInstance()->getOpenGLView();
    const Size& designRes = glview->getDesignResolutionSize() * kMessageListWidthRatio;
    const float itemWidth = (designRes.width > designRes.height ? designRes.width : designRes.height) / 2;
    const float itemLabelWidth = itemWidth - (labelPadding * 2.0f);
    labelRenderer->setDimensions(itemLabelWidth, 0);
    
    // Now get the height of the label
    const float itemLabelHeight = labelRenderer->getContentSize().height;
    const float itemHeight = itemLabelHeight + (labelPadding * 2.0f);
    
    // Resize the elements
    label->setContentSize(Size(itemLabelWidth, itemLabelHeight));
    innerLayout->setContentSize(Size(itemWidth, itemHeight));
    item->setContentSize(Size(_messageListView->getContentSize().width, itemHeight + (kMessageItemMargin * 2)));
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
//    // Quick test of forcing orientation while a scene is open
//    static bool toggle = true;
//    setOrientationToPortrait(toggle);
//    toggle = !toggle;
    selectFriend(_selectedFriend);
}

void ChatTestScene::sendMessage(const std::string& message)
{
    const std::string& trimmedMessage = Azoomee::trim(message);
    if(trimmedMessage.length() > 0)
    {
        cocos2d::log("Send Message: %s", trimmedMessage.c_str());
        ChatAPI::getInstance()->sendMessage(_selectedFriend, trimmedMessage);
    }
    
    // Clear the message field
    _messageEntryField->setString("");
    
    // Dismiss the keyboard
    _messageEntryField->didNotSelectSelf();
    ui::UICCTextField* textFieldRenderer = dynamic_cast<ui::UICCTextField*>(_messageEntryField->getVirtualRenderer());
    textFieldRenderer->closeIME();
}

#pragma mark - MessageBoxDelegate

void ChatTestScene::MessageBoxButtonPressed(std::string messageBoxTitle, std::string buttonTitle)
{
    if(messageBoxTitle == kUpdateWelcomeTitle)
    {
        // Display what's new
        const std::string& messageBody = StringUtils::format("v0.0.3:\n"
                                                             " - UI fits correctly in all screen sizes, including iPhones.\n"
                                                             " - UI is much more responsive and should generally look correct in all orientations.\n"
                                                             " - Moderated messages no longer crash the app and now display appropriately in the message list.\n"
                                                             " - Messages are retrieved automatically every 5 seconds as a temp feature until Pusher is integrated.\n"
                                                             " - Various Android fixes, including ensuring the send message box slides up when entering text.");
        MessageBox* messageBox = MessageBox::createWith(kUpdateWhatsNewTitle, messageBody, "OK", this);
        messageBox->setBodyHAlignment(TextHAlignment::LEFT);
    }
    else
    {
        // Done displaying messages, don't display again for this version
        const std::string& version = Azoomee::Chat::Version;
        UserDefault::getInstance()->setStringForKey(kUpdateLastSeenStorageKey, version);
    }
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
            
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            // On Android we must deal with the faff of getting the keyboard height here, because
            // IMEKeyboardNotificationInfo methods do not get called (sigh)
            resizeUIForKeyboard(0, 0.25f);
#endif
            break;
        }
        case ui::TextField::EventType::INSERT_TEXT:
        {
            // TextField changed
            break;
        }
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
    
    resizeUIForKeyboard(info.end.size.height, info.duration);
}

void ChatTestScene::keyboardDidShow(cocos2d::IMEKeyboardNotificationInfo& info)
{
    // We can get events even when scene is not shown
    if(!isVisible())
    {
        return;
    }
    
    resizeUIForKeyboard(info.end.size.height, 0);
}

void ChatTestScene::keyboardWillHide(cocos2d::IMEKeyboardNotificationInfo& info)
{
    // We can get events even when scene is not shown
    if(!isVisible())
    {
        return;
    }
    
    resizeUIForKeyboard(0, info.duration);
}

void ChatTestScene::keyboardDidHide(cocos2d::IMEKeyboardNotificationInfo& info)
{
    // We can get events even when scene is not shown
    if(!isVisible())
    {
        return;
    }
    
    resizeUIForKeyboard(0, 0);
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
    // Check the messages we've recieved are for the currently selected friend
    // It could be we navigated to a diff friend before this response
    if(messageList.size() > 0)
    {
        const MessageRef& firstMessage = messageList[0];
        if(firstMessage->recipientId() != _selectedFriend->friendId() && firstMessage->senderId() != _selectedFriend->friendId())
        {
            // Not to or from the current friend
            return;
        }
    }
    
    // If messages are zero, we can just remove
    if(messageList.size() == 0)
    {
        _messageListView->removeAllItems();
    }
    else
    {
        // Update message list
        // Do this inline to avoid a flicker of the UI
        // We just overwrite the content of all UI items here
        
        const cocos2d::Vector<ui::Widget*> items = _messageListView->getItems();
        for(int i = 0; i < items.size() || i < messageList.size(); ++i)
        {
            ui::Widget* item = (i < items.size()) ? items.at(i) : nullptr;
            const MessageRef& message = (i < messageList.size()) ? messageList[i] : nullptr;
            
            if(item && message)
            {
                updateMessageMenuItem(item, message);
            }
            else if(item)
            {
                // If we have UI elements but ran out of messages, we can stop now and delete any remaining UI elements
                break;
            }
            else if(message)
            {
                // Ran out of UI elements, so we need to add a new one now
                item = createMessageMenuItem(message);
                _messageListView->pushBackCustomItem(item);
            }
            else
            {
                // We shouldn't get here, but just incase
                break;
            }
        }
        
        // Trim message list
        int64_t numToDelete = _messageListView->getItems().size() - messageList.size();
        while(numToDelete > 0)
        {
            _messageListView->removeLastItem();
            --numToDelete;
        }
        
        // Scroll to bottom if we have more items than before
        if(_messageListView->getItems().size() > items.size())
        {
            _messageListView->scrollToBottom(0, false);
        }
    }
    
    ModalMessages::getInstance()->stopLoading();
    
    // Trigger auto get again
    _timeTillGet = kAutoGetTimeInterval;
}

void ChatTestScene::onChatAPISendMessage(const MessageRef& sentMessage)
{
    // For now, just assume we can add the new message to the end
    // This is only a problem if someone sent a message before this which we haven't got yet
    // But we'll worry about that later
    ui::Widget* menuItem = createMessageMenuItem(sentMessage);
    _messageListView->pushBackCustomItem(menuItem);
    _messageListView->scrollToBottom(0.5f, true);
    
    // Auto get new messages until we have a live feed from PUSHER
    ChatAPI* chatAPI = ChatAPI::getInstance();
    chatAPI->requestMessageHistory(_selectedFriend);
}


NS_AZOOMEE_CHAT_END
