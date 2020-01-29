//
//  HQScene.cpp
//  Azoomee
//
//  Created by Macauley.Scoffins on 21/08/2019.
//

#include "HQScene.h"
#include "HQHistoryManager.h"
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>
#include <AzoomeeCommon/Data/Child/ChildManager.h>
#include <AzoomeeCommon/Data/Parent/UserAccountManager.h>
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include <AzoomeeCommon/Data/HQDataObject/HQDataObjectManager.h>
#include <AzoomeeCommon/Utils/LocaleManager.h>
#include "FlowDataSingleton.h"
#include "ContentHistoryManager.h"
#include "RewardDisplayHandler.h"
#include "SceneManagerScene.h"
#include "AgeGate.h"
#include "ContentOpener.h"


using namespace cocos2d;

NS_AZOOMEE_BEGIN


HQSnapshot::HQSnapshot()
{
    
}

void HQSnapshot::setScrollPositions(const std::map<HQType, float>& scrollPositions)
{
    _scrollPositions = scrollPositions;
}
std::map<HQType, float> HQSnapshot::getScrollPositions() const
{
    return _scrollPositions;
}

void HQSnapshot::setOpenDropDowns(const std::map<HQType, int>& openDorpdowns)
{
    _openDropDowns = openDorpdowns;
}
std::map<HQType, int> HQSnapshot::getOpenDropDowns() const
{
    return _openDropDowns;
}

void HQSnapshot::setActiveHQ(const HQType& activeHQ)
{
    _activeHQ = activeHQ;
}
HQType HQSnapshot::getActiveHQ() const
{
    return _activeHQ;
}

void HQSnapshot::setEpisodeSelectorOpen(bool open)
{
    _episodeSelectorOpen = open;
}
bool HQSnapshot::isEpisodeSelectorOpen() const
{
    return _episodeSelectorOpen;
}

void HQSnapshot::setSelectedSeries(const HQContentItemObjectRef& series)
{
    _selectedSeries = series;
}
HQContentItemObjectRef HQSnapshot::getSelectedSeries() const
{
    return _selectedSeries;
}

const float HQScene::kTitleBarPadding = 120.0f;

bool HQScene::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    _background = ui::Layout::create();
    _background->setSizeType(ui::Layout::SizeType::PERCENT);
    _background->setSizePercent(Vec2(1.0f,1.0f));
    _background->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    _background->setBackGroundColor(Style::Color::darkIndigo);
    addChild(_background);
    
    createHeaderUI();
    createNavigationUI();
    createPageUI();
    
    changeToPage(HQType::GAME);
    
    return true;
}

void HQScene::onEnter()
{
    OnScreenChecker::kUseStrictBoundry = false;
    
    _navBar->toggleHQSelected(_activePageName);

    ContentHistoryManager::getInstance()->setReturnedFromContent(false);
    HQHistoryManager::getInstance()->addHQToHistoryManager(_activePageName);
    
    _gameHQ->setPositionPercent(Vec2::ANCHOR_MIDDLE);
    _videoHQ->setPositionPercent(Vec2::ANCHOR_MIDDLE);
    _oomeeHQ->setPositionPercent(Vec2::ANCHOR_MIDDLE);
    _chatHQ->setPositionPercent(Vec2::ANCHOR_MIDDLE);
    
    Super::onEnter();
}

void HQScene::onExit()
{
    _gameHQ->setPositionPercent(_activePageName == HQConsts::kGameHQName ? Vec2::ANCHOR_MIDDLE : Vec2(-1,0.5));
    _videoHQ->setPositionPercent(_activePageName == HQConsts::kVideoHQName ? Vec2::ANCHOR_MIDDLE : Vec2(-1,0.5));
    _oomeeHQ->setPositionPercent(_activePageName == HQConsts::kOomeeHQName ? Vec2::ANCHOR_MIDDLE : Vec2(-1,0.5));
    _chatHQ->setPositionPercent(_activePageName == HQConsts::kChatHQName ? Vec2::ANCHOR_MIDDLE : Vec2(-1,0.5));
    OnScreenChecker::kUseStrictBoundry = true;
    Super::onExit();
}

