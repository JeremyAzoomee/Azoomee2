#include "FriendListScene.h"
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/UI/ModalMessages.h>
#include <AzoomeeCommon/UI/MessageBox.h>
#include <AzoomeeCommon/Data/Child/ChildDataProvider.h>
#include <AzoomeeCommon/Data/Parent/ParentDataProvider.h>
#include <AzoomeeCommon/Audio/AudioMixer.h>
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include "MessageScene.h"

using namespace cocos2d;


NS_AZOOMEE_CHAT_BEGIN

bool FriendListScene::init()
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
    _titleBar = TitleBarWidget::create();
    _titleBar->setTitleImage("res/chat/ui/azoomee_chat_logo.png");
    _titleBar->setSizeType(ui::Widget::SizeType::PERCENT);
    _titleBar->setLayoutParameter(CreateTopCenterRelativeLayoutParam());
    _titleBar->addBackButtonEventListener([this](Ref* button){
        onBackButtonPressed();
    });
    _rootLayout->addChild(_titleBar);
    
    createContentUI(_contentLayout);
    
    return true;
}

void FriendListScene::onEnter()
{
    Super::onEnter();
    
    // Create a friend object which represents the current user
    const std::string& childId = ChildDataProvider::getInstance()->getParentOrChildId();
    const std::string& childName = ChildDataProvider::getInstance()->getLoggedInChildName();
    const std::string& childAvatar = ChildDataProvider::getInstance()->getParentOrChildAvatarId();
    cocos2d::log("my avatar id: %s", ChildDataProvider::getInstance()->getParentOrChildAvatarId().c_str());
    _currentUser = Friend::create(childId, childName, childAvatar);
    
    // Register for API events
    ChatAPI::getInstance()->registerObserver(this);
    
    // Get friend list
    ChatAPI::getInstance()->requestFriendList();
    ModalMessages::getInstance()->startLoading();
}

void FriendListScene::onExit()
{
    Super::onExit();
    
    // Unregister on chat API events
    ChatAPI::getInstance()->removeObserver(this);
}

#pragma mark - Size Changes

void FriendListScene::onSizeChanged()
{
    Super::onSizeChanged();
    
    const cocos2d::Size& contentSize = getContentSize();
    const bool isLandscape = contentSize.width > contentSize.height;
    
    // TODO: Grab sizes from config
    
    // Main layout
    const Vec2& titleBarSize = Vec2(1.0f, (isLandscape) ? 0.131f : 0.084f);
    _titleBar->setSizePercent(titleBarSize);
    const Vec2& contentLayoutSize = Vec2(1.0f, 1.0f - titleBarSize.y);
    _contentLayout->setSizePercent(contentLayoutSize);
    // Subtitle bar uses same height as title bar
    const Vec2& subTitleBarSize = Vec2(1.0f, titleBarSize.y / contentLayoutSize.y);
    _subTitleBar->setSizePercent(subTitleBarSize);
    _subTitleBarBorder->setContentSize(Size(_subTitleBar->getContentSize().width, 2.0f));
    
    _friendListView->setSizePercent(Vec2(0.9f, 1.0f - subTitleBarSize.y));
    // 2 column on landscape, 1 column portrait
    _friendListView->setColumns((isLandscape) ? 2 : 1);
}

#pragma mark - UI creation

void FriendListScene::createContentUI(cocos2d::ui::Layout* parent)
{
    parent->setLayoutType(ui::Layout::Type::VERTICAL);
    
    // Subtitle bar
    _subTitleBar = ui::Layout::create();
    _subTitleBar->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
    parent->addChild(_subTitleBar);
    createSubTitleBarUI(_subTitleBar);
    
    // Contact list
    _friendListView = FriendListView::create();
    _friendListView->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
    _friendListView->addItemSelectedEventListener(CC_CALLBACK_1(FriendListScene::onFriendListItemSelected, this));
    parent->addChild(_friendListView);
}

