//
//  SignupConfirmEmail.h
//  Azoomee
//
//  Created by Macauley on 17/07/2019.
//

#ifndef SignupConfirmEmail_h
#define SignupConfirmEmail_h

#include "SignupPage.h"
#include "Azoomee.h"

USING_NS_TZ

NS_AZ_BEGIN

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

NS_AZ_END


#endif /* SignupConfirmEmail_h */
