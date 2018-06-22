//
//  ViewController.m
//  webviewComm
//
//  Created by Tamas Bonis on 20/01/2017.
//  Copyright © 2017 Tamas Bonis. All rights reserved.
//

#import "MediaPlayer_ios.h"
#import "ios_Cocos2d_Callbacks.h"

#define SYSTEM_VERSION_GREATER_THAN_OR_EQUAL_TO(v)  ([[[UIDevice currentDevice] systemVersion] compare:v options:NSNumericSearch] != NSOrderedAscending)

@implementation MediaPlayerController

- (id)init
{
    self = [super init];
    if(self)
    {
        exitRequested = false;
        
        _videoTimeSent = -1.0f;
        _previousRate = 0.0f;
    }
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
    NSMutableArray* playlistItems = [NSMutableArray array];
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
        [item release];
    }
    
    AVQueuePlayer* queuePlayer = [[AVQueuePlayer alloc] initWithItems:playlistItems];
    self.queuePlayer = [queuePlayer autorelease];
    
    dispatch_queue_t mainQueue = dispatch_get_main_queue();
    [self.queuePlayer addPeriodicTimeObserverForInterval:CMTimeMakeWithSeconds(1.0f, NSEC_PER_SEC) queue:mainQueue usingBlock:^(CMTime time) {
        [self playbackTimeTickEvent];
    }];
    
    AVPlayerViewController* playerController = [[AVPlayerViewController alloc] init];
    self.playerController = [playerController autorelease];
    
    CGRect bounds = [[UIScreen mainScreen] bounds];
    
    self.playerController.player = self.queuePlayer;
    [self.playerController.view setFrame:CGRectMake(0, 0, bounds.size.width, bounds.size.height)];
    if(SYSTEM_VERSION_GREATER_THAN_OR_EQUAL_TO(@"9.0"))
    {
        self.playerController.allowsPictureInPicturePlayback = false;
    }
    
    [self.view addSubview:self.playerController.view];
    
    [self addLoadingLayer];
    for(int i = 0; i < startPlaylistElementIndex; i++)
    {
        [self.queuePlayer advanceToNextItem];
    }
    
    [self.queuePlayer play];
    
    [self addBackButton];
}

-(void)addBackButton
{
    CGSize screenSize = [[UIScreen mainScreen] bounds].size;
    CGFloat buttonWidth = screenSize.width * 0.05;
    
    self.backButton = [UIButton buttonWithType:UIButtonTypeCustom];
    [self.backButton addTarget:self action:@selector(buttonClicked:) forControlEvents:UIControlEventTouchUpInside];
    [self.backButton setFrame:CGRectMake(buttonWidth / 4.0f, buttonWidth / 4.0f, buttonWidth, buttonWidth)];
    [self.backButton setExclusiveTouch:YES];
    [self.backButton setImage:[UIImage imageNamed:@"res/navigation/back_button.png"] forState:UIControlStateNormal];
    
    [self.view addSubview:self.backButton];
}

-(void) buttonClicked:(UIButton*)sender
{
    [self cleanupAndExit];
    
}

#pragma mark Event listeners ------------------------------------------------------------------------------------------------------

-(void) moviePlaybackLogAccessed:(NSNotification*)notification
{
    if(self.loadingLayer != nil)
    {
        [self.loadingLayer removeFromSuperlayer];
        self.loadingLayer = nil;
    }
}

-(void) playbackTimeTickEvent
{
    if(self.queuePlayer.currentItem != self.lastPlayedItem) //track has changed
    {
        self.lastPlayedItem = _queuePlayer.currentItem;
        _videoTimeSent = -1.0f;
        _previousRate = 0.0f;
    }
    
    AVPlayerItem *currentItem = _queuePlayer.currentItem;
    float duration = CMTimeGetSeconds(currentItem.duration);
    float position = CMTimeGetSeconds(currentItem.currentTime);
    
    if(self.queuePlayer.rate != _previousRate)
    {
        _previousRate = self.queuePlayer.rate;
        Azoomee::sendMixPanelData("video.quality", cocos2d::StringUtils::format("%f", _previousRate).c_str());
    }
    
    if(duration > 0.0f && position > 0.0f)
    {
        float playbackRatio = position / duration;
        
        if(playbackRatio > 0.0f && _videoTimeSent < 0.0f)
        {
            _videoTimeSent = 0.0f;
            Azoomee::sendMixPanelData("video.time", "0");
        }
        
        if(playbackRatio > 0.25f && _videoTimeSent < 0.25f)
        {
            _videoTimeSent = 0.25f;
            Azoomee::sendMixPanelData("video.time", "25");
        }
        
        if(playbackRatio > 0.5f && _videoTimeSent < 0.5f)
        {
            _videoTimeSent = 0.5f;
            Azoomee::sendMixPanelData("video.time", "50");
        }
        
        if(playbackRatio > 0.75f && _videoTimeSent < 0.75f)
        {
            _videoTimeSent = 0.75f;
            Azoomee::sendMixPanelData("video.time", "75");
        }
    }
}

