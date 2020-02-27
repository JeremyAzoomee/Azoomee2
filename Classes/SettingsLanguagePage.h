//
//  SettingsLanguagePage.h
//  azoomee2
//
//  Created by Macauley on 01/10/2018.
//

#ifndef SettingsLanguagePage_h
#define SettingsLanguagePage_h

#include <TinizineCommon/Tinizine.h>
#include <cocos/cocos2d.h>
#include <ui/CocosGUI.h>
#include "Azoomee.h"

USING_NS_TZ

NS_AZ_BEGIN

class SettingsLanguagePage : public cocos2d::ui::Layout
{
	typedef cocos2d::ui::Layout Super;
private:
	cocos2d::ui::ListView* _languageList = nullptr;
	
public:
	
	virtual bool init() override;
	virtual void onEnter() override;
	virtual void onExit() override;
	
	CREATE_FUNC(SettingsLanguagePage);
};

NS_AZ_END

#endif /* SettingsLanguagePage_h */
