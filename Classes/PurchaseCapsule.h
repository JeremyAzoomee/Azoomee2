//
//  PurchaseCapsule.h
//  Azoomee
//
//  Created by Macauley.Scoffins on 17/09/2019.
//

#ifndef PurchaseCapsule_h
#define PurchaseCapsule_h

#include <AzoomeeCommon/Tinizine.h>
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include <AzoomeeCommon/UI/DynamicText.h>
#include <AzoomeeCommon/Data/Parent/BillingData.h>
#include "Azoomee.h"

USING_NS_TZ

NS_AZ_BEGIN

class PurchaseCapsule : public cocos2d::ui::Layout
{
    typedef cocos2d::ui::Layout Super;
    
private:
    static const float kDropshadowPadding;
    static const float kLoginWidthPercent;
    
    cocos2d::ui::Scale9Sprite* _stencil = nullptr;
    cocos2d::ClippingNode* _clippingNode = nullptr;
    cocos2d::ui::Layout* _purchaseLayout = nullptr;
    cocos2d::ui::Layout* _loginLayout = nullptr;
    DynamicText* _purchaseText = nullptr;
    DynamicText* _loginText = nullptr;
    
    BillingStatus _userType = BillingStatus::ANON;
    
    void setupForCurrentState();
    
public:
    
    bool init() override;
    void onEnter() override;
    void onExit() override;
    void onSizeChanged() override;
    
    void setUserType(BillingStatus userType);
    
    CREATE_FUNC(PurchaseCapsule);
};

NS_AZ_END

#endif /* PurchaseCapsule_h */
