#ifndef Azoomee_SettingsButton_h
#define Azoomee_SettingsButton_h

#include <TinizineCommon/Tinizine.h>
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include "Azoomee.h"

USING_NS_TZ

NS_AZ_BEGIN

class SettingsButton : public cocos2d::ui::Button
{
	typedef cocos2d::ui::Button Super;
private:
	bool init() override;
	
    
public:
    CREATE_FUNC(SettingsButton);
};

NS_AZ_END

#endif
