#include "FriendListSceneArtPreview.h"
#include <AzoomeeCommon/Strings.h>
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

bool FriendListSceneArtPreview::init()
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
    
    // Back button
    _backButton = ui::Button::create("res/chat/ui/buttons/back_button.png");
    _backButton->setNormalizedPosition(Vec2(0,1));
    _backButton->setAnchorPoint(Vec2(-0.5,1.5));
    _backButton->addClickEventListener([this](Ref* button){
        onBackButtonPressed();
    });
    addChild(_backButton);
    
    createContentUI(_contentLayout);
    
    return true;
}

void FriendListSceneArtPreview::onEnter()
{
    Super::onEnter();
    
    // Create a friend object which represents the current user
    const std::string& childId = ChildDataProvider::getInstance()->getParentOrChildId();
    const std::string& childName = ChildDataProvider::getInstance()->getParentOrChildName();
    const std::string& childAvatar = ChildDataProvider::getInstance()->getParentOrChildAvatarId();
    _currentUser = Friend::create(childId, childName, childAvatar);
    
    // Register for API events
    ChatAPI::getInstance()->registerObserver(this);
    
    // Up the schedule speed of friend list polling
    ChatAPI::getInstance()->requestFriendList();
    ChatAPI::getInstance()->scheduleFriendListPoll( ChatAPI::kScheduleRateHigh );
    ModalMessages::getInstance()->startLoading();
}

void FriendListSceneArtPreview::onExit()
{
    Super::onExit();
    
    // Unregister on chat API events
    ChatAPI::getInstance()->removeObserver(this);
}

#pragma mark - Size Changes

void FriendListSceneArtPreview::onSizeChanged()
{
    Super::onSizeChanged();
    
    const cocos2d::Size& contentSize = getContentSize();
    const bool isLandscape = contentSize.width > contentSize.height;
    
    // TODO: Grab sizes from config
    
    // Main layout
    const Vec2& contentLayoutSize = Vec2(1.0f, 1.0f);
    _contentLayout->setSizePercent(contentLayoutSize);
    // Subtitle bar uses same height as title bar
    const Vec2& subTitleBarSize = Vec2(1.0f, 0);
    _subTitleBar->setSizePercent(subTitleBarSize);
    _subTitleBarBorder->setContentSize(Size(_subTitleBar->getContentSize().width, 2.0f));
    _artPreviewText->setFontSize((isLandscape) ? 75.0f : 67.5f);
    // Percentage height used by art preview with padding
    float artPreviewScreenPercent = (_artPreviewLayout->getContentSize().height + _backButton->getContentSize().height + 75.0f * 1.5f ) / contentSize.height;
    float listViewHeight = 1 - artPreviewScreenPercent;
    _friendListView->setSizePercent(Vec2(0.9f, listViewHeight));
    // 2 column on landscape, 1 column portrait
    _friendListView->setColumns((isLandscape) ? 2 : 1);
    _friendListView->hideUnreadIndicators();
}

#pragma mark - UI creation

void FriendListSceneArtPreview::createContentUI(cocos2d::ui::Layout* parent)
{
    parent->setLayoutType(ui::Layout::Type::VERTICAL);
    
    ui::Layout* paddingLayoutArt = ui::Layout::create();
    paddingLayoutArt->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
    paddingLayoutArt->setLayoutType(ui::Layout::Type::RELATIVE);
    paddingLayoutArt->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    paddingLayoutArt->setBackGroundColor(Style::Color::dark);
    parent->addChild(paddingLayoutArt);
    
    _artPreviewLayout = ui::Layout::create();
    _artPreviewLayout->setLayoutParameter(CreateCenterRelativeLayoutParam());
    paddingLayoutArt->addChild(_artPreviewLayout);
    createArtPreviewUI(_artPreviewLayout);
    paddingLayoutArt->setContentSize(Size(Director::getInstance()->getVisibleSize().width,_artPreviewLayout->getContentSize().height + _backButton->getContentSize().height));

    ui::Layout* paddingLayout = ui::Layout::create();
    paddingLayout->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
    paddingLayout->setLayoutType(ui::Layout::Type::RELATIVE);
    paddingLayout->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    paddingLayout->setBackGroundColor(Style::Color::dark);
    parent->addChild(paddingLayout);
    
    _artPreviewText = ui::Text::create();
    _artPreviewText->setFontName(Style::Font::Regular());
    _artPreviewText->setFontSize(75.0f);
    _artPreviewText->setTextColor(Color4B(Style::Color::white));
    _artPreviewText->setString(_("Who would you like to share this with?"));
    _artPreviewText->setLayoutParameter(CreateTopCenterRelativeLayoutParam());
    paddingLayout->addChild(_artPreviewText);
    paddingLayout->setContentSize(Size(Director::getInstance()->getVisibleSize().width, _artPreviewText->getContentSize().height * 1.5f));
    
    // Subtitle bar
    _subTitleBar = ui::Layout::create();
    _subTitleBar->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
    parent->addChild(_subTitleBar);
    createSubTitleBarUI(_subTitleBar);
    
    // Contact list
    _friendListView = FriendListView::create();
    _friendListView->setLayoutParameter(CreateCenterHorizontalLinearLayoutParam());
    _friendListView->addItemSelectedEventListener(CC_CALLBACK_1(FriendListSceneArtPreview::onFriendListItemSelected, this));
    _friendListView->setIncludeAddFriendButton(false);
    parent->addChild(_friendListView);
}

