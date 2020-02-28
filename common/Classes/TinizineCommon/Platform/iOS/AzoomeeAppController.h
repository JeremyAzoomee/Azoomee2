#ifndef TinizineCommon_AzoomeeAppController_h
#define TinizineCommon_AzoomeeAppController_h

#import <UIKit/UIKit.h>

@class AzoomeeViewController;

@interface AzoomeeAppController : NSObject <UIApplicationDelegate> {

}

@property(nonatomic, readonly) AzoomeeViewController* viewController;

@end

#endif
