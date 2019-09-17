//
//  PurchaseCapsule.h
//  Azoomee
//
//  Created by Macauley.Scoffins on 17/09/2019.
//

#ifndef PurchaseCapsule_h
#define PurchaseCapsule_h

#include <AzoomeeCommon/Azoomee.h>
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include <AzoomeeCommon/UI/DynamicText.h>

NS_AZOOMEE_BEGIN

class PurchaseCapsule : public cocos2d::ui::Layout
{
    typedef cocos2d::ui::Layout Super;
public:
    enum UserBillingType {ANON, LAPSED, PAID};

private:
    static const float kDropshadowPadding;
    
    cocos2d::ui::Scale9Sprite* _stencil = nullptr;
    cocos2d::ClippingNode* _clippingNode = nullptr;
    cocos2d::ui::Layout* _purchaseLayout = nullptr;
    cocos2d::ui::Layout* _loginLayout = nullptr;
    DynamicText* _purchaseText = nullptr;
    DynamicText* _loginText = nullptr;
    
    UserBillingType _userType = UserBillingType::ANON;
    
    void setupForCurrentState();
    
public:
    
    bool init() override;
    void onEnter() override;
    void onExit() override;
    void onSizeChanged() override;
    
    void setUserType(UserBillingType userType);
    
    CREATE_FUNC(PurchaseCapsule);
};

NS_AZOOMEE_END

#endif /* PurchaseCapsule_h */
