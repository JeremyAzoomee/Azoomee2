//
//  PaymentSuccessScreen.h
//  Azoomee
//
//  Created by Macauley on 25/07/2019.
//

#ifndef PaymentSuccessScreen_h
#define PaymentSuccessScreen_h

#include <AzoomeeCommon/Azoomee.h>
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include <AzoomeeCommon/UI/TextButton.h>
#include <AzoomeeCommon/UI/DynamicText.h>

NS_AZOOMEE_BEGIN

class PaymentSuccessScreen : public cocos2d::ui::Layout
{
	typedef cocos2d::ui::Layout Super;
	typedef std::function<void()> ContinueCallback;
private:
	cocos2d::Sprite* _pattern = nullptr;
	cocos2d::LayerGradient* _gradient = nullptr;

	DynamicText* _headerText = nullptr;
	DynamicText* _bodyText = nullptr;
	TextButton* _continueButton = nullptr;
	
	DynamicText* _loginButton = nullptr;
	DynamicText* _loginHeader = nullptr;
	
	ContinueCallback _callback = nullptr;
	
    void createBackground();
    void createBody();
    void createLoginButton();
    
public:
	
	bool init() override;
	void onSizeChanged() override;
	
	void setContinueCallback(const ContinueCallback& callback);
	
	CREATE_FUNC(PaymentSuccessScreen);
	
	static const std::string kPaymentSuccessScreenName;
};

NS_AZOOMEE_END

#endif /* PaymentSuccessScreen_h */
