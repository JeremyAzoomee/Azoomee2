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
    
    _videoTimeSent = -1.0f;
    _previousRate = 0.0f;
    
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
    NSString* playlistString = Azoomee::getPlaylistString();
    NSArray* playlistUrls = [playlistString componentsSeparatedByString:@"|"];
    NSMutableArray* playlistItems = [[NSMutableArray alloc] init];
    int startPlaylistElementIndex = 0;
    
    for(int i = 0; i < playlistUrls.count; i++)
    {
        if([url isEqualToString:playlistUrls[i]])
        {
            startPlaylistElementIndex = i;
        }
        
        AVPlayerItem *item = [[AVPlayerItem alloc] initWithURL:[NSURL URLWithString:playlistUrls[i]]];
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(moviePlaybackLogAccessed:) name:AVPlayerItemNewAccessLogEntryNotification object:item];
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(playerItemDidReachEnd:) name:AVPlayerItemDidPlayToEndTimeNotification object:item];
        
        [playlistItems addObject:item];
    }
    
    self.queuePlayer = [[AVQueuePlayer alloc] initWithItems:playlistItems];
    
    dispatch_queue_t mainQueue = dispatch_get_main_queue();
    [self.queuePlayer addPeriodicTimeObserverForInterval:CMTimeMakeWithSeconds(1.0f, NSEC_PER_SEC) queue:mainQueue usingBlock:^(CMTime time) {
        [self playbackTimeTickEvent];
    }];
    
    self.playerController = [[AVPlayerViewController alloc] init];
    _playerController.player = _queuePlayer;
    [_playerController.view setFrame:CGRectMake(0, 0, [[UIScreen mainScreen] bounds].size.width, [[UIScreen mainScreen] bounds].size.height)];
    
    [self.view addSubview:_playerController.view];
    
    [self addLoadingLayer];
    for(int i = 0; i < startPlaylistElementIndex; i++)
    {
        [_queuePlayer advanceToNextItem];
    }
    
    [_queuePlayer play];
    
    [self addBackButton];
}

-(void)addBackButton
{
    CGSize screenSize = [[UIScreen mainScreen] bounds].size;
    CGFloat buttonWidth = screenSize.width * 0.05;
    
    _backButton = [UIButton buttonWithType:UIButtonTypeCustom];
    [_backButton addTarget:self action:@selector(buttonClicked:) forControlEvents:UIControlEventTouchUpInside];
    [_backButton setFrame:CGRectMake(buttonWidth / 4.0f, buttonWidth / 4.0f, buttonWidth, buttonWidth)];
    [_backButton setExclusiveTouch:YES];
    [_backButton setImage:[UIImage imageNamed:@"res/navigation/back_button.png"] forState:UIControlStateNormal];
    
    [self.view addSubview:_backButton];
}

-(void) buttonClicked:(UIButton*)sender
{
    [self cleanupAndExit];
    
}

#pragma mark Event listeners ------------------------------------------------------------------------------------------------------

-(void) moviePlaybackLogAccessed:(NSNotification*)notification
{
    if(loadingLayer != nil)
    {
        [loadingLayer removeFromSuperlayer];
        loadingLayer = nil;
    }
}

-(void) playbackTimeTickEvent
{
    if(_queuePlayer.currentItem != _lastPlayedItem) //track has changed
    {
        _lastPlayedItem = _queuePlayer.currentItem;
        _videoTimeSent = -1.0f;
        _previousRate = 0.0f;
        
        NSLog(@"VIDEOLOG First frame appeared");
    }
    
    AVPlayerItem *currentItem = _queuePlayer.currentItem;
    float duration = CMTimeGetSeconds(currentItem.duration);
    float position = CMTimeGetSeconds(currentItem.currentTime);
    
    if(_queuePlayer.rate != _previousRate)
    {
        _previousRate = _queuePlayer.rate;
        NSLog(@"VIDEOLOG Rate change to: %f", _previousRate);
    }
    
    if(duration > 0.0f && position > 0.0f)
    {
        float playbackRatio = position / duration;
        
        if(playbackRatio > 0.0f && _videoTimeSent < 0.0f)
        {
            _videoTimeSent = 0.0f;
            NSLog(@"VIDEOLOG 0pc passed");
        }
        
        if(playbackRatio > 0.25f && _videoTimeSent < 0.25f)
        {
            _videoTimeSent = 0.25f;
            NSLog(@"VIDEOLOG 25pc passed");
        }
        
        if(playbackRatio > 0.5f && _videoTimeSent < 0.5f)
        {
            _videoTimeSent = 0.5f;
            NSLog(@"VIDEOLOG 50pc passed");
        }
        
        if(playbackRatio > 0.75f && _videoTimeSent < 0.75f)
        {
            _videoTimeSent = 0.75f;
            NSLog(@"VIDEOLOG 75pc passed");
        }
    }
}

-(void) playerItemDidReachEnd:(NSNotification*)notification
{
    NSLog(@"VIDEOLOG Item completed");
    
    if(_queuePlayer.currentItem == _queuePlayer.items.lastObject)
    {
        NSLog(@"VIDEOLOG Playlist completed");
        
        [self cleanupAndExit];
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
    CGSize displaySize = CGSizeMake(screenSize.width / 8, screenSize.width / 8 * imageAspectRatio);
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

#pragma mark cleanup and exit ---------------------------------------------------------------------------------------------------------------

-(void)cleanupAndExit
{
    if(exitRequested)
    {
        return;
    }
    
    exitRequested = true;
    [_backButton removeFromSuperview];
    [_queuePlayer pause];
    [_playerController.view removeFromSuperview];
    
    if(loadingLayer != nil)
    {
        [loadingLayer removeFromSuperlayer];
        loadingLayer = nil;
    }
    
    Azoomee::navigateToBaseScene();
}

@end
