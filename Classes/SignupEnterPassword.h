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
};

NS_AZOOMEE_END

#endif /* SignupEnterPassword_h */
