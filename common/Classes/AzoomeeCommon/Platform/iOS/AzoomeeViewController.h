#ifndef AzoomeeCommon_AzoomeeViewController_h
#define AzoomeeCommon_AzoomeeViewController_h

#import <UIKit/UIKit.h>

@interface AzoomeeViewController : UIViewController {
    bool _forcePortrait;
    bool _anyOrientation;
}
- (BOOL) prefersStatusBarHidden;

- (void) setOrientationToPortrait;
- (void) setOrientationToLandscape;
- (void) setOrientationToAny;

+ (AzoomeeViewController*) sharedInstance;

@end

#endif
