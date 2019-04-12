//
//  ViewController.m
//  webviewComm
//
//  Created by Tamas Bonis on 20/01/2017.
//  Copyright © 2017 Tamas Bonis. All rights reserved.
//

#import "MediaPlayer_ios.h"
#import "ios_Cocos2d_Callbacks.h"

using namespace Azoomee;

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

-(void)startBuildingMediaPlayer:(NSString*)url progressSeconds:(int)videoProgressSeconds
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
	_currentItemIndex = startPlaylistElementIndex;
    [self.queuePlayer play];

	if(videoProgressSeconds > 0)
	{
		CMTime startTime = CMTimeMakeWithSeconds(videoProgressSeconds, NSEC_PER_SEC);
		[_queuePlayer seekToTime:startTime];
	}
	
    [self createButtons];
    [self createFavBanner];
}

- (void) createButtons
{
    CGFloat buttonWidth = 0.0f;
    CGSize screenSize = [[UIScreen mainScreen] bounds].size;
    
    buttonWidth = (screenSize.width > screenSize.height) ? screenSize.width / 15.0f : screenSize.height / 15.0f;
    
    _buttonWidth = buttonWidth;
    
    screenSize.width -= buttonWidth * 1.5f;
    screenSize.height -= buttonWidth * 1.5f;
    
    _backButton = [UIButton buttonWithType:UIButtonTypeCustom];
    [_backButton addTarget:self action:@selector(buttonClicked:) forControlEvents:UIControlEventTouchUpInside];
    [_backButton setFrame:CGRectMake(buttonWidth/4, buttonWidth/4, buttonWidth, buttonWidth)];
    [_backButton setExclusiveTouch:YES];
    [_backButton setImage:[UIImage imageNamed:@"res/webview_buttons/close_unelected_v2.png"] forState:UIControlStateNormal];
    
    if(!isAnonUser())
    {
        _favButton = [UIButton buttonWithType:UIButtonTypeCustom];
        [_favButton addTarget:self action:@selector(buttonClicked:) forControlEvents:UIControlEventTouchUpInside];
        [_favButton setFrame:CGRectMake(buttonWidth/4, buttonWidth/4 + buttonWidth, buttonWidth, buttonWidth)];
        [_favButton setExclusiveTouch:YES];
        [_favButton setImage:[UIImage imageNamed:@"res/webview_buttons/favourite_unelected_v2.png"] forState:UIControlStateNormal];
        [_favButton setImage:[UIImage imageNamed:@"res/webview_buttons/favourite_selected_v2.png"] forState:UIControlStateSelected];
        if(isFavContent())
        {
            [_favButton setSelected: true];
        }
        if(isChatEntitled())
        {
            _shareButton = [UIButton buttonWithType:UIButtonTypeCustom];
            [_shareButton addTarget:self action:@selector(buttonClicked:) forControlEvents:UIControlEventTouchUpInside];
            [_shareButton setFrame:CGRectMake(buttonWidth/4, buttonWidth/4 + (2 * buttonWidth), buttonWidth, buttonWidth)];
            [_shareButton setExclusiveTouch:YES];
            [_shareButton setImage:[UIImage imageNamed:@"res/webview_buttons/share_unelected_v2.png"] forState:UIControlStateNormal];
            
            [self.view addSubview:_shareButton];
        }
        [self.view addSubview:_favButton];
    }
    [self.view addSubview:_backButton];
    
    _uiExpanded = false;
}

-(void) createFavBanner
{
    CGRect bounds = [[UIScreen mainScreen] bounds];
    CGSize size = bounds.size;
    
    CGFloat width = MAX(size.width * 0.334f, size.height * 0.334f);
    CGFloat height = MIN(size.height * 0.105f, size.width * 0.105f);
    
    _favContentBanner = [[UIView alloc] initWithFrame:CGRectMake((size.width / 2) - (width / 2), -height, width, height)];
    [self.view addSubview:_favContentBanner];
    
    //layer with bg
    UIImageView* bg = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"res/webview_buttons/fav_banner.png"]];
    [bg setFrame:CGRectMake(0, 0, width, height)];
    [_favContentBanner addSubview:bg];
    
    //layer with heart
    UIImageView* heart = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"res/webview_buttons/heart.png"]];
    [heart setFrame:CGRectMake(width * 0.07, height * 0.28 , width * 0.107, height * 0.44)];
    [_favContentBanner addSubview:heart];
    
    
    UILabel* text = [[UILabel alloc] initWithFrame:CGRectMake(width * 0.25, height * 0.15, width * 0.65, height * 0.7)];
    [text setText:Azoomee::getNSStringForKey("Added to favourites")];
    [text setTextColor:[UIColor whiteColor]];
    [text setAdjustsFontSizeToFitWidth:true];
    [text setFont:[UIFont fontWithName:@"SofiaProSoftRegular" size: height * 0.35]];
    [_favContentBanner addSubview:text];
    
    [bg release];
    [heart release];
    [text release];
}

-(void) buttonClicked:(UIButton*)sender
{
    [sender setSelected: !sender.isSelected];
    
    if(sender == _backButton)
    {
        [self cleanupAndExit];
    }
    else if(sender == _favButton)
    {
        if(isFavContent())
        {
            unFavContent();
        }
        else
        {
            favContent();
            [self favAnimation];
        }
    }
    else if(sender == _shareButton)
    {
        [self cleanupAndExit];
        shareContentInChat();
    }
    
}

-(void) favAnimation
{
    [UIView animateWithDuration:0.5 animations:^{ _favContentBanner.frame = CGRectMake(_favContentBanner.frame.origin.x, 0, _favContentBanner.frame.size.width, _favContentBanner.frame.size.height);}];
    [NSTimer scheduledTimerWithTimeInterval:2.0f
                                     target:self selector:@selector(closePopup:) userInfo:nil repeats:NO];
    
}

- (void) closePopup:(NSTimer *)timer
{
    [UIView animateWithDuration:0.5 animations:^{ _favContentBanner.frame = CGRectMake(_favContentBanner.frame.origin.x, -_favContentBanner.frame.size.height, _favContentBanner.frame.size.width, _favContentBanner.frame.size.height);}];
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
	Azoomee::sendProgressMetaDataVideo(0, CMTimeGetSeconds(_queuePlayer.currentItem.duration));
    if(self.queuePlayer.currentItem == self.queuePlayer.items.lastObject)
    {
        Azoomee::sendMixPanelData("video.playlistComplete", "");
        
        [self cleanupAndExit];
		return;
    }
	_currentItemIndex++;
	Azoomee::newVideoOpened(_currentItemIndex);
	if(!isAnonUser())
	{
		[_favButton setSelected: isFavContent()];
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
	
	Azoomee::sendProgressMetaDataVideo(CMTimeGetSeconds(_queuePlayer.currentItem.currentTime), CMTimeGetSeconds(_queuePlayer.currentItem.duration));
	
    [self.backButton removeFromSuperview];
    if(!isAnonUser())
    {
        [self.favButton removeFromSuperview];
        if(isChatEntitled())
        {
            [self.shareButton removeFromSuperview];
        }
    }
    [self.queuePlayer pause];
    [self.playerController.view removeFromSuperview];
    
    [_favContentBanner removeFromSuperview];
    [_favContentBanner release];
    _favContentBanner = nil;
    
    if(self.loadingLayer != nil)
    {
        [self.loadingLayer removeFromSuperlayer];
        self.loadingLayer = nil;
    }
    
    self.queuePlayer = nil;
    self.playerController = nil;
    self.lastPlayedItem = nil;
    
    Azoomee::navigateToBaseScene();
}

@end
