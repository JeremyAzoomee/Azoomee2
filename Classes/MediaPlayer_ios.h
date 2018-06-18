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
    
    float _videoTimeSent;
    float _previousRate;
    
}

@property (nonatomic, retain) AVQueuePlayer* queuePlayer;
@property (nonatomic, retain) AVPlayerViewController* playerController;
@property (nonatomic, retain) UIButton* backButton;
@property (nonatomic, retain) CALayer *loadingLayer;
@property (nonatomic, retain) AVPlayerItem* lastPlayedItem;

-(id)init;
-(void)startBuildingMediaPlayer:(NSString*)url;
-(void)addBackButton;

@end
