#include "FriendListScene.h"
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/UI/ModalMessages.h>
#include "MessageScene.h"

// TODO: This needs to be a dynamic hook, so app can deal with it when we're in the main app
#include "../../ChildSelectorScene.h"
#include "../../Auth/AuthAPI.h"


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
    // We add this last so it sits on top with a drop shadow
    _titleBar = TitleBarWidget::create();
    _titleBar->setTitleString("Chat"); // @TODO: Get from Strings
    _titleBar->setTitleColor(Style::Color::kermitGreenTwo);
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
    // TODO: Grab sizes from config
    bool isLandscape = contentSize.width > contentSize.height;
    
    // Main layout
    const Vec2& titleBarSize = Vec2(1.0f, (isLandscape) ? 0.131f : 0.084f);
    _titleBar->setSizePercent(titleBarSize);
    _contentLayout->setSizePercent(Vec2(1.0f, 1.0f - titleBarSize.y));
    
    // Layout of content changes depending on orientation
    const Vec2& userPanelSize = Vec2((isLandscape) ? 0.5f : 1.0f, (isLandscape) ? 1.0f : 0.35f);
    _userPanel->setSizePercent(userPanelSize);
    _friendListView->setSizePercent(Vec2((isLandscape) ? 0.5f : 1.0f, (isLandscape) ? 1.0f : 1.0f - userPanelSize.y));
    ui::RelativeLayoutParameter* contactListLayout = (ui::RelativeLayoutParameter*) _friendListView->getLayoutParameter();
    contactListLayout->setAlign(isLandscape ? ui::RelativeLayoutParameter::RelativeAlign::PARENT_TOP_LEFT : ui::RelativeLayoutParameter::RelativeAlign::PARENT_BOTTOM_CENTER_HORIZONTAL);
    ui::RelativeLayoutParameter* userPanelLayout = (ui::RelativeLayoutParameter*) _userPanel->getLayoutParameter();
    userPanelLayout->setAlign(isLandscape ? ui::RelativeLayoutParameter::RelativeAlign::PARENT_TOP_RIGHT : ui::RelativeLayoutParameter::RelativeAlign::PARENT_TOP_CENTER_HORIZONTAL);
    
    
    // User panel
    Vec2 oomeeSize = Vec2(1.0f, (isLandscape) ? 0.458f : 0.70f);
    // TODO: Get aspect ratio from OomeeWidget, for now assume square
    float aspectRatio = 1.0f; // (width / height)
    // Calc width from aspect ratio
    float widthPt = _userPanel->getContentSize().height * oomeeSize.y * aspectRatio;
    oomeeSize.x = widthPt / _userPanel->getContentSize().width;
    _userOomee->setSizePercent(oomeeSize);
}

#pragma mark - UI creation

void FriendListScene::createContentUI(cocos2d::ui::Layout* parent)
{
    parent->setLayoutType(ui::Layout::Type::RELATIVE);
    
    // Contact list
    _friendListView = FriendListView::create();
    _friendListView->setLayoutParameter(CreateTopLeftRelativeLayoutParam());
    _friendListView->addItemSelectedEventListener(std::bind(&FriendListScene::onFriendListItemSelected, this, std::placeholders::_1));
    parent->addChild(_friendListView);
    
    // User panel - holds oomee etc
    _userPanel = ui::Layout::create();
    _userPanel->setSizeType(ui::Widget::SizeType::PERCENT);
    _userPanel->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    _userPanel->setBackGroundColor(Style::Color::dustyLavender);
    _userPanel->setLayoutParameter(CreateTopRightRelativeLayoutParam());
    parent->addChild(_userPanel);
    
    createUserPanelUI(_userPanel);
}

void FriendListScene::createUserPanelUI(cocos2d::ui::Layout* parent)
{
    parent->setLayoutType(ui::Layout::Type::RELATIVE);
    
    _userOomee = OomeeWidget::create();
    _userOomee->setSizeType(ui::Widget::SizeType::PERCENT);
    _userOomee->setLayoutParameter(CreateCenterRelativeLayoutParam());
    parent->addChild(_userOomee);
}

#pragma mark - Interaction

void FriendListScene::onBackButtonPressed()
{
    // TODO: This needs to be a dynamic hook, so app can deal with it when we're in the main app
    // Logout child
    AuthAPI::getInstance()->logoutChild();
    // Back to profile select
    auto childSelectScene = ChildSelectorScene::create();
    Director::getInstance()->replaceScene(childSelectScene);
}

void FriendListScene::onFriendListItemSelected(const FriendRef& friendData)
{
    auto messageScene = MessageScene::create(friendData);
    Director::getInstance()->replaceScene(TransitionSlideInR::create(0.25f, messageScene));
}

#pragma mark - ChatAPIObserver

void FriendListScene::onChatAPIGetFriendList(const FriendList& friendList)
{
    _friendListView->setItems(friendList);
    
    ModalMessages::getInstance()->stopLoading();
}


NS_AZOOMEE_CHAT_END
