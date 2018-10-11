//
//  VodacomOnboardingErrorLayer.h
//  azoomee2
//
//  Created by Macauley on 10/10/2018.
//

#ifndef VodacomOnboardingErrorLayer_h
#define VodacomOnboardingErrorLayer_h

#include "VodacomOnboardingLayer.h"

NS_AZOOMEE_BEGIN

class VodacomOnboardingErrorLayer : public VodacomOnboardingLayer
{
	typedef VodacomOnboardingLayer Super;
private:
	
public:
	
	virtual bool init();
	virtual void onEnter();
	
	CREATE_FUNC(VodacomOnboardingErrorLayer);
};

NS_AZOOMEE_END

#endif /* VodacomOnboardingErrorLayer_h */
