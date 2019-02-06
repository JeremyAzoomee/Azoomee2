//
//  SettingsMessageBoxTryAgain.h
//  azoomee2
//
//  Created by Macauley on 03/10/2018.
//

#ifndef SettingsMessageBoxTryAgain_h
#define SettingsMessageBoxTryAgain_h

#include <cocos/cocos2d.h>
#include <AzoomeeCommon/Azoomee.h>
#include "SettingsMessageBox.h"

NS_AZOOMEE_BEGIN

class SettingsMessageBoxTryAgain : public SettingsMessageBox
{
	typedef SettingsMessageBox Super;
private:
	SettingsMessageBoxButton* _tryAgainButton = nullptr;
	cocos2d::Label* _heading = nullptr;
	cocos2d::Label* _subHeading = nullptr;
	
	std::string _headingStr = "";
	std::string _subHeadingString = "";
	
public:
	
	virtual void onEnter() override;
	
	void setHeading(const std::string& heading);
	void setSubHeading(const std::string& subHeading);
	
	CREATE_FUNC(SettingsMessageBoxTryAgain);
};

NS_AZOOMEE_END

#endif /* SettingsMessageBoxTryAgain_h */
