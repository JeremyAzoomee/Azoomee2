#include "FriendListScene.h"
#include <TinizineCommon/Utils/LocaleManager.h>
#include <TinizineCommon/UI/Colour.h>
#include "../../ModalMessages.h"
#include <TinizineCommon/Data/Child/ChildManager.h>
#include <TinizineCommon/Data/Parent/UserAccountManager.h>
#include <TinizineCommon/Audio/AudioMixer.h>
#include <TinizineCommon/Analytics/AnalyticsSingleton.h>
#include <TinizineCommon/ImageDownloader/RemoteImageSprite.h>
#include <TinizineCommon/Device.h>
#include "MessageScene.h"
#include "../../PopupMessageBox.h"
#include "../../Style.h"

using namespace cocos2d;


USING_NS_TZ

NS_AZ_CHAT_BEGIN

bool FriendListScene::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    _isParent = !ChildManager::getInstance()->isChildLoggedIn();
    
    // Create the root layout which fills the whole screen
    _rootLayout = ui::Layout::create();
    _rootLayout->setSizeType(ui::Widget::SizeType::PERCENT);
    _rootLayout->setSizePercent(Vec2(1.0f, 1.0f));
    
    if(TZ::Device::getInstance()->isDeviceIphoneX())
    {
        const bool isLandscape = _rootLayout->getContentSize().width > _rootLayout->getContentSize().height;
        
        if(isLandscape)
        {
            _rootLayout->setSizePercent(Vec2(0.9f, 1.0f));
            _rootLayout->setPosition(Point(Director::getInstance()->getVisibleOrigin().x + this->getContentSize().width * 0.05 , 0));
        }
        else
        {
            _rootLayout->setSizePercent(Vec2(1.0f, 0.9f));
            _rootLayout->setPosition(Point(0, Director::getInstance()->getVisibleOrigin().y + this->getContentSize().height * 0.05));
        }
    }
    
    _rootLayout->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    _rootLayout->setBackGroundColor(Colours::Color_3B::black);
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
    if(!_isParent)
    {
        const std::string& childId = ChildManager::getInstance()->getLoggedInChild()->getId();
        const std::string& childName = ChildManager::getInstance()->getLoggedInChild()->getProfileName();
        const std::string& childAvatar = ChildManager::getInstance()->getLoggedInChild()->getAvatar();
        _currentUser = Friend::create(childId, childName, childAvatar);
    }
    else
    {
        const std::string& parentId = UserAccountManager::getInstance()->getLoggedInParentId();
        const std::string& ParentName = UserAccountManager::getInstance()->getParentDisplayName();
        const std::string& ParentAvatar = UserAccountManager::getInstance()->getParent()->getAvatar();
        _currentUser = Friend::create(parentId, ParentName, ParentAvatar);
    }
    
    // Register for API events
    ChatAPI::getInstance()->registerObserver(this);
	
	if(!_isParent && ChildManager::getInstance()->getLoggedInChild()->isSessionExpired())
	{
		ModalMessages::getInstance()->startLoading();
		ChatAPI::getInstance()->refreshChildSession();
	}
	else
	{
    	// Up the schedule speed of friend list polling
    	ChatAPI::getInstance()->requestFriendList();
    	ChatAPI::getInstance()->scheduleFriendListPoll( ChatAPI::kScheduleRateHigh );
    	ModalMessages::getInstance()->startLoading();
	}
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
    
    if(TZ::Device::getInstance()->isDeviceIphoneX())
    {
        if(isLandscape)
        {
            _rootLayout->setSizePercent(Vec2(0.9f, 1.0f));
            _rootLayout->setPosition(Point(Director::getInstance()->getVisibleOrigin().x + this->getContentSize().width * 0.05 , 0));
        }
        else
        {
            _rootLayout->setSizePercent(Vec2(1.0f, 0.9f));
            _rootLayout->setPosition(Point(0, Director::getInstance()->getVisibleOrigin().y + this->getContentSize().height * 0.05));
        }
    }
    
    // TODO: Grab sizes from config
    
    // Main layout
    const Vec2& titleBarSize = Vec2(1.0f, (isLandscape) ? 0.131f : 0.084f);
    const Vec2& subTitleBarSize = Vec2(1.0f, (isLandscape) ? 0.15f : 0.07f);
    
    _titleBar->setSizePercent(titleBarSize);
    const Vec2& contentLayoutSize = Vec2(1.0f, 1.0f - titleBarSize.y);
    _contentLayout->setSizePercent(contentLayoutSize);
    
    _subTitleBar->setSizePercent(subTitleBarSize);
    _subTitleBarBorder->setContentSize(Size(contentSize.width,2.0f));
    
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
    _subTitleBar->setBackGroundColor(Colours::Color_3B::dark);
    
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
    
    // Add the current user's name and avatar to the subtitle bar
    
    auto childDataLayout = ui::Layout::create();
    childDataLayout->setLayoutType(cocos2d::ui::Layout::Type::HORIZONTAL);
    childDataLayout->setLayoutParameter(CreateCenterRelativeLayoutParam());
    childDataLayout->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    childDataLayout->setBackGroundColor(Colours::Color_3B::dark);
    
    std::string oomeeFileName;
    std::string displayName;
    
    auto childAvatar = ui::Layout::create();
    childAvatar->setLayoutParameter(CreateCenterVerticalLinearLayoutParam());
    childAvatar->setSizeType(ui::Widget::SizeType::ABSOLUTE);
    
    if(!_isParent)
    {
        oomeeFileName = ChildManager::getInstance()->getLoggedInChild()->getAvatar();
		displayName = "   " + ChildManager::getInstance()->getLoggedInChild()->getProfileName() + " (" + _("Kid Code:") + " " + ChildManager::getInstance()->getLoggedInChild()->getInviteCode() + ")";
        auto childAvatarSprite = RemoteImageSprite::create();
        childAvatarSprite->setKeepAspectRatio(true);
        childAvatarSprite->initWithUrlAndSize(oomeeFileName, Size(128,128));
        childAvatarSprite->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
        childAvatar->setContentSize(Size(128,128));
        childAvatar->addChild(childAvatarSprite);
    }
    else
    {
        oomeeFileName = "res/childSelection/om_GenericParent.png";
        displayName = "   " + _("Parent");
        auto parentAvatar = Sprite::create(oomeeFileName);
        parentAvatar->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
        childAvatar->setContentSize(parentAvatar->getContentSize());
        childAvatar->addChild(parentAvatar);
        childAvatar->setScale(0.25);
    }
    
    //childAvatar->setScale(0.25f);
    
    auto childInfoLabel = ui::Text::create(displayName, AZ::Style::Font::Regular(), 48);
    childInfoLabel->setColor(Colours::Color_3B::white);
    childInfoLabel->setLayoutParameter(CreateCenterVerticalLinearLayoutParam());
    childInfoLabel->setSizeType(ui::Widget::SizeType::ABSOLUTE);
    
    childDataLayout->setSizeType(ui::Widget::SizeType::ABSOLUTE);
    childDataLayout->setContentSize(Size(childAvatar->getBoundingBox().size.width + childInfoLabel->getContentSize().width, childAvatar->getBoundingBox().size.height));
    childDataLayout->addChild(childAvatar);
    childDataLayout->addChild(childInfoLabel);
    
    _subTitleBar->addChild(childDataLayout);
    
    // Border at bottom
    _subTitleBarBorder = ui::Layout::create();
    _subTitleBarBorder->setLayoutParameter(CreateBottomCenterRelativeLayoutParam());
    _subTitleBarBorder->setSizeType(ui::Widget::SizeType::ABSOLUTE);
    _subTitleBarBorder->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    _subTitleBarBorder->setBackGroundColor(Colours::Color_3B::greenishTeal);
    parent->addChild(_subTitleBarBorder);
}