void HQScene::onSizeChanged()
{
    Super::onSizeChanged();
    const Size& visibleSize = Director::getInstance()->getVisibleSize();
    _isPortrait = visibleSize.width < visibleSize.height;
    const Rect& safeAreaRect = Director::getInstance()->getSafeAreaRect();
    const float safeZonePadding = MAX(0, visibleSize.height - (safeAreaRect.origin.y + safeAreaRect.size.height));
    if(_titleBannerContent)
    {
        const Size& titleBannerSize = Size(visibleSize.width, HQConsts::TitleBannerHeight + safeZonePadding);
        _titleBanner->setContentSize(titleBannerSize);
        _topPattern->setContentSize(titleBannerSize);
        _patternGradient->setContentSize(Size(titleBannerSize.width, 107));
        _titleBannerContent->setContentSize(Size(visibleSize.width - kTitleBarPadding, titleBannerSize.height));
    }
    if(_HQPageTitle)
    {
        _HQPageTitle->setTextAreaSize(Size(visibleSize.width / 2, HQConsts::TitleBannerHeight - 20));
    }
    if(_pageLayout)
    {
        const float titleBannerHeight = _titleBanner->getContentSize().height;
        _pageLayout->setContentSize(Size(visibleSize.width * 0.95f, visibleSize.height - titleBannerHeight - _navBar->getContentSize().height));
        _pageLayout->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - titleBannerHeight));
        _pageLayout->forceDoLayout();
    }
    if(_verticalScrollGradient)
    {
        _verticalScrollGradient->setAnchorPoint(Vec2(_isPortrait ? 0.5f : 0.0f, 0.0f));
    }
}

void HQScene::createHeaderUI()
{
    const Size& visibleSize = Director::getInstance()->getVisibleSize();
    _isPortrait = visibleSize.width < visibleSize.height;
    const Rect& safeAreaRect = Director::getInstance()->getSafeAreaRect();
    const float safeZonePadding = visibleSize.height - (safeAreaRect.origin.y + safeAreaRect.size.height);
    
    _titleBanner = ui::Layout::create();
    _titleBanner->setContentSize(Size(visibleSize.width, HQConsts::TitleBannerHeight + safeZonePadding));
    _titleBanner->setClippingEnabled(true);
    _titleBanner->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _titleBanner->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_TOP);
    addChild(_titleBanner, 1);
    
    _topPattern = TileSprite::create();
    _topPattern->setTexture("res/decoration/pattern_stem_tile.png");
    _topPattern->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _topPattern->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_TOP);
    _topPattern->setColor(Style::Color::macaroniAndCheese);
    _titleBanner->addChild(_topPattern);
    
    const Color3B& gradColour = Style::Color::darkIndigo;
    _patternGradient = LayerGradient::create(Color4B(gradColour.r, gradColour.g, gradColour.b, 0), Color4B(gradColour));
    _patternGradient->setIgnoreAnchorPointForPosition(false);
    _patternGradient->setContentSize(Size(_titleBanner->getContentSize().width, 107));
    _patternGradient->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _patternGradient->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _titleBanner->addChild(_patternGradient);
    
    _titleBannerContent = ui::Layout::create();
    _titleBannerContent->setContentSize(Size(visibleSize.width - kTitleBarPadding, HQConsts::TitleBannerHeight));
    _titleBannerContent->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _titleBannerContent->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _titleBanner->addChild(_titleBannerContent);
    
    _settingsButton = SettingsButton::create();
    _settingsButton->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    _settingsButton->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_RIGHT);
    _titleBannerContent->addChild(_settingsButton);
    
    // add coin counter
    _coinDisplay = CoinDisplay::create();
    _coinDisplay->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_LEFT);
    _coinDisplay->setAnchorPoint(Vec2(1.1f,0.5f));
    _coinDisplay->setAnimate(false);
    _settingsButton->addChild(_coinDisplay);

    _HQPageTitle = DynamicText::create(_("Games"), Style::Font::PoppinsBold(), 107);
    _HQPageTitle->enableShadow(Color4B(0,0,0,125), Size(4,-8));
    _HQPageTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    _HQPageTitle->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_LEFT);
    _HQPageTitle->setOverflow(Label::Overflow::SHRINK);
    _HQPageTitle->setTextAreaSize(Size(visibleSize.width / 2, HQConsts::TitleBannerHeight - 20));
    _HQPageTitle->setTextColor(Color4B::WHITE);
    _HQPageTitle->setTextVerticalAlignment(TextVAlignment::CENTER);
    _titleBannerContent->addChild(_HQPageTitle);
    
}
void HQScene::createNavigationUI()
{
    const Size& visibleSize = Director::getInstance()->getVisibleSize();
    _isPortrait = visibleSize.width < visibleSize.height;
    
    _navBar = NavigationBar::create();
    _navBar->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _navBar->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _navBar->setHQSelectedCallback([this](HQType hq){
        const std::string& targetHQName = NavigationBar::kHQTypeToNameConv.at(hq);
        const HQDataObjectRef& targetHQ = HQDataObjectManager::getInstance()->getHQDataObjectForKey(targetHQName);
        
        if(!targetHQ->getHqEntitlement())
        {
#ifndef ALLOW_UNPAID_SIGNUP
            AgeGate* ageGate = AgeGate::create();
            ageGate->setActionCompletedCallback([ageGate](AgeGateResult result){
                ageGate->removeFromParent();
                if(result == AgeGateResult::SUCCESS)
                {
                    Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::IAP));
                }
            });
            Director::getInstance()->getRunningScene()->addChild(ageGate,AGE_GATE_Z_ORDER);
#else
            Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::Signup));
