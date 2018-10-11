//
//  VodacomOnboardingLoginLayer.h
//  azoomee2
//
//  Created by Macauley on 10/10/2018.
//

#ifndef VodacomOnboardingLoginLayer_h
#define VodacomOnboardingLoginLayer_h

#include "VodacomOnboardingLayer.h"

NS_AZOOMEE_BEGIN

class VodacomOnboardingLoginLayer : public VodacomOnboardingLayer
{
	typedef VodacomOnboardingLayer Super;
private:
	
public:
	
	virtual bool init();
	virtual void onEnter();
	
	CREATE_FUNC(VodacomOnboardingLoginLayer);
};

NS_AZOOMEE_END


#endif /* VodacomOnboardingLoginLayer_h */
