//
//  SettingsMessageBoxNotification.h
//  azoomee2
//
//  Created by Macauley on 03/10/2018.
//

#ifndef SettingsMessageBoxNotification_h
#define SettingsMessageBoxNotification_h

#include <cocos/cocos2d.h>
#include <AzoomeeCommon/Azoomee.h>
#include "SettingsMessageBox.h"

NS_AZOOMEE_BEGIN

class SettingsMessageBoxNotification : public SettingsMessageBox
{
	typedef SettingsMessageBox Super;
private:
	SettingsMessageBoxButton* _closeButton = nullptr;
	cocos2d::Label* _heading = nullptr;
	
	std::string _headingStr = "";
	
public:
	
	virtual void onEnter() override;
	
	void setHeading(const std::string& heading);
	
	CREATE_FUNC(SettingsMessageBoxNotification);
};

NS_AZOOMEE_END


#endif /* SettingsMessageBoxNotification_h */
