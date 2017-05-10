#include "OrientationFunctions.h"
#include "../proj.ios_mac/ios/RootViewController.h"
#include "AppDelegate.h"


//RootViewController* test =  (RootViewController*)[UIApplication sharedApplication].keyWindow.rootViewController;

void setOrientationToPortrait()
{
    RootViewController* test =  (RootViewController*)[UIApplication sharedApplication].keyWindow.rootViewController;
    
    [test setOrientationToPortrait];
    ((AppDelegate*)cocos2d::Application::getInstance())->setOrientationToPortrait();
}

void setOrientationToLandscape()
{
    
}
