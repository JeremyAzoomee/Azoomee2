#import <UIKit/UIKit.h>

@interface ButtonController_ios : UIView

@property (retain) UIView* currentView;
@property (retain) UIWebView* currentWebview;
@property (retain) UIButton* currentButton;

- (void) createButton;
- (id)initWithCloseButton:(UIView*)currentView withWebview:(UIWebView *)currentWebView;

@end
