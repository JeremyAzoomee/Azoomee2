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
    
    void updateResolution(int newWidth, int newHeight);
    
public:

    virtual void initGLContextAttrs() override;

    /// Implement Director and Scene init code here.
    virtual bool applicationDidFinishLaunching() override;

    /// Called when the application moves to the background
    virtual void applicationDidEnterBackground() override;

    /// Called when the application reenters the foreground
    virtual void applicationWillEnterForeground() override;
    
    /// The screen size has changed
    virtual void applicationScreenSizeChanged(int newWidth, int newHeight) override;
    
    /// The screen size will change over duration (e.g orientation change)
    virtual void applicationScreenSizeWillChange(int newWidth, int newHeight, float duration);
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    /// Called when keyboard on android is shown
    virtual void onVirtualKeyboardShown(bool shown, int height);
#endif
    
    
    /// Orientation mode
    enum Orientation {
        Portrait,
        Landscape,
        Any
    };
    /// Set the orientation of the device
    static void setOrientation(Orientation orientation);
};

NS_AZOOMEE_END

#endif

