#include "FriendListScene.h"
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/UI/ModalMessages.h>
#include <AzoomeeCommon/Data/Child/ChildDataProvider.h>
#include <AzoomeeCommon/Audio/AudioMixer.h>
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
    _contentLayout->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    _contentLayout->setBackGroundColor(Style::Color::darkTwo);
    _rootLayout->addChild(_contentLayout);
    
    // Titlebar at the top
    // We add this last so it sits on top with a drop shadow
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
    const std::string& childId = ChildDataProvider::getInstance()->getLoggedInChildId();
    const std::string& childName = ChildDataProvider::getInstance()->getLoggedInChildName();
    const std::string& childAvatar = ChildDataProvider::getInstance()->getLoggedInChildAvatarId();
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
    _subTitleBar->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    _subTitleBar->setBackGroundColor(Style::Color::brightAqua);
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
    ;
}

#pragma mark - Interaction

void FriendListScene::onBackButtonPressed()
{
    AudioMixer::getInstance()->playEffect(BACK_BUTTON_AUDIO_EFFECT);
    
    Azoomee::Chat::delegate->onChatNavigationBack();
}

void FriendListScene::onFriendListItemSelected(const FriendRef& friendData)
{
    AudioMixer::getInstance()->playEffect(OK_BUTTON_AUDIO_EFFECT);
    
    FriendList participants = { _currentUser, friendData };
    auto messageScene = MessageScene::create(participants);
    Director::getInstance()->replaceScene(TransitionSlideInR::create(0.25f, messageScene));
}

#pragma mark - ChatAPIObserver

void FriendListScene::onChatAPIGetFriendList(const FriendList& friendList)
{
    _friendListView->setItems(friendList);
    
    ModalMessages::getInstance()->stopLoading();
}


NS_AZOOMEE_CHAT_END
