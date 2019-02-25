//
//  VodacomMessageBoxInfo.h
//  azoomee2
//
//  Created by Macauley on 22/10/2018.
//
#ifdef VODACOM_BUILD
#ifndef VodacomMessageBoxInfo_h
#define VodacomMessageBoxInfo_h

#include "../SettingsMessageBox.h"

NS_AZOOMEE_BEGIN

class VodacomMessageBoxInfo : public SettingsMessageBox
{
	typedef SettingsMessageBox Super;
private:
	cocos2d::Label* _heading = nullptr;
	SettingsMessageBoxButton* _okButton = nullptr;
	
	std::string _headingText;
	
public:
	
	void onEnter() override;
	void setHeading(const std::string& heading);
	CREATE_FUNC(VodacomMessageBoxInfo);
};

NS_AZOOMEE_END


#endif /* VodacomMessageBoxInfo_h */
#endif
