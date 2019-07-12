//
//  IAPFooter.h
//  Azoomee
//
//  Created by Macauley on 09/07/2019.
//

#ifndef IAPFooter_h
#define IAPFooter_h

#include <AzoomeeCommon/Azoomee.h>
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>

NS_AZOOMEE_BEGIN

class IAPFooter : public cocos2d::ui::Layout
{
	typedef cocos2d::ui::Layout Super;
private:
	cocos2d::ui::Text* _loginHeader = nullptr;
	cocos2d::ui::Text* _loginButton = nullptr;
	cocos2d::ui::Text* _termsLink = nullptr;
	cocos2d::ui::Text* _privacyNoticeLink = nullptr;
	cocos2d::ui::Text* _privacyPolicyLink = nullptr;
	
	cocos2d::ui::Layout* _divider = nullptr;
	
	cocos2d::Sprite* _bgPattern = nullptr;
	cocos2d::LayerGradient* _bgGradient = nullptr;
	
public:
	bool init() override;
	void onEnter() override;
	void onExit() override;
	void onSizeChanged() override;
	
	
	CREATE_FUNC(IAPFooter);
};

NS_AZOOMEE_END

#endif /* IAPFooter_h */
