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

-(void)addLoadingLayer
{
    loadingLayer = [CALayer layer];
    [loadingLayer setBackgroundColor:[[UIColor whiteColor] CGColor]];
    [loadingLayer setFrame:CGRectMake(0, 0, [[UIScreen mainScreen] bounds].size.width, [[UIScreen mainScreen] bounds].size.height)];
    [[self.view layer] addSublayer:loadingLayer];
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

#pragma mark Event listeners

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

@end
