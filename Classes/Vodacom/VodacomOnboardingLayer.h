//
//  VodacomOnboardingLayer.h
//  azoomee2
//
//  Created by Macauley on 10/10/2018.
//

#ifndef VodacomOnboardingLayer_h
#define VodacomOnboardingLayer_h

#include <AzoomeeCommon/Azoomee.h>
#include <cocos/cocos2d.h>
#include "VodacomOnboardingFlowData.h"

NS_AZOOMEE_BEGIN
class VodacomOnboardingLayerDelegate
{
	virtual void moveToState(const FlowState& targetState) = 0;
};

class VodacomOnboardingLayer : public cocos2d::Layer
{
	typedef cocos2d::Layer Super;
private:
	
protected:
	VodacomOnboardingFlowDataRef _flowData = nullptr;
	VodacomOnboardingLayerDelegate* _delegate = nullptr;
	
public:
	
	bool init() override;
	void onEnter() override;
	
	void setFlowData(const VodacomOnboardingFlowDataRef& flowData);
	void setDelegate(VodacomOnboardingLayerDelegate* delegate);
	
};

NS_AZOOMEE_END

#endif /* VodacomOnboardingLayer_h */
