//
//  VodacomOnboardingAddChildLayer.h
//  azoomee2
//
//  Created by Macauley on 10/10/2018.
//

#ifndef VodacomOnboardingAddChildLayer_h
#define VodacomOnboardingAddChildLayer_h

#include "VodacomOnboardingLayer.h"

NS_AZOOMEE_BEGIN

class VodacomOnboardingAddChildLayer : public VodacomOnboardingLayer
{
	typedef VodacomOnboardingLayer Super;
private:
	
public:
	
	virtual bool init();
	virtual void onEnter();
	
	CREATE_FUNC(VodacomOnboardingAddChildLayer);
};

NS_AZOOMEE_END


#endif /* VodacomOnboardingAddChildLayer_h */
