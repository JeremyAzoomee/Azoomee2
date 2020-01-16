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
#include <AzoomeeCommon/Data/HQDataObject/HQContentItemObject.h>
#include <AzoomeeCommon/UI/TileSprite.h>
#include "NavigationBar.h"
#include <AzoomeeCommon/UI/DynamicText.h>
#include "SettingsButton.h"
#include "PurchaseCapsule.h"
#include "CoinDisplay.h"
#include "HQPage.h"
#include "GameHQ.h"
#include "VideoHQ.h"
#include "OomeeHQ.h"
#include "ChatHQ.h"
#include "SettingsButton.h"
#include "HQConstants.h"

NS_AZOOMEE_BEGIN

class HQSnapshot
{
private:
    std::map<HQType, float> _scrollPercents;
    std::map<HQType, int> _openDropDowns;
    HQType _activeHQ = HQType::GAME;
    bool _episodeSelectorOpen = false;
    HQContentItemObjectRef _selectedSeries = nullptr;
    
public:
    
    HQSnapshot();
    
    void setScrollPercents(const std::map<HQType, float>& scrollPercents);
    std::map<HQType, float> getScrollPercents() const;
    
    void setOpenDropDowns(const std::map<HQType, int>& openDorpdowns);
    std::map<HQType, int> getOpenDropDowns() const;
    
    void setActiveHQ(const HQType& activeHQ);
    HQType getActiveHQ() const;
    
    void setEpisodeSelectorOpen(bool open);
    bool isEpisodeSelectorOpen() const;
    
    void setSelectedSeries(const HQContentItemObjectRef& series);
    HQContentItemObjectRef getSelectedSeries() const;
};

class HQScene : public Azoomee::Scene
{
    typedef Azoomee::Scene Super;
private:
    static const std::string kTutHandName;
    static const float kTitleBarPadding;
    
    cocos2d::LayerGradient* _verticalScrollGradient = nullptr;
    NavigationBar* _navBar = nullptr;
    PurchaseCapsule* _purchaseCapsule = nullptr;
    bool _showingMessagingLayer = true;
    CoinDisplay* _coinDisplay = nullptr;
    SettingsButton* _settingsButton = nullptr;
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
    ChatHQ* _chatHQ = nullptr;
    
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
    
    HQSnapshot getHQSnapshot();
    
    void setupWithSnapshot(const HQSnapshot& snapshot);
    
    CREATE_FUNC(HQScene);
};

NS_AZOOMEE_END

#endif /* HQScene_h */
