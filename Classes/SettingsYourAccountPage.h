//
//  SettingsYourAccountPage.h
//  azoomee2
//
//  Created by Macauley on 28/08/2018.
//

#ifndef SettingsYourAccountPage_h
#define SettingsYourAccountPage_h

#include <AzoomeeCommon/Tinizine.h>
#include <cocos/cocos2d.h>
#include <ui/CocosGUI.h>
#include "SettingsPageHeader.h"
#include "EditAccountLayer.h"
#include "Azoomee.h"

USING_NS_TZ

NS_AZ_BEGIN

class SettingsYourAccountPage : public cocos2d::ui::Layout
{
    typedef cocos2d::ui::Layout Super;
private:
    
    cocos2d::ui::Layout* _footerBanner = nullptr;
    EditAccountLayer* _editAccountLayer = nullptr;
    
public:
    
    virtual bool init() override;
    virtual void onEnter() override;
    
    CREATE_FUNC(SettingsYourAccountPage);
};

NS_AZ_END

#endif /* SettingsYourAccountPage_h */
