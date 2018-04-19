//
//  ViewController.m
//  webviewComm
//
//  Created by Tamas Bonis on 20/01/2017.
//  Copyright © 2017 Tamas Bonis. All rights reserved.
//

#import "MediaPlayer_ios.h"
#import "ios_Cocos2d_Callbacks.h"

@implementation MediaPlayerController

- (id)init
{
    exitRequested = false;
    self = [super init];
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
}

-(void)startBuildingMediaPlayer:(NSString*)url
{
    NSURL *mediaURL = [NSURL URLWithString:url];
    self.moviePlayerController = [[MPMoviePlayerController alloc] initWithContentURL:mediaURL];
    
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(moviePlaybackComplete:)
                                                 name:MPMoviePlayerPlaybackDidFinishNotification
                                               object:_moviePlayerController];
    
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(moviePlaybackStateChanged:)
                                                 //name:MPMoviePlayerPlaybackStateDidChangeNotification
                                                 name: MPMoviePlayerNowPlayingMovieDidChangeNotification
                                               object:_moviePlayerController];
    
    [_moviePlayerController.view setFrame:CGRectMake(0, 0, [[UIScreen mainScreen] bounds].size.width, [[UIScreen mainScreen] bounds].size.height)];
    
    [self.view addSubview:_moviePlayerController.view];
    [self addLoadingLayer];
    
    [_moviePlayerController play];
    
    [self addBackButton];
}

-(void)addBackButton
{
    CGSize screenSize = [[UIScreen mainScreen] bounds].size;
    CGFloat buttonWidth = screenSize.width * 0.05;
    
    UIButton* backButton = [UIButton buttonWithType:UIButtonTypeCustom];
    [backButton addTarget:self action:@selector(buttonClicked:) forControlEvents:UIControlEventTouchUpInside];
    [backButton setFrame:CGRectMake(buttonWidth / 4.0f, buttonWidth / 4.0f, buttonWidth, buttonWidth)];
    [backButton setExclusiveTouch:YES];
    [backButton setImage:[UIImage imageNamed:@"res/navigation/back_button.png"] forState:UIControlStateNormal];
    
    [self.view addSubview:backButton];
}

-(void) buttonClicked:(UIButton*)sender
{
    if(exitRequested)
    {
        return;
    }
    
    exitRequested = true;
    [sender removeFromSuperview];
    [_moviePlayerController stop];
    
}

#pragma mark Event listeners ------------------------------------------------------------------------------------------------------

- (void) moviePlaybackComplete:(NSNotification*)notification
{
    if(exitRequested)
    {
        if(loadingLayer != nil)
        {
            [loadingLayer removeFromSuperlayer];
            loadingLayer = nil;
        }
        
        exitRequested = false;
        [_moviePlayerController setContentURL:nil];
        [_moviePlayerController setFullscreen:NO animated:YES];
        [_moviePlayerController.view removeFromSuperview];
        Azoomee::navigateToBaseScene();
    }
}

-(void) moviePlaybackStateChanged:(NSNotification*)notification
{
    if(_moviePlayerController.playbackState == 1 && loadingLayer != nil)
    {
        [loadingLayer removeFromSuperlayer];
        loadingLayer = nil;
    }
}

#pragma mark loading screen ---------------------------------------------------------------------------------------------------------

-(void)addLoadingLayer
{
    //basic black layer
    loadingLayer = [CALayer layer];
    [loadingLayer setBackgroundColor:[[UIColor blackColor] CGColor]];
    [loadingLayer setFrame:CGRectMake(0, 0, [[UIScreen mainScreen] bounds].size.width, [[UIScreen mainScreen] bounds].size.height)];
    [[self.view layer] addSublayer:loadingLayer];
    
    CGSize screenSize = CGSizeMake([[UIScreen mainScreen] bounds].size.width, [[UIScreen mainScreen] bounds].size.height);
    CGPoint midPoint = CGPointMake(screenSize.width / 2, screenSize.height / 2);
    
    //layer with circle 1
    CALayer* circle1 = [CALayer layer];
    UIImage* circle1Content = [UIImage imageNamed:@"res/webcommApi/circle_1.png"];
    
    [circle1 setContents:(id)circle1Content.CGImage];
    [circle1 setFrame:CGRectMake(midPoint.x - screenSize.height / 4, midPoint.y - screenSize.height / 4, screenSize.height / 2, screenSize.height / 2)];
    [circle1 setBackgroundColor:[[UIColor clearColor] CGColor]];
    [loadingLayer addSublayer:circle1];
    
    [self addRotateAnimationToLayer:circle1 direction:1];
    
    //layer with circle 2
    CALayer* circle2 = [CALayer layer];
    [circle2 setContents:(id)circle1Content.CGImage];
    [circle2 setFrame:CGRectMake(midPoint.x - screenSize.height / 5, midPoint.y - screenSize.height / 5, screenSize.height / 2.5, screenSize.height / 2.5)];
    [circle2 setBackgroundColor:[[UIColor clearColor] CGColor]];
    [loadingLayer addSublayer:circle2];
    
    [self addRotateAnimationToLayer:circle2 direction:-0.7f];
    
    //layer with loading text
    CALayer* loadingText = [CALayer layer];
    UIImage* loadingTextContent = [UIImage imageNamed:@"res/webcommApi/load.png"];
    [loadingText setContents:(id)loadingTextContent.CGImage];
    float imageAspectRatio = loadingTextContent.size.height / loadingTextContent.size.width;
    CGSize displaySize = CGSizeMake(screenSize.width / 6, screenSize.width / 6 * imageAspectRatio);
    [loadingText setFrame:CGRectMake(midPoint.x - displaySize.width / 2, midPoint.y - displaySize.height / 2, displaySize.width, displaySize.height)];
    [loadingLayer addSublayer:loadingText];
}

-(void)addRotateAnimationToLayer:(CALayer*)layer direction:(float)direction
{
    NSNumber *rotationAtStart = [layer valueForKeyPath:@"transform.rotation"];
    CATransform3D rotationTransform = CATransform3DRotate(layer.transform, 40 * M_PI * direction, 0.0, 0.0, 1.0);
    layer.transform = rotationTransform;
    CABasicAnimation *circleAnimation = [CABasicAnimation animationWithKeyPath:@"transform.rotation"];
    circleAnimation.duration = 120.0f;
    circleAnimation.fromValue = rotationAtStart;
    circleAnimation.toValue = [NSNumber numberWithFloat:([rotationAtStart floatValue] + 40 * M_PI * direction)];
    [layer addAnimation:circleAnimation forKey:@"transform.rotation"];
}

@end
