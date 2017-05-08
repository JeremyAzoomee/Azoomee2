#ifndef  __AzoomeeChat_AppDelegate__
#define  __AzoomeeChat_AppDelegate__

#include <cocos/cocos2d.h>

/**
@brief    The cocos2d Application.

Private inheritance here hides part of interface from Director.
*/
class  AppDelegate : private cocos2d::Application
{
public:
    /// Triggered when the window size changes (e.g on orientation changes)
    static const char* EVENT_WINDOW_SIZE_CHANGED;
    
    AppDelegate();
    virtual ~AppDelegate();

    virtual void initGLContextAttrs() override;

    /**
    @brief    Implement Director and Scene init code here.
    */
    virtual bool applicationDidFinishLaunching() override;

    /**
    @brief  Called when the application moves to the background
    */
    virtual void applicationDidEnterBackground() override;

    /**
    @brief  Called when the application reenters the foreground
    */
    virtual void applicationWillEnterForeground() override;
    
    /**
     @brief  This function will be called when the application screen size is changed.
     */
    virtual void applicationScreenSizeChanged(int newWidth, int newHeight) override;
};

#endif

