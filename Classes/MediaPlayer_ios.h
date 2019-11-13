//
//  ViewController.h
//  webviewComm
//
//  Created by Tamas Bonis on 20/01/2017.
//  Copyright © 2017 Tamas Bonis. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <MediaPlayer/MediaPlayer.h>
#import <AVFoundation/AVFoundation.h>
#import <AVKit/AVKit.h>

@interface MediaPlayerController: UIViewController {
    bool exitRequested;
	
	int _currentItemIndex;
	
    float _videoTimeSent;
    float _previousRate;
    float _buttonWidth;
    bool _uiExpanded;
}

@property (nonatomic, retain) AVQueuePlayer* queuePlayer;
@property (nonatomic, retain) AVPlayerViewController* playerController;
@property (nonatomic, retain) UIButton* backButton;
@property (nonatomic, retain) UIButton* favButton;
@property (nonatomic, retain) UIButton* shareButton;
@property (nonatomic, retain) CALayer *loadingLayer;
@property (nonatomic, retain) AVPlayerItem* lastPlayedItem;

-(id)init;
-(void)startBuildingMediaPlayer:(NSString*)url progressSeconds:(int)videoProgressSeconds;
-(void)createButtons;

@end
