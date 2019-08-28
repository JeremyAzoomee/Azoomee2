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
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include <AzoomeeCommon/Data/HQDataObject/HQDataObjectManager.h>
#include "FlowDataSingleton.h"
#include "ContentHistoryManager.h"
#include "RewardDisplayHandler.h"
#include "SceneManagerScene.h"
#include "AgeGate.h"
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
    _activePageName = ConfigStorage::kGameHQName;
    
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
    //if(_messagingLayer)
    //{
    //    _messagingLayer->setContentSize(Size(visibleSize.width, 350));
    //}
    if(_HQPageTitle)
    {
        _HQPageTitle->setTextAreaSize(Size(visibleSize.width / 2, 240));
    }
    if(_pageLayout)
    {
        _pageLayout->setContentSize(Size(visibleSize.width, visibleSize.height - _titleBanner->getContentSize().height - _navBar->getContentSize().height));
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
    
    _navBar = NavigationBar::create();
    _navBar->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _navBar->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
    _navBar->setHQSelectedCallback([this](HQType hq){
        
        const HQDataObjectRef &currentObject = HQDataObjectManager::getInstance()->getHQDataObjectForKey(_activePageName);
        
        if(!currentObject->getHqEntitlement())
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
            return;
        }
        
        switch (hq) {
            case HQType::GAME:
            {
                if(_activePageName != ConfigStorage::kGameHQName)
                {
                    HQHistoryManager::getInstance()->addHQToHistoryManager(ConfigStorage::kGameHQName);
                    _gameHQ->setVisible(true);
                    _gameHQ->forceDoLayout();
                    _videoHQ->setVisible(false);
                    _activePageName = ConfigStorage::kGameHQName;
                    _HQPageTitle->setString("Games");
                }
                break;
            }
            case HQType::VIDEO:
            {
                if(_activePageName != ConfigStorage::kVideoHQName)
                {
                    HQHistoryManager::getInstance()->addHQToHistoryManager(ConfigStorage::kVideoHQName);
                    _gameHQ->setVisible(false);
                    _videoHQ->setVisible(true);
                    _videoHQ->forceDoLayout();
                    _activePageName = ConfigStorage::kVideoHQName;
                    _HQPageTitle->setString("Videos");
                }
                break;
            }
            case HQType::CHAT:
            {
                if(!ParentManager::getInstance()->isLoggedInParentAnonymous())
                {
                    Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::ChatEntryPointScene));
                }
                break;
            }
            case HQType::OOMEE:
            {
                if(_activePageName != ConfigStorage::kMeHQName)
                {
                    HQHistoryManager::getInstance()->addHQToHistoryManager(ConfigStorage::kMeHQName);
                    Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::Base));
                    break;
                }
            }
        }
    });
    //_messagingLayer->addChild(_navBar);
    addChild(_navBar, 1);
    
    const Color3B& gradColour = Style::Color::darkIndigo;
    _verticalScrollGradient = LayerGradient::create(Color4B(gradColour.r, gradColour.g, gradColour.b, 0), Color4B(gradColour));
    _verticalScrollGradient->setIgnoreAnchorPointForPosition(false);
    _verticalScrollGradient->setContentSize(Size(2736, 160));
    _verticalScrollGradient->setAnchorPoint(Vec2(0.5f, 0.2f));
    _verticalScrollGradient->setNormalizedPosition(Vec2::ANCHOR_MIDDLE_TOP);
    _navBar->addChild(_verticalScrollGradient, -1);
}

void HQScene::createPageUI()
{

    const Size& visibleSize = Director::getInstance()->getVisibleSize();
    
    _pageLayout = ui::Layout::create();
    _pageLayout->setContentSize(Size(visibleSize.width, visibleSize.height - _titleBanner->getContentSize().height - _navBar->getContentSize().height));
    _pageLayout->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
    _pageLayout->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - _titleBanner->getContentSize().height));
    _pageLayout->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
    _pageLayout->setBackGroundColor(Color3B::MAGENTA);
    addChild(_pageLayout);
    
    _gameHQ = GameHQ::create();
    _gameHQ->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _gameHQ->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _pageLayout->addChild(_gameHQ);
    
    _videoHQ = VideoHQ::create();
    _videoHQ->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _videoHQ->setNormalizedPosition(Vec2::ANCHOR_MIDDLE);
    _videoHQ->setVisible(false);
    _pageLayout->addChild(_videoHQ);
}

//delegate functions
void HQScene::onTutorialStateChanged(const std::string& stateId)
{
    
}

NS_AZOOMEE_END
