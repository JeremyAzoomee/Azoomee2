//
//  SignupEnterEmail.h
//  Azoomee
//
//  Created by Macauley on 17/07/2019.
//

#ifndef SignupEnterEmail_h
#define SignupEnterEmail_h

#include "SignupPage.h"

NS_AZOOMEE_BEGIN

class SignupEnterEmail : public SignupPage
{
	typedef SignupPage Super;
private:
	
	cocos2d::ui::Text* _topHeading = nullptr;
	
public:
	
	bool init() override;
	
	CREATE_FUNC(SignupEnterEmail);
	
	//Editbox Delegate Functions
	void editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string& text) override;
	void editBoxReturn(cocos2d::ui::EditBox* editBox) override;
	void editBoxEditingDidBegin(cocos2d::ui::EditBox* editBox) override;
	void editBoxEditingDidEnd(cocos2d::ui::EditBox* editBox) override;
};

NS_AZOOMEE_END

#endif /* SignupEnterEmail_h */
