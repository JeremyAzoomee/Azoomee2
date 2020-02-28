//
//  IAPFooter.h
//  Azoomee
//
//  Created by Macauley on 09/07/2019.
//

#ifndef IAPFooter_h
#define IAPFooter_h

#include <TinizineCommon/Tinizine.h>
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include <TinizineCommon/UI/DynamicText.h>
#include "Azoomee.h"

USING_NS_TZ

NS_AZ_BEGIN

class IAPFooter : public cocos2d::ui::Layout
{
	typedef cocos2d::ui::Layout Super;
private:
	DynamicText* _loginHeader = nullptr;
	DynamicText* _loginButton = nullptr;
	DynamicText* _termsLink = nullptr;
	DynamicText* _privacyNoticeLink = nullptr;
	DynamicText* _privacyPolicyLink = nullptr;
	
	cocos2d::ui::Layout* _divider = nullptr;
	
	cocos2d::LayerGradient* _bgGradient = nullptr;
	
    void createLoginButton();
    void createTermsLinks();
    void createBackroundElements();
    
public:
	bool init() override;
	void onEnter() override;
	void onExit() override;
	void onSizeChanged() override;
	
	
	CREATE_FUNC(IAPFooter);
};

NS_AZ_END

#endif /* IAPFooter_h */
