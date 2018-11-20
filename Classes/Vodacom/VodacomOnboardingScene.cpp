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
#include "VodacomOnboardingTnCLayer.h"
#include "VodacomOnboardingDCBWebview.h"
#include "../SceneManagerScene.h"
#include <AzoomeeCommon/Data/Parent/ParentDataProvider.h>
#include <AzoomeeCommon/Data/Parent/ParentDataParser.h>
#include <AzoomeeCommon/Data/Child/ChildDataParser.h>
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
	_flowData->pushState(FlowState::EXIT);
	if(ParentDataProvider::getInstance()->isPaidUser())
	{
		_flowData->setUserType(UserType::REGISTERED);
		moveToState(FlowState::DCB_WEBVIEW);
	}       
	else
	{
		_flowData->setUserType(ParentDataProvider::getInstance()->isLoggedInParentAnonymous() ? UserType::ANON : UserType::FREE);
		moveToState(FlowState::DETAILS);
	}
	Super::onEnter();
}

void VodacomOnboardingScene::exitFlow()
{
	if(ParentDataProvider::getInstance()->isUserLoggedIn())
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
		HttpRequestCreator* request = API::GetVodacomTransactionId(ParentDataProvider::getInstance()->getLoggedInParentId(), this);
		request->execute();
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
	}
}

void VodacomOnboardingScene::onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body)
{
	ModalMessages::getInstance()->stopLoading();
	if(requestTag == API::TagGetAvailableChildren)
	{
		ChildDataParser::getInstance()->setChildLoggedIn(false);
		ParentDataParser::getInstance()->parseAvailableChildren(body);
		if(_flowData->getPurchaseType() == PurchaseType::DCB && _flowData->getTransactionId() != "" && _flowData->getProductId() != "")
		{
			HttpRequestCreator* request = API::UpdateBillingDataRequest(ParentDataProvider::getInstance()->getLoggedInParentId(), this);
			request->execute();
		}
		else
		{
			Director::getInstance()->replaceScene(SceneManagerScene::createScene(ChildSelector));
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
		ParentDataParser::getInstance()->parseParentBillingData(body);
		Director::getInstance()->replaceScene(SceneManagerScene::createScene(ChildSelector));
	}
}
void VodacomOnboardingScene::onHttpRequestFailed(const std::string& requestTag, long errorCode)
{
	ModalMessages::getInstance()->stopLoading();
	if(requestTag == API::TagGetAvailableChildren || requestTag == API::TagUpdateBillingData)
	{
		ChildDataParser::getInstance()->setChildLoggedIn(false);
		Director::getInstance()->replaceScene(SceneManagerScene::createScene(ChildSelector));
	}
}

NS_AZOOMEE_END
