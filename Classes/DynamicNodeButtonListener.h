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

NS_AZOOMEE_BEGIN

class DynamicNodeButtonListener : public cocos2d::Ref
{
private:
    void closeCTAPopup(float dt);
    void upgradeButtonPressed();
    
public:
    static DynamicNodeButtonListener* getInstance(void);
    virtual ~DynamicNodeButtonListener();
    bool init(void);
    
    void onButtonPressedCallFunc(cocos2d::Ref* button, cocos2d::ui::Widget::TouchEventType evenType, ButtonActionDataRef buttonAction);
};

NS_AZOOMEE_END

#endif /* DynamicNodeButtonListener_h */
