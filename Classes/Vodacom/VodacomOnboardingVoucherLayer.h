//
//  VodacomOnboardingVoucherLayer.h
//  azoomee2
//
//  Created by Macauley on 10/10/2018.
//

#ifndef VodacomOnboardingVoucherLayer_h
#define VodacomOnboardingVoucherLayer_h

#include "VodacomOnboardingLayer.h"

NS_AZOOMEE_BEGIN

class VodacomOnboardingVoucherLayer : public VodacomOnboardingLayer
{
	typedef VodacomOnboardingLayer Super;
private:
	
public:
	
	virtual bool init();
	virtual void onEnter();
	
	CREATE_FUNC(VodacomOnboardingVoucherLayer);
};

NS_AZOOMEE_END


#endif /* VodacomOnboardingVoucherLayer_h */
