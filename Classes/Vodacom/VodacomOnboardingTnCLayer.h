//
//  VodacomOnboardingTnCLayer.h
//  azoomee2
//
//  Created by Macauley on 08/11/2018.
//

#ifndef VodacomOnboardingTnCLayer_h
#define VodacomOnboardingTnCLayer_h

#include "VodacomOnboardingLayer.h"

NS_AZOOMEE_BEGIN

class VodacomOnboardingTnCLayer : public VodacomOnboardingLayer
{
	typedef VodacomOnboardingLayer Super;
private:
	cocos2d::ui::Button* _closeButton = nullptr;
	cocos2d::ui::Button* _confirmButton = nullptr;
	cocos2d::ui::CheckBox* _over18Checkbox = nullptr;
	cocos2d::ui::CheckBox* _tnCCheckbox = nullptr;
	cocos2d::ui::CheckBox* _marketingCheckbox = nullptr;
	cocos2d::Label* _errorMessage = nullptr;
	
	void onConfirmPressed();
	
public:
	
	virtual bool init() override;
	virtual void onEnter() override;
	
	CREATE_FUNC(VodacomOnboardingTnCLayer);
};

NS_AZOOMEE_END


#endif /* VodacomOnboardingTnCLayer_h */
