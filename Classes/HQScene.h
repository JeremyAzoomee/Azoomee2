//
//  HQScene.h
//  Azoomee
//
//  Created by Macauley.Scoffins on 21/08/2019.
//

#ifndef HQScene_h
#define HQScene_h

#include <AzoomeeCommon/Azoomee.h>
#include <AzoomeeCommon/UI/Scene.h>
#include <AzoomeeCommon/Tutorial/TutorialController.h>
#include <AzoomeeCommon/Data/HQDataObject/HQContentItemObject.h>
#include <AzoomeeCommon/UI/TileSprite.h>
#include "NavigationBar.h"
#include <AzoomeeCommon/UI/DynamicText.h>
#include "SettingsButton.h"
#include "UserTypeMessagingLayer.h"
#include "CoinDisplay.h"
#include "HQPage.h"
#include "GameHQ.h"
#include "VideoHQ.h"
#include "OomeeHQ.h"

NS_AZOOMEE_BEGIN

class HQScene : public Azoomee::Scene, TutorialDelegate
{
    typedef Azoomee::Scene Super;
private:
    static const std::string kTutHandName;
    static const float kTitleBarPadding;
    
    cocos2d::LayerGradient* _verticalScrollGradient = nullptr;
    NavigationBar* _navBar = nullptr;
    //UserTypeMessagingLayer* _messagingLayer = nullptr;
    bool _showingMessagingLayer = true;
    CoinDisplay* _coinDisplay = nullptr;
    cocos2d::EventListenerCustom* _rewardRedeemedListener = nullptr;
    bool _initialised = false;
    bool _isPortrait = false;
    cocos2d::ui::Layout* _titleBanner = nullptr;
    DynamicText* _HQPageTitle = nullptr;
    TileSprite* _topPattern = nullptr;
    cocos2d::LayerGradient* _patternGradient = nullptr;
    cocos2d::ui::Layout* _background = nullptr;
    cocos2d::ui::Layout* _titleBannerContent = nullptr;
    
    cocos2d::ui::Layout* _pageLayout = nullptr;
    
    GameHQ* _gameHQ = nullptr;
    VideoHQ* _videoHQ = nullptr;
    OomeeHQ* _oomeeHQ = nullptr;
    
    std::string _activePageName;
    
    void createHeaderUI();
    void createNavigationUI();
    void createPageUI();
    
    void changeToPage(const HQType& page);
    
public:
    
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;
    virtual void onSizeChanged() override;
    
    CREATE_FUNC(HQScene);
    
    //delegate functions
    virtual void onTutorialStateChanged(const std::string& stateId) override;
};

NS_AZOOMEE_END

#endif /* HQScene_h */
