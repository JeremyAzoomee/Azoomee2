//
//  OfflineScene.h
//  Azoomee
//
//  Created by Macauley.Scoffins on 20/11/2019.
//

#ifndef OfflineScene_h
#define OfflineScene_h

#include <AzoomeeCommon/Azoomee.h>
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include <AzoomeeCommon/UI/Scene.h>
#include <AzoomeeCommon/Utils/LocaleManager.h>
#include <AzoomeeCommon/UI/DynamicText.h>
#include <AzoomeeCommon/UI/TextButton.h>
#include <AzoomeeCommon/UI/RoundedRectSprite.h>
#include "FeaturedTile.h"
#include "OfflineChecker.h"

NS_AZOOMEE_BEGIN

class OfflineScene : public Azoomee::Scene, OfflineCheckerDelegate
{
    typedef Azoomee::Scene Super;
private:
    
    static const cocos2d::Size kTileSize;
    static const float kPadding;
    
    RoundedRectSprite* _bgPattern = nullptr;
    cocos2d::LayerGradient* _bgGradient = nullptr;
    
    cocos2d::ui::ImageView* _offlineLogo = nullptr;
    DynamicText* _offlineText = nullptr;
    DynamicText* _checkConnectionText = nullptr;
    TextButton* _retryButton = nullptr;
    DynamicText* _gamesSmallHeading = nullptr;
    DynamicText* _playTheseText = nullptr;
    cocos2d::ui::Layout* _offlineMsgLayout = nullptr;
    cocos2d::ui::Layout* _gamesLayout = nullptr;
    cocos2d::ui::ListView* _gamesList = nullptr;
    
    void populateOfflineGamesList();
    
public:
    bool init() override;
    void onEnter() override;
    void onExit() override;
    void onSizeChanged() override;
    
    CREATE_FUNC(OfflineScene);
    
    // delegate functions
    void connectivityStateChanged(bool online) override;
};

NS_AZOOMEE_END

#endif /* OfflineScene_h */
