#ifndef __LOGIN_LOGIC_HANDLER_H__
#define __LOGIN_LOGIC_HANDLER_H__

#include <cocos/cocos2d.h>
#include <AzoomeeCommon/Azoomee.h>

NS_AZOOMEE_BEGIN

enum class LoginOrigin {LOGOUT, IAP_PAYWALL, SIGNUP, HQ};

class LoginLogicHandler : public cocos2d::Ref
{
private:
    LoginOrigin _origin = LoginOrigin::LOGOUT;
    
public:
    static LoginLogicHandler* getInstance(void);
    virtual ~LoginLogicHandler();
    bool init(void);
    void doLoginLogic();
    void forceNewLogin(const LoginOrigin& origin = LoginOrigin::LOGOUT);
    
    void setLoginOrigin(const LoginOrigin& origin);
    LoginOrigin getOrigin() const;
};

NS_AZOOMEE_END

#endif