#endif
            return false;
        }
        
        // Navigation event
        const std::string& eventName = NavigationBar::kHQTypeToNameConv.at(hq);
        AnalyticsSingleton::getInstance()->navSelectionEvent(eventName);
        
        changeToPage(hq);
        return true;
    });
    addChild(_navBar, 1);
    
    _purchaseCapsule = PurchaseCapsule::create();
    _purchaseCapsule->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _purchaseCapsule->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_TOP);
    const BillingDataRef& billingData = UserAccountManager::getInstance()->getBillingData();
    BillingStatus billingStatus = BillingStatus::ANON;
    if(UserAccountManager::getInstance()->isUserLoggedIn())
    {
        billingStatus = billingData->getBillingStatus();
    }
    _purchaseCapsule->setUserType(billingStatus);
    _navBar->addChild(_purchaseCapsule);
    
    const Color3B& gradColour = Style::Color::darkIndigo;
    _verticalScrollGradient = LayerGradient::create(Color4B(gradColour.r, gradColour.g, gradColour.b, 0), Color4B(gradColour));
    _verticalScrollGradient->setIgnoreAnchorPointForPosition(false);
    _verticalScrollGradient->setContentSize(Size(2736, 160));
    _verticalScrollGradient->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    _verticalScrollGradient->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_TOP);
    _navBar->addChild(_verticalScrollGradient, -1);
}

void HQScene::createPageUI()
{
    const Size& visibleSize = Director::getInstance()->getVisibleSize();
    
    _pageLayout = ui::Layout::create();
    _pageLayout->setContentSize(Size(visibleSize.width * 0.95f, visibleSize.height - _titleBanner->getContentSize().height - _navBar->getContentSize().height));
    _pageLayout->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _pageLayout->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - _titleBanner->getContentSize().height));
    addChild(_pageLayout);
    
    _gameHQ = GameHQ::create();
    _gameHQ->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _gameHQ->setPositionType(ui::Widget::PositionType::PERCENT);
    _gameHQ->setPositionPercent(Vec2::ANCHOR_MIDDLE);
    _gameHQ->setVisible(false);
    _gameHQ->setContentSelectedCallback([](HQContentItemObjectRef content, int elementIndex, int rowIndex, const std::string& location){
        ContentOpener::getInstance()->doCarouselContentOpenLogic(content, rowIndex, elementIndex, HQConsts::kGameHQName, location);
    });
    _pageLayout->addChild(_gameHQ);
    
    _videoHQ = VideoHQ::create();
    _videoHQ->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _videoHQ->setPositionType(ui::Widget::PositionType::PERCENT);
    _videoHQ->setPositionPercent(Vec2::ANCHOR_MIDDLE);
    _videoHQ->setVisible(false);
    _videoHQ->setContentSelectedCallback([](HQContentItemObjectRef content, int elementIndex, int rowIndex, const std::string& location){
        ContentOpener::getInstance()->doCarouselContentOpenLogic(content, rowIndex, elementIndex, HQConsts::kVideoHQName, location);
    });
    _videoHQ->setEpisodeSelectorContentSelectedCallback([](HQContentItemObjectRef content, int elementIndex, int rowIndex, const std::string& location){
        ContentOpener::getInstance()->doCarouselContentOpenLogic(content, rowIndex, elementIndex, ConfigStorage::kGroupHQName, location);
    });
    _pageLayout->addChild(_videoHQ);
    
    _oomeeHQ = OomeeHQ::create();
    _oomeeHQ->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _oomeeHQ->setPositionType(ui::Widget::PositionType::PERCENT);
    _oomeeHQ->setPositionPercent(Vec2::ANCHOR_MIDDLE);
    _oomeeHQ->setVisible(false);
    _oomeeHQ->setContentSelectedCallback([](HQContentItemObjectRef content, int elementIndex, int rowIndex, const std::string& location){
        ContentOpener::getInstance()->doCarouselContentOpenLogic(content, rowIndex, elementIndex, HQConsts::kOomeeHQName, location);
    });
    _pageLayout->addChild(_oomeeHQ);
    
    _chatHQ = ChatHQ::create();
    _chatHQ->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _chatHQ->setPositionType(ui::Widget::PositionType::PERCENT);
    _chatHQ->setPositionPercent(Vec2::ANCHOR_MIDDLE);
    _chatHQ->setVisible(false);
    _pageLayout->addChild(_chatHQ);
}

