//
//  VodacomMessageBoxExitFlow.h
//  azoomee2
//
//  Created by Macauley on 22/10/2018.
//
#ifdef AZOOMEE_VODACOM_BUILD
#ifndef VodacomMessageBoxExitFlow_h
#define VodacomMessageBoxExitFlow_h

#include "../SettingsMessageBox.h"
#include "../Azoomee.h"

USING_NS_TZ

NS_AZ_BEGIN
enum class ExitFlowState {VOUCHER, ACCOUNT_CREATE};

class VodacomMessageBoxExitFlow : public SettingsMessageBox
{
	typedef SettingsMessageBox Super;
private:
	SettingsMessageBoxButton* _yesButton = nullptr;
	SettingsMessageBoxButton* _noButton = nullptr;
	cocos2d::Label* _heading = nullptr;
	cocos2d::Label* _subHeading = nullptr;
	ExitFlowState _state;
	
public:
	
	virtual void onEnter() override;
	void setState(const ExitFlowState& state);
	
	CREATE_FUNC(VodacomMessageBoxExitFlow);
	
};

NS_AZ_END

#endif /* VodacomMessageBoxExitFlow_h */
#endif
