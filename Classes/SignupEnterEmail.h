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
};

NS_AZOOMEE_END

#endif /* SignupEnterEmail_h */