void HQScene::changeToPage(const HQType& page)
{
    _gameHQ->setVisible(page == HQType::GAME);
    _videoHQ->setVisible(page == HQType::VIDEO);
    _oomeeHQ->setVisible(page == HQType::OOMEE);
    _chatHQ->setVisible(page == HQType::CHAT);
    switch(page)
    {
        case HQType::GAME:
            _gameHQ->forceDoLayout();
            _HQPageTitle->setString(_("Games"));
            _activePageName = HQConsts::kGameHQName;
            break;
        case HQType::VIDEO:
            _videoHQ->forceDoLayout();
            _HQPageTitle->setString(_("Videos"));
            _activePageName = HQConsts::kVideoHQName;
            break;
        case HQType::CHAT:
            _chatHQ->forceDoLayout();
            _HQPageTitle->setString(_("Chat"));
            _activePageName = HQConsts::kChatHQName;
            break;
        case HQType::OOMEE:
            _oomeeHQ->forceDoLayout();
            _HQPageTitle->setString(ChildManager::getInstance()->getParentOrChildName());
            _activePageName = HQConsts::kOomeeHQName;
            break;
    }
    HQHistoryManager::getInstance()->addHQToHistoryManager(_activePageName);
}

HQSnapshot HQScene::getHQSnapshot()
{
    HQSnapshot snapshot;
    
    std::map<HQType, float> scrollPositions;
    scrollPositions[HQType::GAME] = _gameHQ->getScrolledPos();
    scrollPositions[HQType::VIDEO] = _videoHQ->getScrolledPos();
    scrollPositions[HQType::CHAT] = _chatHQ->getScrolledPos();
    scrollPositions[HQType::OOMEE] = _oomeeHQ->getScrolledPos();
    snapshot.setScrollPositions(scrollPositions);
    
    std::map<HQType, int> openDropdowns;
    openDropdowns[HQType::GAME] = _gameHQ->getOpenDropdown();
    openDropdowns[HQType::VIDEO] = _videoHQ->getOpenDropdown();
    openDropdowns[HQType::OOMEE] = _oomeeHQ->getOpenDropdown();
    snapshot.setOpenDropDowns(openDropdowns);
    
    HQType openHQ = HQType::GAME;
    if(_activePageName == HQConsts::kVideoHQName)
    {
        openHQ = HQType::VIDEO;
    }
    else if(_activePageName == HQConsts::kOomeeHQName)
    {
        openHQ = HQType::OOMEE;
    }
    else if(_activePageName == HQConsts::kChatHQName)
    {
        openHQ = HQType::CHAT;
    }
    snapshot.setActiveHQ(openHQ);
    
    snapshot.setEpisodeSelectorOpen(_videoHQ->isEpisodePlayerOpen());
    
    return snapshot;
}

void HQScene::setupWithSnapshot(const HQSnapshot& snapshot)
{
    changeToPage(snapshot.getActiveHQ());
    const auto& openDropdowns = snapshot.getOpenDropDowns();
    if(openDropdowns.find(HQType::GAME) != openDropdowns.end())
    {
        _gameHQ->setDropdownOpen(openDropdowns.at(HQType::GAME));
    }
    if(openDropdowns.find(HQType::VIDEO) != openDropdowns.end())
    {
        _videoHQ->setDropdownOpen(openDropdowns.at(HQType::VIDEO));
    }
    if(openDropdowns.find(HQType::OOMEE) != openDropdowns.end())
    {
        if(openDropdowns.at(HQType::OOMEE) > 0)
        {
            _oomeeHQ->setDropdownOpen();
        }
    }
    
    const auto& scrollPositions = snapshot.getScrollPositions();
    if(scrollPositions.find(HQType::GAME) != scrollPositions.end())
    {
        _gameHQ->setScrolledPos(scrollPositions.at(HQType::GAME));
    }
    if(scrollPositions.find(HQType::VIDEO) != scrollPositions.end())
    {
        _videoHQ->setScrolledPos(scrollPositions.at(HQType::VIDEO));
    }
    if(scrollPositions.find(HQType::OOMEE) != scrollPositions.end())
    {
        _oomeeHQ->setScrolledPos(scrollPositions.at(HQType::OOMEE));
    }
    if(scrollPositions.find(HQType::CHAT) != scrollPositions.end())
    {
        _chatHQ->setScrolledPos(scrollPositions.at(HQType::CHAT));
    }
}

NS_AZOOMEE_END
