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

NS_AZOOMEE_BEGIN

class PaymentSuccessScreen : public cocos2d::ui::Layout
{
	typedef cocos2d::ui::Layout Super;
	typedef std::function<void()> ContinueCallback;
private:
	cocos2d::Sprite* _pattern = nullptr;
	cocos2d::LayerGradient* _gradient = nullptr;

	cocos2d::ui::Text* _headerText = nullptr;
	cocos2d::ui::Text* _bodyText = nullptr;
	cocos2d::ui::Button* _continueButton = nullptr;
	
	cocos2d::ui::Text* _loginButton = nullptr;
	cocos2d::ui::Text* _loginHeader = nullptr;
	
	ContinueCallback _callback = nullptr;
	
public:
	
	bool init() override;
	void onSizeChanged() override;
	
	void setContinueCallback(const ContinueCallback& callback);
	
	CREATE_FUNC(PaymentSuccessScreen);
	
	static const std::string kPaymentSuccessScreenName;
};

NS_AZOOMEE_END

#endif /* PaymentSuccessScreen_h */
