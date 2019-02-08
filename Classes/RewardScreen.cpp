//
//  RewardScreen.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 08/02/2019.
//

#include "RewardScreen.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool RewardScreen::init()
{
	if(!Super::init())
	{
		return false;
	}
	
	this->setContentSize(Director::getInstance()->getWinSize());
	
	return true;
}
void RewardScreen::onEnter()
{
	this->runAction(Sequence::createWithTwoActions(DelayTime::create(_duration), CallFunc::create([this](){
		if(this->_delegate)
		{
			this->_delegate->onAnimationComplete();
		}
	})));
	
	Super::onEnter();
}
void RewardScreen::onExit()
{
	
}
void RewardScreen::update(float deltaT)
{
	Super::update(deltaT);
}

void RewardScreen::setDeleagte(RewardScreenDelegate* delegate)
{
	_delegate = delegate;
}
void RewardScreen::setDuration(float duration)
{
	_duration = duration;
}

NS_AZOOMEE_END
