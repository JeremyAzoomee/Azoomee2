//
//  SignupEnterPin.h
//  Azoomee
//
//  Created by Macauley on 17/07/2019.
//

#ifndef SignupEnterPin_h
#define SignupEnterPin_h

#include "SignupPage.h"

NS_AZOOMEE_BEGIN

class SignupEnterPin : public SignupPage
{
	typedef SignupPage Super;
private:
	
public:
	
	bool init() override;
	
	CREATE_FUNC(SignupEnterPin);
};

NS_AZOOMEE_END

#endif /* SignupEnterPin_h */
