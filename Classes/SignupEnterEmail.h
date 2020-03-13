//
//  SignupEnterEmail.h
//  Azoomee
//
//  Created by Macauley on 17/07/2019.
//

#ifndef SignupEnterEmail_h
#define SignupEnterEmail_h

#include "SignupPage.h"
#include "Azoomee.h"

USING_NS_TZ

NS_AZ_BEGIN

class SignupEnterEmail : public SignupPage
{
	typedef SignupPage Super;
private:

	
public:
	
	bool init() override;
	
	CREATE_FUNC(SignupEnterEmail);
	
	//Editbox Delegate Functions
	void editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string& text) override;
	void editBoxReturn(cocos2d::ui::EditBox* editBox) override;
	void editBoxEditingDidBegin(cocos2d::ui::EditBox* editBox) override;
	void editBoxEditingDidEnd(cocos2d::ui::EditBox* editBox) override;
};

NS_AZ_END

#endif /* SignupEnterEmail_h */