void FriendListSceneArtPreview::createSubTitleBarUI(cocos2d::ui::Layout* parent)
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

void FriendListSceneArtPreview::createArtPreviewUI(ui::Layout *parent)
{
    parent->setLayoutType(ui::Layout::Type::RELATIVE);
    
    ui::ImageView* artPreview = ui::ImageView::create(delegate->_imageFileName);
    artPreview->ignoreContentAdaptWithSize(false); // stretch the image
    artPreview->setAnchorPoint(Vec2(0.5, 0.5));
    artPreview->setLayoutParameter(CreateCenterRelativeLayoutParam());
    artPreview->setContentSize(artPreview->getVirtualRenderer()->getContentSize() * 0.75);
    artPreview->setPosition(artPreview->getContentSize()/2);
    
    ui::ImageView* stencilSprite = ui::ImageView::create("res/artapp/popup_bg.png");
    stencilSprite->setScale9Enabled(true);
    stencilSprite->setAnchorPoint(Vec2(0.5,0.5));
    stencilSprite->setLayoutParameter(CreateCenterRelativeLayoutParam());
    stencilSprite->setContentSize(artPreview->getContentSize());
    stencilSprite->setPosition(artPreview->getContentSize()/2);
    parent->addChild(stencilSprite);
    
    ClippingNode* clipNode = ClippingNode::create(stencilSprite);
    clipNode->setContentSize(stencilSprite->getContentSize());
    clipNode->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    clipNode->setAlphaThreshold(0.5f);
    clipNode->addChild(artPreview);
    clipNode->setPosition(stencilSprite->getPosition());
    parent->addChild(clipNode);
    
    parent->setContentSize(artPreview->getContentSize());
}

#pragma mark - Interaction

void FriendListSceneArtPreview::onBackButtonPressed()
{
    AudioMixer::getInstance()->playEffect(BACK_BUTTON_AUDIO_EFFECT);
    AnalyticsSingleton::getInstance()->genericButtonPressEvent("ChatScene - BackButton");
    
    // Reset the polling time
    ChatAPI::getInstance()->scheduleFriendListPoll( ChatAPI::kScheduleRateLow );
    Azoomee::Chat::delegate->_imageFileName = "";
    Azoomee::Chat::delegate->_sharedContentId = "";
    Azoomee::Chat::delegate->onChatNavigationBack();
}

void FriendListSceneArtPreview::onFriendListItemSelected(const FriendRef& friendData)
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

void FriendListSceneArtPreview::onChatAPIGetFriendList(const FriendList& friendList, int amountOfNewMessages)
{
    AnalyticsSingleton::getInstance()->setNumberOfChatFriends((int)friendList.size());
    
    _friendListData = friendList;
    _friendListView->setItems(friendList);
    _friendListView->hideUnreadIndicators();
    
    ModalMessages::getInstance()->stopLoading();
}

void FriendListSceneArtPreview::onChatAPIErrorRecieved(const std::string& requestTag, long errorCode)
{
    ModalMessages::getInstance()->stopLoading();
    MessageBox::createWith(ERROR_CODE_SOMETHING_WENT_WRONG, nullptr);
}


NS_AZOOMEE_CHAT_END
