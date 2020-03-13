//
//  VodacomOnboardingLayer.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 11/10/2018.
//
#ifdef AZOOMEE_VODACOM_BUILD
#include "VodacomOnboardingLayer.h"

using namespace cocos2d;

USING_NS_TZ

NS_AZ_BEGIN

bool VodacomOnboardingLayer::init()
{
	if(!Super::init())
	{
		return false;
	}
	_flowData = VodacomOnboardingFlowData::create();
	_verticalLayout = ui::Layout::create();
	_verticalLayout->setSizeType(SizeType::PERCENT);
	_verticalLayout->setSizePercent(Vec2(1.0f,1.0f));
	_verticalLayout->setLayoutType(Type::VERTICAL);
	this->addChild(_verticalLayout);
	
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

NS_AZ_END
#endif