#pragma mark - Interaction

void FriendListScene::onBackButtonPressed()
{
    AudioMixer::getInstance()->playEffect("res/audio/Azoomee_Button_Click_01_v1.mp3");
    AnalyticsSingleton::getInstance()->genericButtonPressEvent("ChatScene - BackButton");
    
    // Reset the polling time
    ChatAPI::getInstance()->scheduleFriendListPoll( ChatAPI::kScheduleRateLow );
    
    AZ::Chat::delegate->onChatNavigationBack();
}

void FriendListScene::onFriendListItemSelected(const FriendRef& friendData)
{
    const bool isParent = friendData->friendId() == UserAccountManager::getInstance()->getLoggedInParentId();
    AnalyticsSingleton::getInstance()->setChatFriendIsParent(isParent);
    AnalyticsSingleton::getInstance()->genericButtonPressEvent(isParent ? "ChatScene - SelectedParent" : "ChatScene - SelectedFriend");
    
    AnalyticsSingleton::getInstance()->contentItemSelectedEvent("CHAT");
    
    AudioMixer::getInstance()->playEffect("res/audio/Azoomee_Button_Click_06_v1.mp3");
    
    FriendList participants = { _currentUser, friendData };
    auto messageScene = MessageScene::create(participants);
    Director::getInstance()->replaceScene(TransitionSlideInB::create(0.25f, messageScene));
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
    const auto& errorMessageText = LocaleManager::getInstance()->getErrorMessageWithCode(errorCode);
           
    PopupMessageBox* messageBox = PopupMessageBox::create();
    messageBox->setTitle(errorMessageText.at(ERROR_TITLE));
    messageBox->setBody(errorMessageText.at(ERROR_BODY));
    messageBox->setButtonText(_("Back"));
    messageBox->setButtonColour(Colours::Color_3B::darkIndigo);
    messageBox->setPatternColour(Colours::Color_3B::azure);
    messageBox->setButtonPressedCallback([this](MessagePopupBase* pSender){
        pSender->removeFromParent();
    });
    addChild(messageBox, 1);
}

void FriendListScene::onChatAPIModerationStatusChanged(const FriendRef& friendObj)
{
    // Refresh the list with existing data as the data should already be updated
    // and we just need to update the UI
    _friendListView->setItems(_friendListData);
}

void FriendListScene::onChatAPIRefreshChildSession()
{
	ChatAPI::getInstance()->requestFriendList();
	ChatAPI::getInstance()->scheduleFriendListPoll( ChatAPI::kScheduleRateHigh );
}

NS_AZ_CHAT_END
