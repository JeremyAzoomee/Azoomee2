#ifndef __LOGIN_LOGIC_HANDLER_H__
#define __LOGIN_LOGIC_HANDLER_H__

#include <cocos/cocos2d.h>

class LoginLogicHandler : public cocos2d::Ref
{
public:
    static LoginLogicHandler* getInstance(void);
    virtual ~LoginLogicHandler();
    bool init(void);
    void doLoginLogic();
    void forceNewLogin();
};

#endif
