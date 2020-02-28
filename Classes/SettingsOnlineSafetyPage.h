//
//  SettingsOnlineSafetyPage.h
//  azoomee2
//
//  Created by Macauley on 24/08/2018.
//

#ifndef SettingsOnlineSafetyPage_h
#define SettingsOnlineSafetyPage_h

#include <TinizineCommon/Tinizine.h>
#include <cocos/cocos2d.h>
#include <ui/CocosGUI.h>
#include "SettingsPageHeader.h"
#include "Azoomee.h"

USING_NS_TZ

NS_AZ_BEGIN

class SettingsOnlineSafetyPage : public cocos2d::ui::Layout
{
    typedef cocos2d::ui::Layout Super;
private:
    
    SettingsPageHeader* _headerBanner = nullptr;
    cocos2d::ui::PageView* _onlineSafetyPageView = nullptr;
    cocos2d::ui::Button* _pageTurnButton = nullptr;
    
public:
    
    virtual bool init() override;
    virtual void onEnter() override;
    
    CREATE_FUNC(SettingsOnlineSafetyPage);
};

NS_AZ_END

#endif /* SettingsOnlineSafetyPage_h */
