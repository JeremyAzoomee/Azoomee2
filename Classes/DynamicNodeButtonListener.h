//
//  DynamicNodeButtonListener.h
//  azoomee2
//
//  Created by Macauley on 25/09/2017.
//
//

#ifndef DynamicNodeButtonListener_h
#define DynamicNodeButtonListener_h

#include <AzoomeeCommon/Azoomee.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include <AzoomeeCommon/Data/Json.h>
#include "ButtonActionData.h"
#include "AwaitingAdultPinLayer.h"

NS_AZOOMEE_BEGIN



class DynamicNodeButtonListener : public cocos2d::Ref, AwaitingAdultPinLayerDelegate
{
private:
    //constants for button type
    const std::string _kButtonTypeInternal = "internal";
    const std::string _kButtonTypeWeb = "web";
    const std::string _kButtonTypeDeepLink = "deeplink";
    const std::string _kButtonTypeCTATransition = "ctaTransition";
    const std::string _kButtonTypeCTATransitionParams = "ctaTransitionParams";
    const std::string _kButtonTypeCTATransitionGroup = "ctaTransitionGroup";
    
    //constants for button params
    const std::string _kButtonLocationUpgrade = "showUpgrade";
    const std::string _kButtonLocationIAP = "iap";
    const std::string _kButtonLocationSignUp = "signUp";
    const std::string _kButtonLocationAddChild = "addChild";
    
    const std::string _kButtonActionRestorePurchase = "restorePurchase";
    const std::string _kButtonActionStartPayment = "startPayment";
    
    ButtonActionDataRef _buttonAction;
    
    void closeCTAPopup();
    void upgradeButtonPressed();
    void inAppPurchaseButtonPressed();
    void signUp();
    void addChild();
    
public:
    static DynamicNodeButtonListener* getInstance(void);
    virtual ~DynamicNodeButtonListener();
    bool init(void);
    
    void onButtonPressedCallFunc(cocos2d::Ref* button, cocos2d::ui::Widget::TouchEventType evenType, ButtonActionDataRef buttonAction);
    
    // delegate functions
    void AdultPinCancelled(AwaitingAdultPinLayer* layer);
    void AdultPinAccepted(AwaitingAdultPinLayer* layer);
};

NS_AZOOMEE_END

#endif /* DynamicNodeButtonListener_h */
