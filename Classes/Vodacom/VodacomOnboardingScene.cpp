//
//  VodacomOnboardingScene.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 11/10/2018.
//

#include "VodacomOnboardingScene.h"
#include "VodacomOnboardingAddChildLayer.h"
#include "VodacomOnboardingRegisterLayer.h"
#include "VodacomOnboardingVoucherLayer.h"
#include "VodacomOnboardingLoginLayer.h"
#include "VodacomOnboardingSuccessLayer.h"
#include "VodacomOnboardingDetailsLayer.h"
#include "VodacomOnboardingErrorLayer.h"
#include "VodacomOnboardingPinLayer.h"
#include <AzoomeeCommon/Data/Parent/ParentDataProvider.h>
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include <AzoomeeCommon/Strings.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool VodacomOnboardingScene::init()
{
	if(!Super::init())
	{
		return false;
	}
	
	setContentSize(Director::getInstance()->getVisibleSize());
	
	return true;
}

void VodacomOnboardingScene::onEnter()
{
	_flowData = VodacomOnboardingFlowData::create();
	_flowData->setUserType(ParentDataProvider::getInstance()->isLoggedInParentAnonymous() ? UserType::ANON : UserType::FREE);
	_flowData->setCurrentState(FlowState::EXIT);
	_flowData->setPrevState(FlowState::EXIT);
	moveToState((_flowData->getUserType() == UserType::FREE) ? FlowState::ADD_VOUCHER : FlowState::DETAILS);
	Super::onEnter();
}

//delegate Functions
void VodacomOnboardingScene::moveToState(const FlowState& targetState)
{
	if(_currentLayer)
	{
		_currentLayer->removeFromParent();
		_currentLayer = nullptr;
	}
	VodacomOnboardingLayer* nextLayer = nullptr;
	switch (targetState) {
		case FlowState::EXIT:
			exitFlow();
			return;
		case FlowState::DETAILS:
			nextLayer = VodacomOnboardingDetailsLayer::create();
			break;
		case FlowState::ADD_VOUCHER:
			nextLayer = VodacomOnboardingVoucherLayer::create();
			break;
		case FlowState::REGISTER:
			nextLayer = VodacomOnboardingRegisterLayer::create();
			break;
		case FlowState::PIN:
			nextLayer = VodacomOnboardingPinLayer::create();
			break;
		case FlowState::ADD_CHILD:
			nextLayer = VodacomOnboardingAddChildLayer::create();
			break;
		case FlowState::LOGIN:
			nextLayer = VodacomOnboardingLoginLayer::create();
			break;
		case FlowState::SUCCESS:
			nextLayer = VodacomOnboardingSuccessLayer::create();
			break;
		case FlowState::ERROR:
			nextLayer = VodacomOnboardingErrorLayer::create();
			break;
	}
	if(nextLayer)
	{
		_flowData->setPrevState(_flowData->getCurrentState());
		_flowData->setCurrentState(targetState);
		nextLayer->setFlowData(_flowData);
		nextLayer->setDelegate(this);
		nextLayer->setContentSize(this->getContentSize());
		this->addChild(nextLayer);
		_currentLayer = nextLayer;
	}
}

NS_AZOOMEE_END
