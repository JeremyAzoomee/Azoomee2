//
//  VodacomOnboardingPinLayer.h
//  azoomee2
//
//  Created by Macauley on 10/10/2018.
//

#ifndef VodacomOnboardingPinLayer_h
#define VodacomOnboardingPinLayer_h

#include "VodacomOnboardingLayer.h"

NS_AZOOMEE_BEGIN

class VodacomOnboardingPinLayer : public VodacomOnboardingLayer
{
	typedef VodacomOnboardingLayer Super;
private:
	
public:
	
	virtual bool init();
	virtual void onEnter();
	
	CREATE_FUNC(VodacomOnboardingPinLayer);
};

NS_AZOOMEE_END


#endif /* VodacomOnboardingPinLayer_h */
