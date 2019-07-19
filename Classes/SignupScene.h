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
#include "SignupTermsPage.h"

NS_AZOOMEE_BEGIN

struct SignupData
{
	std::string _email;
	std::string _password;
	std::string _pin;
	bool _acceptMarketing;
};

class SignupScene : public Azoomee::Scene, cocos2d::IMEDelegate
{
	typedef Azoomee::Scene Super;
private:
	static const std::string kEnterEmailPageKey;
	static const std::string kConfirmEmailPageKey;
	static const std::string kEnterPasswordPageKey;
	static const std::string kEnterPinPageKey;
	static const std::string kTermsPageKey;
	
	static const std::map<std::string, cocos2d::Color3B> kPagePatternColours;
	
	SignupData _signupData;
	
	cocos2d::ui::Text* _titleText = nullptr;
	cocos2d::Sprite* _bgPattern = nullptr;
	cocos2d::LayerGradient* _gradient = nullptr;
	cocos2d::ui::Layout* _bgColour = nullptr;
	cocos2d::DrawNode* _patternHider = nullptr;
	cocos2d::ui::Layout* _activePage = nullptr;
	cocos2d::ui::Layout* _nextPage = nullptr;
	cocos2d::Map<std::string,cocos2d::ui::Layout*> _pages;
	
	void changeToPage(const std::string& pageKey);
	
	// - IMEDelegate
	void keyboardWillShow(cocos2d::IMEKeyboardNotificationInfo& info) override;
	void keyboardWillHide(cocos2d::IMEKeyboardNotificationInfo& info) override;
	
public:
	
	bool init() override;
	void onEnter() override;
	void onExit() override;
	void onSizeChanged() override;
	
	CREATE_FUNC(SignupScene);
	
};

NS_AZOOMEE_END

#endif /* SignupScene_h */
