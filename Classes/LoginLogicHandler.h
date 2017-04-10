#include <cocos/cocos2d.h>

class LoginLogicHandler : public cocos2d::Ref
{
public:
    void doLoginLogic();
    
private:
    bool hasLoginData();
    std::string getLoginName();
    std::string getLoginPassword();
    void emptyLoginData();
};