void FriendListScene::createSubTitleBarUI(cocos2d::ui::Layout* parent)
{
    parent->setLayoutType(ui::Layout::Type::RELATIVE);
    
    // Content of the sub title bar
    ui::Layout* contentLayout = ui::Layout::create();
    contentLayout->setLayoutType(ui::Layout::Type::HORIZONTAL);
    contentLayout->setLayoutParameter(CreateCenterRelativeLayoutParam());
    contentLayout->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    contentLayout->setBackGroundColor(Style::Color::blueGreen);
    parent->addChild(contentLayout);
    
    // Title
    ui::Text* titleLabel = ui::Text::create();
    titleLabel->setFontName(Style::Font::Regular);
    titleLabel->setFontSize(95.0f);
    titleLabel->setTextColor(Color4B(Style::Color::brightAqua));
    // TODO: Get from Strings
    titleLabel->setString("My Friends");
    titleLabel->setLayoutParameter(CreateCenterVerticalLinearLayoutParam());
    contentLayout->addChild(titleLabel);
    
    // Add friend button
    ui::Button* addFriendButton = ui::Button::create("res/chat/ui/buttons/outline_button.png");
    // TODO: Get from Strings
    addFriendButton->setTitleText("Add a friend");
    addFriendButton->setTitleColor(Style::Color::brightAqua);
    addFriendButton->setTitleFontName(Style::Font::Regular);
    addFriendButton->setTitleFontSize(45.0f);
    addFriendButton->setScale9Enabled(true);
    addFriendButton->setTitleAlignment(TextHAlignment::LEFT, TextVAlignment::CENTER);
    
    ui::ImageView* plusIcon = ui::ImageView::create("res/chat/ui/buttons/add_icon.png");
    addFriendButton->addChild(plusIcon);
    plusIcon->setAnchorPoint(Vec2(0.5f, 0.5f));
    // Position icon and title
    const auto& addFriendButtonSize = addFriendButton->getContentSize();
    plusIcon->setPosition(Vec2(addFriendButtonSize.height * 0.5f, addFriendButtonSize.height * 0.5f));
    addFriendButton->getTitleRenderer()->setAnchorPoint(Vec2(0.0f, 0.5f));
    // We need some offset because the title doesn't get centered vertically correctly
    // Likely due to font renderering via TTF
    const float lineHeightOffset = -3.0f;
    addFriendButton->getTitleRenderer()->setPosition(Vec2(plusIcon->getPositionX() + (plusIcon->getContentSize().width * 0.5f) + 15.0f, (addFriendButtonSize.height * 0.5f) + lineHeightOffset));
    
//    addFriendButton->getRendererNormal()->setStrechEnabled(true);
//    addFriendButton->getRendererClicked()->setStrechEnabled(true);
//    addFriendButton->getRendererDisabled()->setStrechEnabled(true);
    const float buttonLeftMargin = 50.0f;
    addFriendButton->setLayoutParameter(CreateCenterVerticalLinearLayoutParam(ui::Margin(buttonLeftMargin, 0.0f, 0.0f, 0.0f)));
    addFriendButton->addClickEventListener([this](Ref* button){
        Azoomee::Chat::delegate->onChatAddFriend();
    });
    contentLayout->addChild(addFriendButton);
    
    // Size the content layer to fit, so everything is centered
    const Size& titleSize = titleLabel->getContentSize();
    const Size& buttonSize = addFriendButton->getContentSize();
    const float totalWidth = titleSize.width + buttonLeftMargin + buttonSize.width;
    contentLayout->setContentSize(Size(totalWidth, 0.0f));
    
    
    // Border at bottom
    _subTitleBarBorder = ui::Layout::create();
    _subTitleBarBorder->setLayoutParameter(CreateBottomCenterRelativeLayoutParam());
    _subTitleBarBorder->setSizeType(ui::Widget::SizeType::ABSOLUTE);
    _subTitleBarBorder->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    _subTitleBarBorder->setBackGroundColor(Style::Color::greenishTeal);
    parent->addChild(_subTitleBarBorder);
}

#pragma mark - Interaction

void FriendListScene::onBackButtonPressed()
{
    AudioMixer::getInstance()->playEffect(BACK_BUTTON_AUDIO_EFFECT);
    AnalyticsSingleton::getInstance()->genericButtonPressEvent("ChatScene - BackButton");
    
    Azoomee::Chat::delegate->onChatNavigationBack();
}

void FriendListScene::onFriendListItemSelected(const FriendRef& friendData)
{
    if(friendData->friendId() == ParentDataProvider::getInstance()->getLoggedInParentId())
    {
        AnalyticsSingleton::getInstance()->genericButtonPressEvent("ChatScene - SelectedParent");
        AnalyticsSingleton::getInstance()->setChatFriendIsParent(true);
    }
    else
    {
        AnalyticsSingleton::getInstance()->genericButtonPressEvent("ChatScene - SelectedFriend");
        AnalyticsSingleton::getInstance()->setChatFriendIsParent(false);
    }
    
    AnalyticsSingleton::getInstance()->contentItemSelectedEvent("CHAT");
    
    AudioMixer::getInstance()->playEffect(OK_BUTTON_AUDIO_EFFECT);
    
    FriendList participants = { _currentUser, friendData };
    auto messageScene = MessageScene::create(participants);
    Director::getInstance()->replaceScene(TransitionSlideInR::create(0.25f, messageScene));
}

#pragma mark - ChatAPIObserver

void FriendListScene::onChatAPIGetFriendList(const FriendList& friendList)
{
    AnalyticsSingleton::getInstance()->setNumberOfChatFriends((int)friendList.size());
    
    _friendListData = friendList;
    _friendListView->setItems(friendList);
    
    ModalMessages::getInstance()->stopLoading();
}

void FriendListScene::onChatAPIMessageRecieved(const MessageRef& message)
{
    // Find the friend this message is from, and if necessary mark them as having unread messages
    for(const FriendRef& frnd : _friendListData)
    {
        if(frnd->friendId() == message->senderId())
        {
            frnd->markMessagesLocalUnread();
            // Force list to re-render
            _friendListView->setItems(_friendListData);
            break;
        }
    }
}

void FriendListScene::onChatAPICustomMessageReceived(const std::string &messageType, const std::map<std::string, std::string> &messageProperties)
{
    if(messageType == "MODERATION")
    {
        for(const FriendRef& frnd : _friendListData)
        {
            if(messageProperties.find("otherChildId") != messageProperties.end())
            {
                if(frnd->friendId() == messageProperties.at("otherChildId"))
                {
                    if(messageProperties.find("status") != messageProperties.end())
                    {
                        if(messageProperties.at("status") == "IN_MODERATION") frnd->markFriendInModeration(true);
                        if(messageProperties.at("status") == "ACTIVE") frnd->markFriendInModeration(false);
                        
                        _friendListView->setItems(_friendListData);
                    }

                    break;
                }
            }
        }
    }
}

void FriendListScene::onChatAPIErrorRecieved(const std::string& requestTag, long errorCode)
{
    ModalMessages::getInstance()->stopLoading();
    MessageBox::createWith(ERROR_CODE_SOMETHING_WENT_WRONG, nullptr);
}


NS_AZOOMEE_CHAT_END
