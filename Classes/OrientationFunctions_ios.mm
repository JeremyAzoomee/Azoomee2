#include "OrientationFunctions_ios.h"
#include "../proj.ios_mac/ios/RootViewController.h"

void setOrientationToPortrait()
{
    RootViewController* rootViewController =  (RootViewController*)[UIApplication sharedApplication].keyWindow.rootViewController;
    [rootViewController setOrientationToPortrait];
}

void setOrientationToLandscape()
{
    RootViewController* rootViewController =  (RootViewController*)[UIApplication sharedApplication].keyWindow.rootViewController;
    [rootViewController setOrientationToLandscape];
}
