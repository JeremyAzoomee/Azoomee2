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
#include <cocos/ui/CocosGUI.h>
#include "VodacomOnboardingFlowData.h"

NS_AZOOMEE_BEGIN

class VodacomOnboardingLayerDelegate
{
public:
	virtual void moveToState(const FlowState& targetState) = 0;
	virtual void moveToPreviousState() = 0;
};

class VodacomOnboardingLayer : public cocos2d::ui::Layout
{
	typedef cocos2d::ui::Layout Super;
private:
	
protected:
	VodacomOnboardingFlowDataRef _flowData = nullptr;
	VodacomOnboardingLayerDelegate* _delegate = nullptr;
	
	cocos2d::ui::Layout* _verticalLayout = nullptr;
	
public:
	
	bool init() override;
	void onEnter() override;
	
	void setFlowData(const VodacomOnboardingFlowDataRef& flowData);
	void setDelegate(VodacomOnboardingLayerDelegate* delegate);
	
};

NS_AZOOMEE_END

#endif /* VodacomOnboardingLayer_h */
