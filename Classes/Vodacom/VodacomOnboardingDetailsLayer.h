//
//  VodacomOnboardingDetailsLayer.h
//  azoomee2
//
//  Created by Macauley on 10/10/2018.
//

#ifndef VodacomOnboardingDetailsLayer_h
#define VodacomOnboardingDetailsLayer_h

#include "VodacomOnboardingLayer.h"

NS_AZOOMEE_BEGIN

class VodacomOnboardingDetailsLayer : public VodacomOnboardingLayer
{
	typedef VodacomOnboardingLayer Super;
private:
	
public:
	
	virtual bool init();
	virtual void onEnter();
	
	CREATE_FUNC(VodacomOnboardingDetailsLayer);
};

NS_AZOOMEE_END


#endif /* VodacomOnboardingDetailsLayer_h */
