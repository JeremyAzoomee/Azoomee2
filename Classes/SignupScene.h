//
//  SignupScene.h
//  Azoomee
//
//  Created by Macauley on 15/07/2019.
//

#ifndef SignupScene_h
#define SignupScene_h

#include <AzoomeeCommon/Azoomee.h>
#include <AzoomeeCommon/UI/Scene.h>
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include "SignupPage.h"

NS_AZOOMEE_BEGIN

class SignupScene : public Azoomee::Scene
{
	typedef Azoomee::Scene Super;
private:
	
	cocos2d::ui::Text* _titleText = nullptr;
	cocos2d::Sprite* _bgPattern = nullptr;
	cocos2d::LayerGradient* _gradient = nullptr;
	cocos2d::ui::Layout* _bgColour = nullptr;
	cocos2d::DrawNode* _patternHider = nullptr;
	SignupPage* _signupPage = nullptr;
	
public:
	
	bool init() override;
	void onEnter() override;
	void onExit() override;
	void onSizeChanged() override;
	
	CREATE_FUNC(SignupScene);
};

NS_AZOOMEE_END

#endif /* SignupScene_h */
