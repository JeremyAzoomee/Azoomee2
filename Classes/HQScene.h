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
#include <AzoomeeCommon/UI/DynamicText.h>
#include "NavigationLayer.h"
#include "NavigationBar.h"
#include "SettingsButton.h"
#include "UserTypeMessagingLayer.h"
#include "CoinDisplay.h"

NS_AZOOMEE_BEGIN

class HQScene : public Azoomee::Scene, TutorialDelegate
{
    typedef Azoomee::Scene Super;
private:
    static const std::string kTutHandName;
    
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
    cocos2d::ui::ImageView* _topPattern = nullptr;
    cocos2d::LayerGradient* _patternGradient = nullptr;
    cocos2d::ui::Layout* _background = nullptr;
    cocos2d::ui::Layout* _titleBannerContent = nullptr;
    
    cocos2d::ui::Layout* _pageLayout = nullptr;
    
    void createHeaderUI();
    void createNavigationUI();
    void createPageUI();
    
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
