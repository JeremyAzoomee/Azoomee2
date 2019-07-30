//
//  SignupPage.h
//  Azoomee
//
//  Created by Macauley on 15/07/2019.
//

#ifndef SignupPage_h
#define SignupPage_h

#include <AzoomeeCommon/Azoomee.h>
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include "ProgressBar.h"

NS_AZOOMEE_BEGIN

class SignupPage : public cocos2d::ui::Layout, cocos2d::ui::EditBoxDelegate
{
	typedef cocos2d::ui::Layout Super;
	typedef std::function<void(const std::string&)> ButtonCallback;
protected:
	
	cocos2d::ui::Layout* _inputHolder = nullptr;
	cocos2d::ui::Text* _topHeading = nullptr;
	cocos2d::ui::Text* _inputTitle = nullptr;
	cocos2d::ui::Scale9Sprite* _inputBg = nullptr;
	cocos2d::ui::EditBox* _inputBox = nullptr;
	cocos2d::ui::Button* _continueButton = nullptr;
	cocos2d::ui::Text* _backButton = nullptr;
	cocos2d::ui::Text* _termsLink = nullptr;
	cocos2d::ui::Text* _privacyPolicyLink = nullptr;
	cocos2d::ui::Text* _progressText = nullptr;
	ProgressBar* _progressBar = nullptr;
	
	ButtonCallback _continueCallback = nullptr;
	ButtonCallback _backCallback = nullptr;
	
	float _keyboardOffset = 0;
	
	void setContinueButtonEnabled(bool enabled);
	
public:
	
	bool init() override;
	void onEnter() override;
	void onExit() override;
	void onSizeChanged() override;
	
	void setContinueCallback(const ButtonCallback& callback);
	void setBackCallback(const ButtonCallback& callback);
	
	void repositionForKeyboardHeight(int height, float duration);
	float getMoveDistanceForKeyboardHeight(int height);
	
	void clearInputText();
	
	CREATE_FUNC(SignupPage);
	
	//Editbox Delegate Functions
	void editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string& text) override;
	void editBoxReturn(cocos2d::ui::EditBox* editBox) override;
	void editBoxEditingDidEndWithAction(cocos2d::ui::EditBox* editBox, EditBoxEndAction action) override;
	void editBoxEditingDidBegin(cocos2d::ui::EditBox* editBox) override;
	void editBoxEditingDidEnd(cocos2d::ui::EditBox* editBox) override;
};

NS_AZOOMEE_END

#endif /* SignupPage_h */
