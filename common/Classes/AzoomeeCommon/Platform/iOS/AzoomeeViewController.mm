#import "AzoomeeViewController.h"
#import "../../Application.h"
#import <cocos/cocos2d.h>
#import <cocos/platform/ios/CCEAGLView-ios.h>

#define DEGREES_TO_RADIANS(angle) ((angle) / 180.0 * M_PI)


@implementation AzoomeeViewController

/*
 // The designated initializer.  Override if you create the controller programmatically and want to perform customization that is not appropriate for viewDidLoad.
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    if ((self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil])) {
        // Custom initialization
    }
    return self;
}
*/

// Implement loadView to create a view hierarchy programmatically, without using a nib.
- (void)loadView {
    
    _forcePortrait = false;
    cocos2d::Application *app = cocos2d::Application::getInstance();
    
    // Initialize the GLView attributes
    app->initGLContextAttrs();
    cocos2d::GLViewImpl::convertAttrs();
    
    // Initialize the CCEAGLView
    CCEAGLView *eaglView = [CCEAGLView viewWithFrame: [UIScreen mainScreen].bounds
                                         pixelFormat: (__bridge NSString *)cocos2d::GLViewImpl::_pixelFormat
                                         depthFormat: cocos2d::GLViewImpl::_depthFormat
                                  preserveBackbuffer: NO
                                          sharegroup: nil
                                       multiSampling: NO
                                     numberOfSamples: 0 ];
    
    // Enable or disable multiple touches
    [eaglView setMultipleTouchEnabled:YES];
    
    // Set EAGLView as view of RootViewController
    self.view = eaglView;
    
    cocos2d::GLView *glview = cocos2d::GLViewImpl::createWithEAGLView((__bridge void *)self.view);
    
    //set the GLView as OpenGLView of the Director
    cocos2d::Director::getInstance()->setOpenGLView(glview);
    
    //run the cocos2d-x game scene
    app->run();
}

// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];

}

- (void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
}

- (void)viewDidDisappear:(BOOL)animated {
    [super viewDidDisappear:animated];
}


// For ios6, use supportedInterfaceOrientations & shouldAutorotate instead
#ifdef __IPHONE_6_0
- (NSUInteger) supportedInterfaceOrientations{
    if(_forcePortrait)
    {
        return UIInterfaceOrientationMaskPortrait;
    }
    return UIInterfaceOrientationMaskLandscape;
}
- (UIInterfaceOrientation) preferredInterfaceOrientationForPresentation
{
    if(_forcePortrait)
    {
        return UIInterfaceOrientationPortrait;
    }
    return UIInterfaceOrientationLandscapeLeft;
}

- (BOOL) shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    if( _forcePortrait )
    {
        return UIInterfaceOrientationIsPortrait(interfaceOrientation);
    }
    return YES;
}

#endif

- (BOOL) shouldAutorotate {
    return YES;
}

- (void)willAnimateRotationToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation duration:(NSTimeInterval)duration {
    [super willAnimateRotationToInterfaceOrientation:toInterfaceOrientation duration:duration];
    
    auto glview = cocos2d::Director::getInstance()->getOpenGLView();
    if(glview)
    {
        CCEAGLView* eaglview = (__bridge CCEAGLView*)glview->getEAGLView();
        if(eaglview)
        {
            CGSize s = CGSizeMake([eaglview getWidth], [eaglview getHeight]);
            Azoomee::Application* app = (Azoomee::Application*) cocos2d::Application::getInstance();
            if(app)
            {
                app->applicationScreenSizeWillChange((int) s.width, (int) s.height, (float)duration);
            }
        }
    }
}

- (void)didRotateFromInterfaceOrientation:(UIInterfaceOrientation)fromInterfaceOrientation {
    [super didRotateFromInterfaceOrientation:fromInterfaceOrientation];

    auto glview = cocos2d::Director::getInstance()->getOpenGLView();
    if(glview)
    {
        CCEAGLView* eaglview = (__bridge CCEAGLView*)glview->getEAGLView();
        if(eaglview)
        {
            CGSize s = CGSizeMake([eaglview getWidth], [eaglview getHeight]);
            Azoomee::Application* app = (Azoomee::Application*) cocos2d::Application::getInstance();
            if(app)
            {
                app->applicationScreenSizeChanged((int) s.width, (int) s.height);
            }
        }
    }
}

//fix not hide status on ios7
- (BOOL)prefersStatusBarHidden {
    return YES;
}

- (void)didReceiveMemoryWarning {
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];

    // Release any cached data, images, etc that aren't in use.
}

- (void) setOrientationToPortrait
{
    _forcePortrait = true;
     
    NSNumber *value = [NSNumber numberWithInt:UIInterfaceOrientationPortrait];
    [[UIDevice currentDevice] setValue:value forKey:@"orientation"];
    [UIViewController attemptRotationToDeviceOrientation];
}

- (void) setOrientationToLandscape
{
    _forcePortrait = false;
    
    UIInterfaceOrientation currentOrientation = [[UIApplication sharedApplication] statusBarOrientation];
    
    NSNumber *value = [NSNumber numberWithInt:UIInterfaceOrientationLandscapeRight];
    
    if(currentOrientation == UIInterfaceOrientationLandscapeLeft)
        value = [NSNumber numberWithInt:UIInterfaceOrientationLandscapeLeft];
    
    [[UIDevice currentDevice] setValue:value forKey:@"orientation"];
    [UIViewController attemptRotationToDeviceOrientation];
}

@end
