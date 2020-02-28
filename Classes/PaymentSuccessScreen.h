//
//  PaymentSuccessScreen.h
//  Azoomee
//
//  Created by Macauley on 25/07/2019.
//

#ifndef PaymentSuccessScreen_h
#define PaymentSuccessScreen_h

#include <TinizineCommon/Tinizine.h>
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include <TinizineCommon/UI/TextButton.h>
#include <TinizineCommon/UI/DynamicText.h>
#include "Azoomee.h"

USING_NS_TZ

NS_AZ_BEGIN

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

NS_AZ_END

#endif /* PaymentSuccessScreen_h */
