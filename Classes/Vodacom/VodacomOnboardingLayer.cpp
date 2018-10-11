//
//  VodacomOnboardingLayer.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 11/10/2018.
//

#include "VodacomOnboardingLayer.h"

NS_AZOOMEE_BEGIN

bool VodacomOnboardingLayer::init()
{
	if(!Super::init())
	{
		return false;
	}
	
	return true;
}
void VodacomOnboardingLayer::onEnter()
{
	Super::onEnter();
}

void VodacomOnboardingLayer::setFlowData(const VodacomOnboardingFlowDataRef& flowData)
{
	_flowData = flowData;
}
void VodacomOnboardingLayer::setDelegate(VodacomOnboardingLayerDelegate* delegate)
{
	_delegate = delegate;
}

NS_AZOOMEE_END
