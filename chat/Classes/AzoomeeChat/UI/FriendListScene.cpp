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
    
    //create Privacy Button, set location in OnSizeChanged.
    _privacyButton = PrivacyLayer::createWithColor();
    _titleBar->addChild(_privacyButton);
    
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
    
    // Up the schedule speed of friend list polling
    ChatAPI::getInstance()->requestFriendList();
    ChatAPI::getInstance()->scheduleFriendListPoll( ChatAPI::kScheduleRateHigh );
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
    _subTitleBar->setSizePercent(Vec2(1.0f, 0.01f));
    _subTitleBarBorder->setContentSize(Size(contentSize.width,2.0f));
    _friendListView->setSizePercent(Vec2(0.9f, 0.99));
    // 2 column on landscape, 1 column portrait
    _friendListView->setColumns((isLandscape) ? 2 : 1);

    //Set location according the the titlebar
    _privacyButton->setCenterPosition(Vec2(_titleBar->getContentSize().width - _privacyButton->getContentSize().height/2 -_privacyButton->getContentSize().width/2,_titleBar->getContentSize().height - _privacyButton->getContentSize().height));
}

#pragma mark - UI creation

void FriendListScene::createContentUI(cocos2d::ui::Layout* parent)
{
    parent->setLayoutType(ui::Layout::Type::VERTICAL);
    
    // Subtitle bar
    _subTitleBar = ui::Layout::create();
    _subTitleBar->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
    _subTitleBar->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    _subTitleBar->setBackGroundColor(Style::Color::dark);
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
    
    // Reset the polling time
    ChatAPI::getInstance()->scheduleFriendListPoll( ChatAPI::kScheduleRateLow );
    
    Azoomee::Chat::delegate->onChatNavigationBack();
}

void FriendListScene::onFriendListItemSelected(const FriendRef& friendData)
{
    const bool isParent = friendData->friendId() == ParentDataProvider::getInstance()->getLoggedInParentId();
    AnalyticsSingleton::getInstance()->setChatFriendIsParent(isParent);
    AnalyticsSingleton::getInstance()->genericButtonPressEvent(isParent ? "ChatScene - SelectedParent" : "ChatScene - SelectedFriend");
    
    AnalyticsSingleton::getInstance()->contentItemSelectedEvent("CHAT");
    
    AudioMixer::getInstance()->playEffect(OK_BUTTON_AUDIO_EFFECT);
    
    FriendList participants = { _currentUser, friendData };
    auto messageScene = MessageScene::create(participants);
    Director::getInstance()->replaceScene(TransitionSlideInR::create(0.25f, messageScene));
}

#pragma mark - ChatAPIObserver

void FriendListScene::onChatAPIGetFriendList(const FriendList& friendList, int amountOfNewMessages)
{
    AnalyticsSingleton::getInstance()->setNumberOfChatFriends((int)friendList.size());
    
    _friendListData = friendList;
    _friendListView->setItems(friendList);
    
    ModalMessages::getInstance()->stopLoading();
}

void FriendListScene::onChatAPIErrorRecieved(const std::string& requestTag, long errorCode)
{
    ModalMessages::getInstance()->stopLoading();
    MessageBox::createWith(ERROR_CODE_SOMETHING_WENT_WRONG, nullptr);
}


NS_AZOOMEE_CHAT_END
