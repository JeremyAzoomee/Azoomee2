//
//  ViewController.h
//  webviewComm
//
//  Created by Tamas Bonis on 20/01/2017.
//  Copyright © 2017 Tamas Bonis. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface WebViewController : UIViewController <UIWebViewDelegate>

-(id)init;
- (void)startBuildingWebView:(NSString*)url userid:(NSString *)userid;

@end

NSString *urlToLoad;
NSString *useridToUse;
bool iframeloaded;
