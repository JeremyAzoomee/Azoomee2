//
//  ViewController.h
//  webviewComm
//
//  Created by Tamas Bonis on 20/01/2017.
//  Copyright © 2017 Tamas Bonis. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <WebKit/WebKit.h>

@interface WebViewController : UIViewController <UIWebViewDelegate, WKScriptMessageHandler, WKNavigationDelegate> {
    NSString *urlToLoad;
    NSString *useridToUse;
    bool iframeloaded;
    UIWebView *webview;
    UIButton* backButton;
}

-(id)init;
- (void)startBuildingWebView:(NSString*)url userid:(NSString *)userid;
- (void)removeWebViewWhileInBackground;
- (void)addWebViewToScreen;
- (void)createButton:(NSString*)buttonImage;

@end
