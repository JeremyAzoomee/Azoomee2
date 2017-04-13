#ifndef __AzoomeeChat_HelloWorldScene__
#define __AzoomeeChat_HelloWorldScene__

#include <cocos/cocos2d.h>

using namespace cocos2d;
using namespace cocos2d::ui;


namespace Azoomee { namespace Chat
{
    
class HelloWorldScene : public cocos2d::Scene
{
    typedef cocos2d::Scene Super;
private:
    
    
public:
    virtual bool init();

    CREATE_FUNC(HelloWorldScene);
};

    
}}

#endif
