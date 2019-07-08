//
//  AgeGate.cpp
//  Azoomee
//
//  Created by Macauley on 08/07/2019.
//

#include "AgeGate.h"

NS_AZOOMEE_BEGIN

bool AgeGate::init()
{
	if(!Super::init())
	{
		return false;
	}
	
	
	
	return true;
}
void AgeGate::onEnter()
{
	Super::onEnter();
}
void AgeGate::onExit()
{
	Super::onExit();
}

void AgeGate::setActionCompletedCallback(const ActionCompletedCallback& callback)
{
	_callback = callback;
}

NS_AZOOMEE_END
