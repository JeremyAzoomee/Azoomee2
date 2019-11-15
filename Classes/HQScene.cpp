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
#include <AzoomeeCommon/Data/Parent/ParentManager.h>
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include <AzoomeeCommon/Data/HQDataObject/HQDataObjectManager.h>
#include "FlowDataSingleton.h"
#include "ContentHistoryManager.h"
#include "RewardDisplayHandler.h"
#include "SceneManagerScene.h"
#include "AgeGate.h"
#include "ContentOpener.h"


using namespace cocos2d;

NS_AZOOMEE_BEGIN

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
    _navBar->toggleHQSelected(_activePageName);

	_rewardRedeemedListener = EventListenerCustom::create(RewardDisplayHandler::kRewardRedeemedEventKey, [this](EventCustom* event){
		if(!_coinDisplay->isVisible())
		{
			_coinDisplay->setVisible(TutorialController::getInstance()->isTutorialCompleted(TutorialController::kFTUShopID) || ChildManager::getInstance()->getLoggedInChild()->getInventory()->getCoins() > 0);
		}
	});
	_eventDispatcher->addEventListenerWithSceneGraphPriority(_rewardRedeemedListener, this);
	
	TutorialController::getInstance()->registerDelegate(this);
	if(TutorialController::getInstance()->isTutorialActive())
	{
		onTutorialStateChanged(TutorialController::getInstance()->getCurrentState());
	}
    
	ContentHistoryManager::getInstance()->setReturnedFromContent(false);
    HQHistoryManager::getInstance()->addHQToHistoryManager(_activePageName);
    
    Super::onEnter();
}

void HQScene::onExit()
{
    Super::onExit();
}

void HQScene::onSizeChanged()
{
    Super::onSizeChanged();
    const Size& visibleSize = Director::getInstance()->getVisibleSize();
    _isPortrait = visibleSize.width < visibleSize.height;
    const Rect& safeAreaRect = Director::getInstance()->getSafeAreaRect();
    const float safeZonePadding = visibleSize.height - (safeAreaRect.origin.y + safeAreaRect.size.height);
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
    //show coin counter if they have coins or have completed the shop tutorial
    //_coinDisplay->setVisible(TutorialController::getInstance()->isTutorialCompleted(TutorialController::kFTUShopID) || ChildManager::getInstance()->getLoggedInChild()->getInventory()->getCoins() > 0);
    
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
    const BillingDataRef& billingData = ParentManager::getInstance()->getBillingData();
    BillingStatus billingStatus = BillingStatus::ANON;
    if(billingData)
    {
        billingStatus = billingData->getBillingStatus();
    }
    else if(ParentManager::getInstance()->isUserLoggedIn())
    {
        billingStatus = BillingStatus::FREE_REGISTERED;
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
        ContentOpener::getInstance()->doCarouselContentOpenLogic(content, rowIndex, elementIndex, ConfigStorage::kGameHQName, location);
    });
    _pageLayout->addChild(_gameHQ);
    
    _videoHQ = VideoHQ::create();
    _videoHQ->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _videoHQ->setPositionType(ui::Widget::PositionType::PERCENT);
    _videoHQ->setPositionPercent(Vec2::ANCHOR_MIDDLE);
    _videoHQ->setVisible(false);
    _videoHQ->setContentSelectedCallback([](HQContentItemObjectRef content, int elementIndex, int rowIndex, const std::string& location){
        ContentOpener::getInstance()->doCarouselContentOpenLogic(content, rowIndex, elementIndex, ConfigStorage::kVideoHQName, location);
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
        ContentOpener::getInstance()->doCarouselContentOpenLogic(content, rowIndex, elementIndex, ConfigStorage::kMeHQName, location);
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
            _activePageName = ConfigStorage::kGameHQName;
            break;
        case HQType::VIDEO:
            _videoHQ->forceDoLayout();
            _HQPageTitle->setString(_("Videos"));
            _activePageName = ConfigStorage::kVideoHQName;
            break;
        case HQType::CHAT:
            _chatHQ->forceDoLayout();
            _HQPageTitle->setString(_("Chat"));
            _activePageName = ConfigStorage::kChatHQName;
            break;
        case HQType::OOMEE:
            _oomeeHQ->forceDoLayout();
            _HQPageTitle->setString(ChildManager::getInstance()->getParentOrChildName());
            _activePageName = ConfigStorage::kMeHQName;
            break;
    }
    HQHistoryManager::getInstance()->addHQToHistoryManager(_activePageName);
}

//delegate functions
void HQScene::onTutorialStateChanged(const std::string& stateId)
{
    
}

NS_AZOOMEE_END
