/****************************************************************************
 Copyright (c) 2010 cocos2d-x.org

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#import "AppController.h"
#import <cocos/cocos2d.h>
#import "../../Classes/AppDelegate.h"
#import <Mixpanel/Mixpanel.h>
#import <AppsFlyerLib/AppsFlyerTracker.h>
#import "../../Classes/DeepLinkingSingleton.h"

@implementation AppController

@synthesize window;

#pragma mark -
#pragma mark Application lifecycle

// cocos2d application instance
static AppDelegate s_sharedApplication;

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {    
    // Override point for customization after application launch.
    [super application:application didFinishLaunchingWithOptions:launchOptions];
    
    //Shake Gesture breaks Portrait text input. Switched off
    [application setApplicationSupportsShakeToEdit:NO];
    
    [AppsFlyerTracker sharedTracker].appsFlyerDevKey = @"BzPYMg8dkYsCuDn8XBUN94";
    [AppsFlyerTracker sharedTracker].appleAppID = @"1068910573";
    
    [Mixpanel sharedInstanceWithToken:@"7e94d58938714fa180917f0f3c7de4c9"];
    [Mixpanel sharedInstance].showNotificationOnActive = NO;

    return YES;
}

-(BOOL)application:(UIApplication *)application openURL:(NSURL *)url sourceApplication:(NSString *)sourceApplication annotation:(id)annotation {
    
    if(url)
        DeepLinkingSingleton::getInstance()->setDeepLink(std::string([[url absoluteString] UTF8String]));
    
    return YES;
}


@end
