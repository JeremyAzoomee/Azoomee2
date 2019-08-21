//
//  HQScene.cpp
//  Azoomee
//
//  Created by Macauley.Scoffins on 21/08/2019.
//

#include "HQScene.h"
#include "HQHistoryManager.h"
#include <AzoomeeCommon/UI/Style.h>
#include <AzoomeeCommon/Data/Child/ChildManager.h>
#include <AzoomeeCommon/Data/Parent/ParentManager.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

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
    
    return true;
}

void HQScene::onEnter()
{
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
    
    if(_titleBannerContent)
    {
        _titleBannerContent->setContentSize(Size(visibleSize.width - 120, 260));
    }
    if(_messagingLayer)
    {
        _messagingLayer->setContentSize(Size(visibleSize.width, 350));
    }
    if(_HQPageTitle)
    {
        _HQPageTitle->setTextAreaSize(Size(visibleSize.width / 2, 240));
    }
    if(_pageLayout)
    {
        _pageLayout->setContentSize(Size(visibleSize.width, visibleSize.height - _titleBanner->getContentSize().height - _navLayer->getContentSize().height));
        _pageLayout->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - _titleBanner->getContentSize().height));
    }
}

void HQScene::createHeaderUI()
{
    const Size& visibleSize = Director::getInstance()->getVisibleSize();
    _isPortrait = visibleSize.width < visibleSize.height;
    
    _titleBanner = ui::Layout::create();
    _titleBanner->setContentSize(Size(2736, 260));
    _titleBanner->setClippingEnabled(true);
    _titleBanner->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _titleBanner->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_TOP);
    addChild(_titleBanner, 1);
    
    _topPattern = ui::ImageView::create("res/decoration/main_pattern_big.png");
    _topPattern->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _topPattern->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_TOP);
    _topPattern->setColor(Style::Color::macaroniAndCheese);
    _titleBanner->addChild(_topPattern);
    
    const Color3B& gradColour = Style::Color::darkIndigo;
    _patternGradient = LayerGradient::create(Color4B(gradColour.r, gradColour.g, gradColour.b, 0), Color4B(gradColour));
    _patternGradient->setIgnoreAnchorPointForPosition(false);
    _patternGradient->setContentSize(_titleBanner->getContentSize());
    _patternGradient->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _patternGradient->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_TOP);
    _topPattern->addChild(_patternGradient);
    
    _titleBannerContent = ui::Layout::create();
    _titleBannerContent->setContentSize(Size(visibleSize.width - 120, 260));
    _titleBannerContent->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _titleBannerContent->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _titleBanner->addChild(_titleBannerContent);
    
    // add coin counter
    _coinDisplay = CoinDisplay::create();
    _coinDisplay->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_RIGHT);
    _coinDisplay->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    _coinDisplay->setAnimate(true);
    _titleBannerContent->addChild(_coinDisplay);
    //show coin counter if they have coins or have completed the shop tutorial
    //_coinDisplay->setVisible(TutorialController::getInstance()->isTutorialCompleted(TutorialController::kFTUShopID) || ChildManager::getInstance()->getLoggedInChild()->getInventory()->getCoins() > 0);
    
    _HQPageTitle = DynamicText::create(_("Games"), Style::Font::PoppinsBold(), 129);
    _HQPageTitle->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    _HQPageTitle->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_LEFT);
    _HQPageTitle->setOverflow(Label::Overflow::SHRINK);
    _HQPageTitle->setTextAreaSize(Size(visibleSize.width / 2, 240));
    _HQPageTitle->setTextColor(Color4B::WHITE);
    _titleBannerContent->addChild(_HQPageTitle);
    
}
void HQScene::createNavigationUI()
{
    const Size& visibleSize = Director::getInstance()->getVisibleSize();
    _isPortrait = visibleSize.width < visibleSize.height;
    
    _messagingLayer = UserTypeMessagingLayer::create();
    _messagingLayer->setContentSize(Size(visibleSize.width, 350));
    _messagingLayer->setPosition(-Vec2(0,350));
    _messagingLayer->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    UserBillingType userType = UserBillingType::ANON;
    if(!ParentManager::getInstance()->isLoggedInParentAnonymous())
    {
        userType = UserBillingType::LAPSED;
        if(ParentManager::getInstance()->isPaidUser())
        {
            userType = UserBillingType::PAID;
        }
    }
    _messagingLayer->setUserType(userType);
    if(userType == UserBillingType::PAID)
    {
        _showingMessagingLayer = false;
        _messagingLayer->setOpacity(0);
    }
    else
    {
        if(HQHistoryManager::getInstance()->getHistorySize() == 1)
        {
            _messagingLayer->runAction(MoveTo::create(1, Vec2(0,0)));
        }
        else
        {
            _messagingLayer->setPosition(Vec2(0,0));
        }
    }
    addChild(_messagingLayer,1);
    
    _navLayer = NavigationLayer::create();
    _navLayer->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_TOP);
    _navLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _messagingLayer->addChild(_navLayer);
}

void HQScene::createPageUI()
{
    const Size& visibleSize = Director::getInstance()->getVisibleSize();
    
    _pageLayout = ui::Layout::create();
    _pageLayout->setContentSize(Size(visibleSize.width, visibleSize.height - _titleBanner->getContentSize().height - _navLayer->getContentSize().height));
    _pageLayout->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _pageLayout->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - _titleBanner->getContentSize().height));
    addChild(_pageLayout);
}

//delegate functions
void HQScene::onTutorialStateChanged(const std::string& stateId)
{
    
}

NS_AZOOMEE_END
