//
//  SignupEnterPassword.h
//  Azoomee
//
//  Created by Macauley on 17/07/2019.
//

#ifndef SignupEnterPassword_h
#define SignupEnterPassword_h

#include "SignupPage.h"

NS_AZOOMEE_BEGIN

class SignupEnterPassword : public SignupPage
{
	typedef SignupPage Super;
private:
	
public:
	
	bool init() override;
	
	CREATE_FUNC(SignupEnterPassword);
	
	//Editbox Delegate Functions
	void editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string& text) override;
	void editBoxReturn(cocos2d::ui::EditBox* editBox) override;
	void editBoxEditingDidBegin(cocos2d::ui::EditBox* editBox) override;
	void editBoxEditingDidEnd(cocos2d::ui::EditBox* editBox) override;
};

NS_AZOOMEE_END

#endif /* SignupEnterPassword_h */
