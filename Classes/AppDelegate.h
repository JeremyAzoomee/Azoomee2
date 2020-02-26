#ifndef  _APP_DELEGATE_H_
#define  _APP_DELEGATE_H_

#include <cocos/cocos2d.h>
#include <AzoomeeCommon/Tinizine.h>
#include <AzoomeeCommon/Application.h>

/**
@brief    The cocos2d Application.

Private inheritance here hides part of interface from Director.
*/
class  AppDelegate : private TZ::Application
{
    typedef TZ::Application Super;
public:

    /**
    @brief    Implement Director and Scene init code here.
    @return true    Initialize success, app continue.
    @return false   Initialize failed, app terminate.
    */
    virtual bool applicationDidFinishLaunching() override;

    /**
    @brief  Called when the application moves to the background
    @param  the pointer of the application
    */
    virtual void applicationDidEnterBackground() override;

    /**
    @brief  Called when the application reenters the foreground
    @param  the pointer of the application
    */
    virtual void applicationWillEnterForeground() override;
};

#endif // _APP_DELEGATE_H_

