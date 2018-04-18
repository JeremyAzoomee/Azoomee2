//
//  ViewController.h
//  webviewComm
//
//  Created by Tamas Bonis on 20/01/2017.
//  Copyright © 2017 Tamas Bonis. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <MediaPlayer/MediaPlayer.h>

@interface MediaPlayerController: UIViewController {
    bool exitRequested;
    CALayer *loadingLayer;
}

@property (nonatomic, strong) MPMoviePlayerController* moviePlayerController;

-(id)init;
-(void)startBuildingMediaPlayer:(NSString*)url;
-(void)addBackButton;

@end
