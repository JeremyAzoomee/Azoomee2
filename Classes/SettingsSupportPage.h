//
//  SettingsSupportPage.h
//  azoomee2
//
//  Created by Macauley on 23/08/2018.
//

#ifndef SettingsSupportPage_h
#define SettingsSupportPage_h

#include <AzoomeeCommon/Azoomee.h>
#include <cocos/cocos2d.h>
#include <ui/CocosGUI.h>
#include "SettingsPageHeader.h"

NS_AZOOMEE_BEGIN

class SettingsSupportPage : cocos2d::ui::Layout
{
    typedef cocos2d::ui::Layout Super;
private:
    
    SettingsPageHeader* _headerBanner = nullptr;
    cocos2d::ui::Layout* _supportBox = nullptr;
    
public:
    
    virtual bool init() override;
    virtual void onEnter() override;
    
    CREATE_FUNC(SettingsSupportPage);
};

NS_AZOOMEE_END

#endif /* SettingsSupportPage_h */
