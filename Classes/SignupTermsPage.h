//
//  SignupTermsPage.h
//  Azoomee
//
//  Created by Macauley on 18/07/2019.
//

#ifndef SignupTermsPage_h
#define SignupTermsPage_h

#include <AzoomeeCommon/Azoomee.h>
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>

NS_AZOOMEE_BEGIN

class SignupTermsPage : public cocos2d::ui::Layout
{
	typedef cocos2d::ui::Layout Super;
	typedef std::function<void(bool, bool, bool)> ButtonCallback;
private:
	cocos2d::ui::Layout* _contentLayout = nullptr;
	
	cocos2d::ui::CheckBox* _over18Checkbox = nullptr;
	cocos2d::ui::CheckBox* _acceptTermsCheckBox = nullptr;
	cocos2d::ui::CheckBox* _acceptMarketingCheckBox = nullptr;
	
	cocos2d::ui::Button* _submitButton = nullptr;
	cocos2d::ui::Text* _backButton = nullptr;
	
	ButtonCallback _submitCallback = nullptr;
	ButtonCallback _backCallback = nullptr;
	
public:
	
	bool init() override;
	void onSizeChanged() override;
	
	void setSubmitCallback(const ButtonCallback& callback);
	void setBackCallback(const ButtonCallback& callback);
	void setOver18Selected(bool selected);
	void setTermsAccepted(bool selected);
	void setMarketingAccepted(bool selected);
	
	CREATE_FUNC(SignupTermsPage);
};

NS_AZOOMEE_END

#endif /* SignupTermsPage_h */
