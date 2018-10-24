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
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include <AzoomeeCommon/Strings.h>
#include <AzoomeeCommon/API/API.h>
#include <AzoomeeCommon/UI/ModalMessages.h>

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
	_flowData = VodacomOnboardingFlowData::create();
	_flowData->setUserType(ParentDataProvider::getInstance()->isLoggedInParentAnonymous() ? UserType::ANON : UserType::FREE);
	_flowData->pushState(FlowState::EXIT);
	moveToState((_flowData->getUserType() == UserType::FREE) ? FlowState::ADD_VOUCHER : FlowState::DETAILS);
	Super::onEnter();
}

void VodacomOnboardingScene::exitFlow()
{
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
	}
}

void VodacomOnboardingScene::onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body)
{
	ModalMessages::getInstance()->stopLoading();
	ParentDataParser::getInstance()->parseAvailableChildren(body);
	Director::getInstance()->replaceScene(SceneManagerScene::createScene(ChildSelector));
}
void VodacomOnboardingScene::onHttpRequestFailed(const std::string& requestTag, long errorCode)
{
	ModalMessages::getInstance()->stopLoading();
	Director::getInstance()->replaceScene(SceneManagerScene::createScene(ChildSelector));
}

NS_AZOOMEE_END
