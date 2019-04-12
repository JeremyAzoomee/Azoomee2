#ifndef Azoomee_SettingsButton_h
#define Azoomee_SettingsButton_h

#include <AzoomeeCommon/Azoomee.h>
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>

NS_AZOOMEE_BEGIN

class SettingsButton : public cocos2d::ui::Button
{
	typedef cocos2d::ui::Button Super;
private:
	bool init() override;
	
    
public:
    CREATE_FUNC(SettingsButton);
};

NS_AZOOMEE_END

#endif
