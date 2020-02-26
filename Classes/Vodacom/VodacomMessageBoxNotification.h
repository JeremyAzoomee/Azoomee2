//
//  VodacomMessageBoxNotification.h
//  azoomee2
//
//  Created by Macauley on 22/10/2018.
//
#ifdef AZOOMEE_VODACOM_BUILD
#ifndef VodacomMessageBoxNotification_h
#define VodacomMessageBoxNotification_h

#include "../SettingsMessageBox.h"
#include "../Azoomee.h"

USING_NS_TZ

NS_AZ_BEGIN

class VodacomMessageBoxNotification : public SettingsMessageBox
{
	typedef SettingsMessageBox Super;
private:
	cocos2d::Label* _heading = nullptr;
	cocos2d::Sprite* _oomee = nullptr;
	
	std::string _headingText;
	
public:
	
	void onEnter() override;
	void setHeading(const std::string& heading);
	CREATE_FUNC(VodacomMessageBoxNotification);
};

NS_AZ_END

#endif /* VodacomMessageBoxNotification_h */
#endif
