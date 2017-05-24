#ifndef AzoomeeCommon_Application_h
#define AzoomeeCommon_Application_h

#include <cocos/cocos2d.h>
#include "Azoomee.h"


NS_AZOOMEE_BEGIN


/**
@brief    The cocos2d Application.

Private inheritance here hides part of interface from Director.
*/
class Application : private cocos2d::Application
{
private:
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    /// Last keyboard notification
    cocos2d::IMEKeyboardNotificationInfo imeNotification;
#endif
    
public:

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

NS_AZOOMEE_END

#endif

