//
//  SignupConfirmEmail.h
//  Azoomee
//
//  Created by Macauley on 17/07/2019.
//

#ifndef SignupConfirmEmail_h
#define SignupConfirmEmail_h

#include "SignupPage.h"

NS_AZOOMEE_BEGIN

class SignupConfirmEmail : public SignupPage
{
	typedef SignupPage Super;
private:
	
	cocos2d::ui::Text* _email = nullptr;
	
public:
	
	bool init() override;
	
	void setEmail(const std::string& email);
	
	CREATE_FUNC(SignupConfirmEmail);
};

NS_AZOOMEE_END


#endif /* SignupConfirmEmail_h */
