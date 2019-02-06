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
#include "../SceneManagerScene.h"
#include <AzoomeeCommon/Data/Parent/ParentDataProvider.h>
#include <AzoomeeCommon/Data/Parent/ParentDataParser.h>
#include <AzoomeeCommon/Data/Child/ChildDataParser.h>
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include <AzoomeeCommon/Strings.h>
#include <AzoomeeCommon/API/API.h>
#include <AzoomeeCommon/UI/ModalMessages.h>
#include <AzoomeeCommon/Analytics/AnalyticsSingleton.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool VodacomOnboardingScene::init()
{
	if(!Super::init())
	{
		return false;
	}
	
	LayerColor* bg = LayerColor::create(Color4B::WHITE);
	this->addChild(bg);
	
	return true;
}

void VodacomOnboardingScene::onEnter()
{
	AnalyticsSingleton::getInstance()->vodacomOnboardingFlowStartedEvent();
	_flowData = VodacomOnboardingFlowData::create();
	_flowData->setUserType(ParentDataProvider::getInstance()->isLoggedInParentAnonymous() ? UserType::ANON : UserType::FREE);
	_flowData->pushState(FlowState::EXIT);
	moveToState((_flowData->getUserType() == UserType::FREE) ? FlowState::ADD_VOUCHER : FlowState::DETAILS);
	Super::onEnter();
}

void VodacomOnboardingScene::exitFlow()
{
	AnalyticsSingleton::getInstance()->vodacomOnboardingFlowExitEvent();
	if(_flowData->getUserType() == UserType::FREE || _flowData->getUserType() == UserType::REGISTERED)
	{
		ModalMessages::getInstance()->startLoading();
		HttpRequestCreator* request = API::GetAvailableChildrenRequest(this);
		request->execute();
	}
	else
	{
		Director::getInstance()->replaceScene(SceneManagerScene::createScene(Base));
	}
	
}

void VodacomOnboardingScene::sendEventForStateTransition()
{
	std::string stateString;
	switch(_flowData->getCurrentState())
	{
		case FlowState::EXIT:
			stateString = "Exit";
			break;
		case FlowState::DETAILS:
			stateString = "Details";
			break;
		case FlowState::ADD_VOUCHER:
			stateString = "Add Voucher";
			break;
		case FlowState::REGISTER:
			stateString = "Register";
			break;
		case FlowState::PIN:
			stateString = "PIN";
			break;
		case FlowState::ADD_CHILD:
			stateString = "Add Child";
			break;
		case FlowState::LOGIN:
			stateString = "Login";
			break;
		case FlowState::SUCCESS:
			if(ParentDataProvider::getInstance()->isPaidUser())
			{
				stateString = "Success - voucher redeemed";
			}
			else
			{
				stateString = "Success - no voucher";
			}
			break;
		case FlowState::ERROR:
			stateString = "Error - ";
			switch (_flowData->getErrorType()) {
					
				case ErrorType::NONE:
					stateString += "None";
					break;
				case ErrorType::LOGIN:
					stateString += "Login";
					break;
				case ErrorType::VOUCHER:
					stateString += "Voucher";
					break;
				case ErrorType::RESET:
					stateString += "Password reset";
					break;
				case ErrorType::ALREADY_PREMIUM:
					stateString += "Already premium";
					break;
				case ErrorType::ALREADY_REGISTERED:
					stateString += "Already registered";
					break;
			}
			break;
	}
	
	AnalyticsSingleton::getInstance()->vodacomOnboardingFlowMoveToScreen(stateString);
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
		_flowData->pushState(targetState);
		nextLayer->setFlowData(_flowData);
		nextLayer->setDelegate(this);
		nextLayer->setContentSize(this->getContentSize());
		this->addChild(nextLayer);
		_currentLayer = nextLayer;
		sendEventForStateTransition();
	}
}

void VodacomOnboardingScene::moveToPreviousState()
{
	if(_currentLayer)
	{
		_currentLayer->removeFromParent();
		_currentLayer = nullptr;
	}
	VodacomOnboardingLayer* nextLayer = nullptr;
	_flowData->popState();
	switch (_flowData->getCurrentState()) {
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
		nextLayer->setFlowData(_flowData);
		nextLayer->setDelegate(this);
		nextLayer->setContentSize(this->getContentSize());
		this->addChild(nextLayer);
		_currentLayer = nextLayer;
		sendEventForStateTransition();
	}
}

void VodacomOnboardingScene::onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body)
{
	ModalMessages::getInstance()->stopLoading();
	ChildDataParser::getInstance()->setChildLoggedIn(false);
	ParentDataParser::getInstance()->parseAvailableChildren(body);
	Director::getInstance()->replaceScene(SceneManagerScene::createScene(ChildSelector));
}
void VodacomOnboardingScene::onHttpRequestFailed(const std::string& requestTag, long errorCode)
{
	ModalMessages::getInstance()->stopLoading();
	ChildDataParser::getInstance()->setChildLoggedIn(false);
	Director::getInstance()->replaceScene(SceneManagerScene::createScene(ChildSelector));
}

NS_AZOOMEE_END
