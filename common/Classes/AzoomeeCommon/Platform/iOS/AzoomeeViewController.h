#ifndef AzoomeeCommon_AzoomeeViewController_h
#define AzoomeeCommon_AzoomeeViewController_h

#import <UIKit/UIKit.h>

@interface AzoomeeViewController : UIViewController {
    BOOL _forcePortrait;
}
- (BOOL) prefersStatusBarHidden;

- (void) setOrientationToPortrait;
- (void) setOrientationToLandscape;

@end

#endif
