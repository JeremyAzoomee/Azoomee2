//
//  VodacomOnboardingScene.cpp
//  azoomee2-mobile
//
//  Created by Macauley on 11/10/2018.
//
#ifdef AZOOMEE_VODACOM_BUILD
#include "VodacomOnboardingScene.h"
#include "VodacomOnboardingAddChildLayer.h"
#include "VodacomOnboardingRegisterLayer.h"
#include "VodacomOnboardingVoucherLayer.h"
#include "VodacomOnboardingLoginLayer.h"
#include "VodacomOnboardingSuccessLayer.h"
#include "VodacomOnboardingDetailsLayer.h"
#include "VodacomOnboardingErrorLayer.h"
#include "VodacomOnboardingPinLayer.h"
#include "VodacomOnboardingTnCLayer.h"
#include "VodacomOnboardingDCBWebview.h"
#include "../SceneManagerScene.h"
#include <AzoomeeCommon/Data/Parent/UserAccountManager.h>
#include <AzoomeeCommon/Data/Child/ChildManager.h>
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include <AzoomeeCommon/Utils/LocaleManager.h>
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
	_flowData->pushState(FlowState::EXIT);
	if(UserAccountManager::getInstance()->isPaidUser())
	{
		_flowData->setUserType(UserType::REGISTERED);
		moveToState(FlowState::DCB_WEBVIEW);
	}       
	else
	{
		_flowData->setUserType(UserAccountManager::getInstance()->isLoggedInParentAnonymous() ? UserType::ANON : UserType::FREE);
		moveToState(FlowState::DETAILS);
	}
	Super::onEnter();
}

void VodacomOnboardingScene::exitFlow()
{
	AnalyticsSingleton::getInstance()->vodacomOnboardingFlowExitEvent();
	if(UserAccountManager::getInstance()->isUserLoggedIn())
	{
		ModalMessages::getInstance()->startLoading();
		HttpRequestCreator* request = API::GetAvailableChildrenRequest(this);
		request->execute();
	}
	else
	{
		Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::Base));
	}
	
}

void VodacomOnboardingScene::moveToStateDCBProductSelected(const std::string& productId)
{
	_flowData->setProductId(productId);
	if(_flowData->getUserType() != UserType::FREE)
	{
		moveToState(FlowState::TERMS);
	}
	else
	{
		ModalMessages::getInstance()->startLoading();
		HttpRequestCreator* request = API::GetVodacomTransactionId(UserAccountManager::getInstance()->getLoggedInParentId(), this);
		request->execute();
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
			if(UserAccountManager::getInstance()->isPaidUser())
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
				case ErrorType::GENERIC:
					stateString += "Generic";
					break;
			}
			break;
		case FlowState::TERMS:
			stateString = "Terms";
			break;
		case FlowState::DCB_WEBVIEW:
			stateString = "DCB webview";
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
		case FlowState::TERMS:
			nextLayer = VodacomOnboardingTnCLayer::create();
			break;
		case FlowState::DCB_WEBVIEW:
			nextLayer = VodacomOnboardingDCBWebview::create();
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
		case FlowState::TERMS:
			nextLayer = VodacomOnboardingTnCLayer::create();
			break;
		case FlowState::DCB_WEBVIEW:
			nextLayer = VodacomOnboardingDCBWebview::create();
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
	if(requestTag == API::TagGetAvailableChildren)
	{
		UserAccountManager::getInstance()->parseAvailableChildren(body);
		if(_flowData->getDCBComplete())
		{
			ModalMessages::getInstance()->startLoading();
			this->runAction(Sequence::createWithTwoActions(DelayTime::create(4.0f), CallFunc::create([this](){
				HttpRequestCreator* request = API::UpdateBillingDataRequest(UserAccountManager::getInstance()->getLoggedInParentId(), this);
				request->execute();
			})));
		}
		else
		{
			ChildManager::getInstance()->setChildLoggedIn(false);
			Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::ChildSelector));
		}
	}
	else if(requestTag == API::TagGetVodacomTransactionId)
	{
		rapidjson::Document data;
		data.Parse(body.c_str());
		if(data.HasParseError())
		{
			return;
		}
		_flowData->setTransactionId(getStringFromJson("id", data));
		moveToState(FlowState::DCB_WEBVIEW);
	}
	else if(requestTag == API::TagUpdateBillingData)
	{
		ChildManager::getInstance()->setChildLoggedIn(false);
		UserAccountManager::getInstance()->parseParentBillingData(body);
		Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::ChildSelector));
	}
}
void VodacomOnboardingScene::onHttpRequestFailed(const std::string& requestTag, long errorCode)
{
	ModalMessages::getInstance()->stopLoading();
	if(requestTag == API::TagGetAvailableChildren || requestTag == API::TagUpdateBillingData)
	{
		ChildManager::getInstance()->setChildLoggedIn(false);
		Director::getInstance()->replaceScene(SceneManagerScene::createScene(SceneNameEnum::ChildSelector));
	}
}

NS_AZOOMEE_END
#endif
