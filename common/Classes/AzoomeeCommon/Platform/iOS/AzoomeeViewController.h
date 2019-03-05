#ifndef AzoomeeCommon_AzoomeeViewController_h
#define AzoomeeCommon_AzoomeeViewController_h

#import <UIKit/UIKit.h>

@interface AzoomeeViewController : UIViewController {
    bool _forcePortrait;
    bool _anyOrientation;
}

@property(nonatomic, readonly) AzoomeeViewController* viewController;

- (BOOL) prefersStatusBarHidden;

- (void) setOrientationToPortrait;
- (void) setOrientationToLandscape;
- (void) setOrientationToAny;

- (void) setMultipleTouchEnabled:(BOOL)enabled;

+ (AzoomeeViewController*) sharedInstance;

@end

#endif
