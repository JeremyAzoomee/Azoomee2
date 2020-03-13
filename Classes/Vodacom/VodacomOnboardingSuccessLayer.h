//
//  VodacomOnboardingSuccessLayer.h
//  azoomee2
//
//  Created by Macauley on 10/10/2018.
//
#ifdef AZOOMEE_VODACOM_BUILD
#ifndef VodacomOnboardingSuccessLayer_h
#define VodacomOnboardingSuccessLayer_h

#include "VodacomOnboardingLayer.h"
#include "../Azoomee.h"

USING_NS_TZ

NS_AZ_BEGIN

class VodacomOnboardingSuccessLayer : public VodacomOnboardingLayer
{
	typedef VodacomOnboardingLayer Super;
private:
	cocos2d::ui::Button* _closeButton = nullptr;
	
public:
	
	virtual bool init();
	virtual void onEnter();
	
	CREATE_FUNC(VodacomOnboardingSuccessLayer);
};

NS_AZ_END


#endif /* VodacomOnboardingSuccessLayer_h */
#endif
