//
//  VodacomOnboardingSuccessLayer.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 11/10/2018.
//

#include "VodacomOnboardingSuccessLayer.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool VodacomOnboardingSuccessLayer::init()
{
	if(!Super::init())
	{
		return false;
	}
	
	setLayoutType(ui::Layout::Type::VERTICAL);
	
	return true;
}

void VodacomOnboardingSuccessLayer::onEnter()
{
	Super::onEnter();
}

NS_AZOOMEE_END
