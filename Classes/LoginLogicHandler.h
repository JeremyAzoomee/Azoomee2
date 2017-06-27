#ifndef __LOGIN_LOGIC_HANDLER_H__
#define __LOGIN_LOGIC_HANDLER_H__

#include <cocos/cocos2d.h>
#include <AzoomeeCommon/Azoomee.h>

NS_AZOOMEE_BEGIN

class LoginLogicHandler : public cocos2d::Ref
{
public:
    static LoginLogicHandler* getInstance(void);
    virtual ~LoginLogicHandler();
    bool init(void);
    void doLoginLogic();
    void forceNewLogin();
};

NS_AZOOMEE_END

#endif
