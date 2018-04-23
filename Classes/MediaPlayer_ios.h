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
    CALayer *loadingLayer;
    
    float _videoTimeSent;
    AVPlayerItem* _lastPlayedItem;
    float _previousRate;
    
    UIButton* _backButton;
}

@property (nonatomic, retain) AVQueuePlayer* queuePlayer;
@property (nonatomic, retain) AVPlayerViewController* playerController;

-(id)init;
-(void)startBuildingMediaPlayer:(NSString*)url;
-(void)addBackButton;

@end
