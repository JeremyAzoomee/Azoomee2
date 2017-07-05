#include "OrientationFunctions_ios.h"
#include <AzoomeeCommon/Platform/iOS/AzoomeeViewController.h>

NS_AZOOMEE_BEGIN

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

NS_AZOOMEE_END
