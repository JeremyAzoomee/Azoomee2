#ifndef  __AzoomeeChat_AppDelegate__
#define  __AzoomeeChat_AppDelegate__

#include <cocos/cocos2d.h>

/**
@brief    The cocos2d Application.

Private inheritance here hides part of interface from Director.
*/
class AppDelegate : private cocos2d::Application
{
private:
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    /// Last keyboard notification
    cocos2d::IMEKeyboardNotificationInfo imeNotification;
#endif
    
public:
    
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
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    /// Called when keyboard on android is shown
    void onVirtualKeyboardShown(bool shown, int height);
#endif
};

#endif

