//
//  VodacomOnboardingErrorLayer.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 11/10/2018.
//

#include "VodacomOnboardingErrorLayer.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool VodacomOnboardingErrorLayer::init()
{
	if(!Super::init())
	{
		return false;
	}
	
	setLayoutType(ui::Layout::Type::VERTICAL);
	
	return true;
}

void VodacomOnboardingErrorLayer::onEnter()
{
	Super::onEnter();
}

NS_AZOOMEE_END
