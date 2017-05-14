#include "OrientationFunctions.h"

#include <cocos/cocos2d.h>
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "../proj.ios_mac/ios/RootViewController.h"
#include "AppDelegate.h"
#endif


void setOrientationToPortrait(bool portrait)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    RootViewController* viewController =  (RootViewController*)[UIApplication sharedApplication].keyWindow.rootViewController;
    [viewController forcePortrait:portrait];
#endif
}
