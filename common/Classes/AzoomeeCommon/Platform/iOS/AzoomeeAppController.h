#ifndef AzoomeeCommon_AzoomeeAppController_h
#define AzoomeeCommon_AzoomeeAppController_h

#import <UIKit/UIKit.h>

@class AzoomeeViewController;

@interface AzoomeeAppController : NSObject <UIApplicationDelegate> {

}

@property(nonatomic, readonly) AzoomeeViewController* viewController;
@property(nonatomic, readonly) UIWindow* window;

@end

#endif
