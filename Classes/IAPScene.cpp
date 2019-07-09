//
//  IAPScene.cpp
//  Azoomee
//
//  Created by Macauley on 09/07/2019.
//

#include "IAPScene.h"

NS_AZOOMEE_BEGIN

bool IAPScene::init()
{
	if(!Super::init())
	{
		return false;
	}
	
	
	
	return true;
}
void IAPScene::onEnter()
{
	Super::onEnter();
}
void IAPScene::onExit()
{
	Super::onExit();
}
void IAPScene::onSizeChanged()
{
	Super::onSizeChanged();
}

NS_AZOOMEE_END
