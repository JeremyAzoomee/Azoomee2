//
//  ViewController.h
//  webviewComm
//
//  Created by Tamas Bonis on 20/01/2017.
//  Copyright © 2017 Tamas Bonis. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <WebKit/WebKit.h>
#import <WebKit/WKWebsiteDataStore.h>

@interface WebViewController : UIViewController <WKNavigationDelegate, WKUIDelegate> {
    NSString *urlToLoad;
    NSString *useridToUse;
    bool iframeloaded;
    WKWebView *webview;
    UIButton* backButton;
    UIButton* _favButton;
    UIButton* _shareButton;
    float _closeButtonAnchorX;
    float _closeButtonAnchorY;
    float _buttonWidth;
    bool _uiExpanded;
}

-(id)init;
- (void)startBuildingWebView:(NSString*)url userid:(NSString *)userid closeButtonAnchorX:(float)closeButtonAnchorX closeButtonAnchorY:(float)closeButtonAnchorY;
- (void)removeWebViewWhileInBackground;
- (void)addWebViewToScreen;
- (void)createButtons;

@end
