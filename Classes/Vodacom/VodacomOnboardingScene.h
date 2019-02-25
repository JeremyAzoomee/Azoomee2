//
//  VodacomOnboardingScene.h
//  azoomee2
//
//  Created by Macauley on 10/10/2018.
//
#ifdef VODACOM_BUILD
#ifndef VodacomOnboardingScene_h
#define VodacomOnboardingScene_h

#include <AzoomeeCommon/Azoomee.h>
#include <AzoomeeCommon/UI/Scene.h>
#include <cocos/cocos2d.h>
#include "VodacomOnboardingFlowData.h"
#include "VodacomOnboardingLayer.h"
#include <AzoomeeCommon/API/HttpRequestCreator.h>

NS_AZOOMEE_BEGIN

class VodacomOnboardingScene : public Azoomee::Scene, VodacomOnboardingLayerDelegate, HttpRequestCreatorResponseDelegate
{
	typedef Azoomee::Scene Super;
private:
	VodacomOnboardingFlowDataRef _flowData = nullptr;
	VodacomOnboardingLayer* _currentLayer = nullptr;
	
	void exitFlow();
	void sendEventForStateTransition();
	
public:
	
	virtual bool init() override;
	virtual void onEnter() override;
	
	CREATE_FUNC(VodacomOnboardingScene);
	
	void moveToStateDCBProductSelected(const std::string& productId);
	
	//delegate Functions
	void moveToState(const FlowState& targetState) override;
	void moveToPreviousState() override;
	
	void onHttpRequestSuccess(const std::string& requestTag, const std::string& headers, const std::string& body) override;
	void onHttpRequestFailed(const std::string& requestTag, long errorCode) override;
};

NS_AZOOMEE_END

#endif /* VodacomOnboardingScene_h */
#endif
