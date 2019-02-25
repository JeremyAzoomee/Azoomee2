//
//  VodacomOnboardingDetailsLayer.h
//  azoomee2
//
//  Created by Macauley on 10/10/2018.
//
#ifdef VODACOM_BUILD
#ifndef VodacomOnboardingDetailsLayer_h
#define VodacomOnboardingDetailsLayer_h

#include "VodacomOnboardingLayer.h"
#include <cocos/ui/CocosGUI.h>

NS_AZOOMEE_BEGIN

class VodacomOnboardingDetailsLayer : public VodacomOnboardingLayer
{
	typedef VodacomOnboardingLayer Super;
private:
	cocos2d::ui::Layout* _contentLayout = nullptr;
	cocos2d::ui::ImageView* _image = nullptr;
	cocos2d::ui::Layout* _textHolder = nullptr;
	cocos2d::ui::Button* _closeButton = nullptr;
	cocos2d::ui::Button* _addVoucherButton = nullptr;
	cocos2d::ui::Layout* _learnMoreHolder = nullptr;
	
public:
	
	virtual bool init();
	virtual void onEnter();
	
	CREATE_FUNC(VodacomOnboardingDetailsLayer);
};

NS_AZOOMEE_END


#endif /* VodacomOnboardingDetailsLayer_h */
#endif
