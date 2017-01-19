#import "ButtonController_ios.h"
#import "ios_Cocos2d_Callbacks.h"

@implementation ButtonController_ios

- (id)initWithCloseButton:(UIView*)currentView withWebview:(UIWebView *)currentWebView {
    self = [super init];
    if (self) {
        self.currentWebview = currentWebView;
        self.currentView = currentView;
        [self createButton];
    }
    return self;
}

- (void) createButton
{
    self.currentButton = [UIButton buttonWithType:UIButtonTypeCustom];
    [self.currentButton addTarget:self action:@selector(buttonClicked:) forControlEvents:UIControlEventTouchUpInside];
    [self.currentButton setFrame:CGRectMake(52, 252, 215, 40)];
    [self.currentButton setTitle:@"Login" forState:UIControlStateNormal];
    [self.currentButton setExclusiveTouch:YES];
    
    // if you like to add backgroundImage else no need
    //[but setbackgroundImage:[UIImage imageNamed:@"XXX.png"] forState:UIControlStateNormal];
    
    [self.currentView addSubview:self.currentButton];
}

-(void) buttonClicked:(UIButton*)sender
{
    NSLog(@"you clicked on button %ld", (long)sender.tag);
    
    [self.currentWebview removeFromSuperview];
    [self.currentButton removeFromSuperview];
    [self.currentButton release];
    
    self.currentWebview = nil;
    
    navigateToBaseScene();
    
    [self release];
}


@end
