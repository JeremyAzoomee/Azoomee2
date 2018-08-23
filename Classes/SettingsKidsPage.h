//
//  SettingsKidsPage.h
//  azoomee2
//
//  Created by Macauley on 23/08/2018.
//

#ifndef SettingsKidsPage_h
#define SettingsKidsPage_h

#include <AzoomeeCommon/Azoomee.h>
#include <cocos/cocos2d.h>
#include <ui/CocosGUI.h>

NS_AZOOMEE_BEGIN

class SettingsKidsPage : cocos2d::ui::Layout
{
    typedef cocos2d::ui::Layout Super;
private:
    
    cocos2d::ui::ListView* _kidList = nullptr;
    cocos2d::ui::Layout* _headerBanner = nullptr;
    cocos2d::ui::Layout* _footerBanner = nullptr;
    
public:
    
    virtual bool init() override;
    virtual void onEnter() override;
    
    CREATE_FUNC(SettingsKidsPage);
};

NS_AZOOMEE_END

#endif /* SettingsKidsPage_h */
