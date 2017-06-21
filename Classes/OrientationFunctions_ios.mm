#include "OrientationFunctions_ios.h"
#include <AzoomeeCommon/Platform/iOS/AzoomeeViewController.h>

void setOrientationToPortrait()
{
    AzoomeeViewController* rootViewController =  (AzoomeeViewController*)[UIApplication sharedApplication].keyWindow.rootViewController;
    [rootViewController setOrientationToPortrait];
}

void setOrientationToLandscape()
{
    AzoomeeViewController* rootViewController =  (AzoomeeViewController*)[UIApplication sharedApplication].keyWindow.rootViewController;
    [rootViewController setOrientationToLandscape];
}
