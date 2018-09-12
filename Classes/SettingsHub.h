//
//  SettingsHub.h
//  azoomee2
//
//  Created by Macauley on 22/08/2018.
//

#ifndef SettingsHub_h
#define SettingsHub_h

#include <AzoomeeCommon/Azoomee.h>
#include <AzoomeeCommon/UI/Scene.h>
#include <cocos/cocos2d.h>
#include <ui/CocosGUI.h>
#include "SettingsNavigationButton.h"

NS_AZOOMEE_BEGIN

enum class SettingsPages {KIDS, FRIENDSHIPS, ACCOUNT, ONLINE_SAFETY, SUPPORT};

class SettingsHub : public Azoomee::Scene
{
    typedef Azoomee::Scene Super;
private:
    cocos2d::ui::Layout* _contentLayout = nullptr;
    cocos2d::ui::Layout* _mainBodyLayout = nullptr;
    // titile bar
    cocos2d::ui::Layout* _titleLayout = nullptr;
    cocos2d::ui::Text* _titleText = nullptr;
    cocos2d::ui::Button* _titleBarButton = nullptr;
    // navigation buttons
    cocos2d::ui::Layout* _navigationLayout = nullptr;
    SettingsNavigationButton* _kidsButton = nullptr;
    SettingsNavigationButton* _friendshipsButton = nullptr;
    SettingsNavigationButton* _yourAccountButton = nullptr;
    SettingsNavigationButton* _onlineSafetyButton = nullptr;
    SettingsNavigationButton* _supportButton = nullptr;
    
    bool _inHub = true;
    
    cocos2d::ui::Layout* _activeSettingsPageHolder = nullptr;
    
    void changeToPage(SettingsPages page);
    
public:
    virtual bool init() override;
    virtual void onEnter() override;
    
    virtual void onSizeChanged() override;
    
    CREATE_FUNC(SettingsHub);
};

NS_AZOOMEE_END

#endif /* SettingsHub_h */
