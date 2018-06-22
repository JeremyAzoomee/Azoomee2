//
//  UserTypeMessagingLayer.h
//  azoomee2
//
//  Created by Macauley on 26/04/2018.
//

#ifndef UserTypeMessagingLayer_h
#define UserTypeMessagingLayer_h

#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include <AzoomeeCommon/Azoomee.h>

NS_AZOOMEE_BEGIN

enum class UserType {ANON, LAPSED, PAID};

class UserTypeMessagingLayer : public cocos2d::Layer
{
    typedef cocos2d::Layer Super;
private:
    cocos2d::ui::Scale9Sprite* _bgSprite = nullptr;
    cocos2d::ui::Button* _signInButton = nullptr;
    cocos2d::ui::Button* _startTrialButton = nullptr;
    cocos2d::ui::Button* _reactivateButton = nullptr;
    cocos2d::Label* _signInLabel = nullptr;
    cocos2d::Label* _startTrialLabel = nullptr;
    cocos2d::Label* _reactivateLabel = nullptr;
    cocos2d::Label* _premiumLabel = nullptr;
    
    UserType _userType;
public:
    
    virtual bool init() override;
    virtual void onEnter() override;
    
    void setUserType(UserType userType);
    
    void repositionElements();
    
    CREATE_FUNC(UserTypeMessagingLayer);
};

NS_AZOOMEE_END

#endif /* UserTypeMessagingLayer_h */
