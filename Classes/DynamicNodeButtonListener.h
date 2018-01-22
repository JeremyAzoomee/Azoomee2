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



class DynamicNodeButtonListener : public cocos2d::Ref
{
private:
    //constants for button type
    const std::string _kButtonTypeInternal = "internal";
    const std::string _kButtonTypeWeb = "web";
    const std::string _kButtonTypeDeepLink = "deeplink";
    const std::string _kButtonTypeCTATransition = "ctaTransition";
    const std::string _kButtonTypeCTATransitionParams = "ctaTransitionParams";
    const std::string _kButtonTypeCTATransitionGroup = "ctaTransitionGroup";
    const std::string _kButtonTypeFlowAction = "flowAction";
    
    //constants for button params
    const std::string _kButtonLocationUpgrade = "showUpgrade";
    
    ButtonActionDataRef _buttonAction;
    
    void closeCTAPopup();
    void upgradeButtonPressed();
    
public:
    static DynamicNodeButtonListener* getInstance(void);
    virtual ~DynamicNodeButtonListener();
    bool init(void);
    
    void onButtonPressedCallFunc(cocos2d::Ref* button, cocos2d::ui::Widget::TouchEventType evenType, ButtonActionDataRef buttonAction);
    
};

NS_AZOOMEE_END

#endif /* DynamicNodeButtonListener_h */
