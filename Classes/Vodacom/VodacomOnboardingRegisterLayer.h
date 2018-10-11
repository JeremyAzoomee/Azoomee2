//
//  VodacomOnboardingRegisterLayer.h
//  azoomee2
//
//  Created by Macauley on 10/10/2018.
//

#ifndef VodacomOnboardingRegisterLayer_h
#define VodacomOnboardingRegisterLayer_h

#include "VodacomOnboardingLayer.h"

NS_AZOOMEE_BEGIN

class VodacomOnboardingRegisterLayer : public VodacomOnboardingLayer
{
	typedef VodacomOnboardingLayer Super;
private:
	
public:
	
	virtual bool init();
	virtual void onEnter();
	
	CREATE_FUNC(VodacomOnboardingRegisterLayer);
};

NS_AZOOMEE_END

#endif /* VodacomOnboardingRegisterLayer_h */
