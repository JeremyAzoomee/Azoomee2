#ifndef __LOGIN_CONTROLLER_H__
#define __LOGIN_CONTROLLER_H__

#include <cocos/cocos2d.h>
#include <AzoomeeCommon/Azoomee.h>

NS_AZOOMEE_BEGIN

enum class LoginOrigin {LOGOUT, IAP_PAYWALL, SIGNUP, HQ};

class LoginController : public cocos2d::Ref
{
private:
    LoginOrigin _origin = LoginOrigin::LOGOUT;
    
public:
    static LoginController* getInstance(void);
    virtual ~LoginController();
    bool init(void);
    void doLoginLogic();
    void forceNewLogin(const LoginOrigin& origin = LoginOrigin::LOGOUT);
    
    void handleLoginSuccess();
    void handleGetChildrenSuccess();
    void handleChildLoginSuccess();
    
    void setLoginOrigin(const LoginOrigin& origin);
    LoginOrigin getOrigin() const;
};

NS_AZOOMEE_END

#endif
