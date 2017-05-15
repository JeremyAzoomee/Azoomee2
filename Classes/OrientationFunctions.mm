#include "OrientationFunctions.h"
#include "../proj.ios_mac/ios/RootViewController.h"


//RootViewController* test =  (RootViewController*)[UIApplication sharedApplication].keyWindow.rootViewController;

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