-(void) playerItemDidReachEnd:(NSNotification*)notification
{
    Azoomee::sendMixPanelData("video.complete", "");
    
    if(self.queuePlayer.currentItem == self.queuePlayer.items.lastObject)
    {
        Azoomee::sendMixPanelData("video.playlistComplete", "");
        
        [self cleanupAndExit];
    }
}

#pragma mark loading screen ---------------------------------------------------------------------------------------------------------

-(void)addLoadingLayer
{
    CGRect bounds = [[UIScreen mainScreen] bounds];
    
    //basic black layer
    self.loadingLayer = [CALayer layer];
    [self.loadingLayer setBackgroundColor:[[UIColor blackColor] CGColor]];
    [self.loadingLayer setFrame:CGRectMake(0, 0, bounds.size.width, bounds.size.height)];
    [[self.view layer] addSublayer:self.loadingLayer];
    
    CGSize screenSize = bounds.size;
    CGPoint midPoint = CGPointMake(screenSize.width / 2, screenSize.height / 2);
    
    //layer with circle 1
    CALayer* circle1 = [CALayer layer];
    UIImage* circle1Content = [UIImage imageNamed:@"res/webcommApi/circle_1.png"];
    
    [circle1 setContents:(id)circle1Content.CGImage];
    [circle1 setFrame:CGRectMake(midPoint.x - screenSize.height / 4, midPoint.y - screenSize.height / 4, screenSize.height / 2, screenSize.height / 2)];
    [circle1 setBackgroundColor:[[UIColor clearColor] CGColor]];
    [self.loadingLayer addSublayer:circle1];
    
    [self addRotateAnimationToLayer:circle1 direction:1];
    
    //layer with circle 2
    CALayer* circle2 = [CALayer layer];
    [circle2 setContents:(id)circle1Content.CGImage];
    [circle2 setFrame:CGRectMake(midPoint.x - screenSize.height / 5, midPoint.y - screenSize.height / 5, screenSize.height / 2.5, screenSize.height / 2.5)];
    [circle2 setBackgroundColor:[[UIColor clearColor] CGColor]];
    [self.loadingLayer addSublayer:circle2];
    
    [self addRotateAnimationToLayer:circle2 direction:-0.7f];
    
    //layer with loading text
    CALayer* loadingText = [CALayer layer];
    UIImage* loadingTextContent = [UIImage imageNamed:@"res/webcommApi/load.png"];
    [loadingText setContents:(id)loadingTextContent.CGImage];
    float imageAspectRatio = loadingTextContent.size.height / loadingTextContent.size.width;
    CGSize displaySize = CGSizeMake(screenSize.width / 8, screenSize.width / 8 * imageAspectRatio);
    [loadingText setFrame:CGRectMake(midPoint.x - displaySize.width / 2, midPoint.y - displaySize.height / 2, displaySize.width, displaySize.height)];
    [self.loadingLayer addSublayer:loadingText];
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
    [self.backButton removeFromSuperview];
    [self.queuePlayer pause];
    [self.playerController.view removeFromSuperview];
    
    if(self.loadingLayer != nil)
    {
        [self.loadingLayer removeFromSuperlayer];
        self.loadingLayer = nil;
    }
    
    self.queuePlayer = nil;
    self.playerController = nil;
    self.backButton = nil;
    self.lastPlayedItem = nil;
    
    Azoomee::navigateToBaseScene();
}

@end
