#ifndef  __AzoomeeChat_AppDelegate__
#define  __AzoomeeChat_AppDelegate__

#include <cocos/cocos2d.h>
#include <AzoomeeCommon/Application.h>

/**
@brief    The cocos2d Application.

Private inheritance here hides part of interface from Director.
*/
class AppDelegate : public Azoomee::Application
{
    typedef Azoomee::Application Super;
public:

    /**
    @brief    Implement Director and Scene init code here.
    */
    virtual bool applicationDidFinishLaunching() override;
};

#endif

