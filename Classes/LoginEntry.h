//
//  LoginEntry.h
//  Azoomee
//
//  Created by Macauley on 23/07/2019.
//

#ifndef LoginEntry_h
#define LoginEntry_h

#include <TinizineCommon/Tinizine.h>
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include <TinizineCommon/UI/TextButton.h>
#include <TinizineCommon/UI/DynamicText.h>
#include "Azoomee.h"

USING_NS_TZ

NS_AZ_BEGIN

enum class LoginEntryState {EMAIL, PASSWORD};

class LoginEntry : public cocos2d::ui::Layout, cocos2d::ui::EditBoxDelegate
{
	typedef cocos2d::ui::Layout Super;
	typedef std::function<void(const std::string&, const std::string&)> LoginConfirmCallback;
	typedef std::function<void(const LoginEntryState&)> ButtonPressedCallback;
protected:
	
	cocos2d::ui::Layout* _inputHolder = nullptr;
	DynamicText* _inputTitle = nullptr;
	cocos2d::ui::Scale9Sprite* _inputBg = nullptr;
	cocos2d::ui::EditBox* _inputBox = nullptr;
	TextButton* _continueButton = nullptr;
	DynamicText* _backButton = nullptr;
	DynamicText* _versionNumber = nullptr;
	
	LoginConfirmCallback _loginConfirmCallback = nullptr;
	ButtonPressedCallback _backCallback = nullptr;
	ButtonPressedCallback _continueCallback = nullptr;
	
	std::string _email;
	std::string _password;
	
	LoginEntryState _state = LoginEntryState::EMAIL;
	
	float _keyboardOffset = 0;
	
	void setContinueButtonEnabled(bool enabled);
	bool isValidInput(const std::string& text, const LoginEntryState& state);
	
public:
	
	bool init() override;
	void onEnter() override;
	void onExit() override;
	void onSizeChanged() override;
	
	void setLoginConfirmCallback(const LoginConfirmCallback& callback);
	void setBackButtonCallback(const ButtonPressedCallback& callback);
	void setContinueButtonCallback(const ButtonPressedCallback& callback);
	
	void setState(const LoginEntryState& state);
	void setEmail(const std::string& email);
	
	void repositionForKeyboardHeight(int height, float duration);
	
	void clearInputText();
	
	CREATE_FUNC(LoginEntry);
	
	//Editbox Delegate Functions
	void editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string& text) override;
	void editBoxReturn(cocos2d::ui::EditBox* editBox) override;
	void editBoxEditingDidBegin(cocos2d::ui::EditBox* editBox) override;
	void editBoxEditingDidEnd(cocos2d::ui::EditBox* editBox) override;
};

NS_AZ_END

#endif /* LoginEntry_h */
