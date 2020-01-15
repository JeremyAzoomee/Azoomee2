//
//  ViewController.m
//  webviewComm
//
//  Created by Tamas Bonis on 20/01/2017.
//  Copyright © 2017 Tamas Bonis. All rights reserved.
//

#import "WebViewController_ios.h"
#import "ios_Cocos2d_Callbacks.h"

using namespace Azoomee;

@implementation WebViewController

- (id)init
{
    self = [super init];
    iframeloaded = false;
    return self;
}

- (void)startBuildingWebView:(NSString*)url userid:(NSString*)userid closeButtonAnchorX:(float)closeButtonAnchorX closeButtonAnchorY:(float)closeButtonAnchorY
{
    urlToLoad = [url copy];
    useridToUse = [userid copy];
    _closeButtonAnchorX = closeButtonAnchorX;
    _closeButtonAnchorY = closeButtonAnchorY;
    
    [self addWebViewToScreen];
    [self createButtons];
}

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
    
}

- (void)addWebViewToScreen {
    if(webview) return;
    
    CGRect screenRect = [[UIScreen mainScreen] bounds];
    CGFloat width = screenRect.size.width;
    CGFloat height = screenRect.size.height;
    
    if(isDeviceIphoneX())
    {
        height < width ? width -= 50.0f : height -= 50.0f;
    }
    WKWebViewConfiguration* config = [[WKWebViewConfiguration alloc] init];
    [config setAllowsInlineMediaPlayback:YES];
    [config setMediaPlaybackRequiresUserAction:NO];
    [config.preferences setValue:@"TRUE" forKey:@"allowFileAccessFromFileURLs"];
    
    webview = [[WKWebView alloc]initWithFrame:CGRectMake(0, 0, width, height) configuration:config];
    
    NSString* cachesPath = [NSSearchPathForDirectoriesInDomains(NSCachesDirectory, NSUserDomainMask, YES) lastObject];
    NSString* urlToCall = [cachesPath stringByAppendingPathComponent:@"gameCache/index_ios.html"];
	NSURL *nsurl = [NSURL fileURLWithPath:urlToCall];

    [webview scrollView].scrollEnabled = NO;
    [webview scrollView].bounces = NO;
    webview.navigationDelegate = self;
    webview.UIDelegate = self;

    [webview loadFileURL:nsurl allowingReadAccessToURL:[nsurl URLByDeletingLastPathComponent]];
    [webview setBackgroundColor:[UIColor blackColor]];
    [self.view addSubview:webview];
    
}

- (void)webView:(WKWebView *)webView decidePolicyForNavigationAction:(WKNavigationAction *)navigationAction decisionHandler:(void (^)(WKNavigationActionPolicy))decisionHandler
{
    
    if(navigationAction.targetFrame != nil)
    {
        if(!navigationAction.targetFrame.mainFrame)
        {
            decisionHandler(WKNavigationActionPolicyAllow);
            return;
        }
    }
    
    NSURL* url = navigationAction.request.URL;
    NSString *urlString = url.absoluteString;
    
    if ([urlString hasPrefix:@"apirequest:"]) {
        
        NSURLComponents *urlComponents = [NSURLComponents componentsWithURL:url resolvingAgainstBaseURL:NO];
        
        NSArray *urlItems = [urlComponents.string componentsSeparatedByString:@"?"];
        NSString *method = [urlItems objectAtIndex:2];
        NSString *responseId = [urlItems objectAtIndex:4];
        NSString *sendData = @"null";
        
        if([method isEqualToString:@"updateHighScore"])
        {
            sendData = [urlItems objectAtIndex:6];
        }
        
        if([method isEqualToString:@"saveImage"])
        {
            sendData = [urlItems objectAtIndex:6];
        }
        
        const char* returnString = sendGameApiRequest([method cStringUsingEncoding:NSUTF8StringEncoding], [responseId cStringUsingEncoding:NSUTF8StringEncoding], [sendData cStringUsingEncoding:NSUTF8StringEncoding]);
        NSLog(@"Sending string back to web: %s", returnString);
        
        NSString *callString = [NSString stringWithFormat:@"answerMessageReceivedFromAPI(\"%s\")", returnString];
        NSLog(@"callstring is: %@", callString);
        [webView evaluateJavaScript:callString completionHandler:nil];
        
        decisionHandler(WKNavigationActionPolicyCancel);
        return;
    }
    
    if ([urlString hasPrefix:@"videoerror"])
    {
        [webview loadHTMLString:@"" baseURL:nil];
        [webview stopLoading];
        webview.navigationDelegate = nil;
        webview.UIDelegate = nil;
        
        [webview removeFromSuperview];
        
        [backButton removeFromSuperview];
        
        [useridToUse release];
        [urlToLoad release];
        [webview release];
        webview = nil;
        
        navigateToLoginScene();
        
        decisionHandler(WKNavigationActionPolicyCancel);
        return;
    }
    
    if ([urlString hasPrefix:@"videoevent"])
    {
        NSLog(@"VideoEvent received");
        NSLog(@"Query: %@", [url query]);
        NSLog(@"Host: %@", [url host]);
        
        NSString *urlHost = [url host];
        NSString *urlQuery = [url query];
        
        sendMixPanelData([urlHost cStringUsingEncoding:NSUTF8StringEncoding], [urlQuery cStringUsingEncoding:NSUTF8StringEncoding]);
        
        decisionHandler(WKNavigationActionPolicyCancel);
        return;
    }
    
    if ([urlString hasPrefix:@"finishview"])
    {
        [self finishView];
        return;
    }
    
    decisionHandler(WKNavigationActionPolicyAllow);
    
}

- (void)webView:(WKWebView *)webView didFinishNavigation:(WKNavigation *)navigation
{
    if(!iframeloaded)
       {
           [webView evaluateJavaScript:@"clearLocalStorage()" completionHandler:^(id result, NSError *error) {
               if (error == nil) {
                   if (result != nil) {
                       NSLog(@"%@", [NSString stringWithFormat:@"%@", result]);
                   }
               }
               else
               {
                   NSLog(@"%@", error.localizedDescription);
               }
           }];
               
           NSString *localStorageData = [NSString stringWithFormat: @"%s", getLocalStorageForGame()];
               
           NSString *addDataString = [NSString stringWithFormat:@"addDataToLocalStorage(\"%@\")", localStorageData];
           NSLog(@"addDataString: %@", addDataString);
           [webView evaluateJavaScript:addDataString completionHandler:^(id result, NSError *error) {
               if (error == nil) {
                   if (result != nil) {
                       NSLog(@"%@", [NSString stringWithFormat:@"%@", result]);
                   }
               }
               else
               {
                   NSLog(@"%@", error.localizedDescription);
               }
           }];
           
           NSString *loadString = [NSString stringWithFormat:@"addFrameWithUrl(\"%@\")", urlToLoad];
           [webView evaluateJavaScript:loadString completionHandler:^(id result, NSError *error) {
               if (error == nil) {
                   if (result != nil) {
                       NSLog(@"%@", [NSString stringWithFormat:@"%@", result]);
                   }
               }
               else
               {
                   NSLog(@"%@", error.localizedDescription);
               }
           }];
           
           iframeloaded = true;
       };
}

- (void)webViewWebContentProcessDidTerminate:(WKWebView *)webView {
    // Reload current page, since we have crashed the WebContent process
    // (most likely due to memory pressure)
    NSSet *websiteDataTypes = [NSSet setWithArray:@[
                                WKWebsiteDataTypeDiskCache,
                                WKWebsiteDataTypeOfflineWebApplicationCache,
                                WKWebsiteDataTypeMemoryCache,
                                WKWebsiteDataTypeLocalStorage,
                                WKWebsiteDataTypeCookies,
                                WKWebsiteDataTypeSessionStorage,
                                //WKWebsiteDataTypeIndexedDBDatabases,
                                //WKWebsiteDataTypeWebSQLDatabases,
                                //WKWebsiteDataTypeFetchCache, //(iOS 11.3, *)
                                //WKWebsiteDataTypeServiceWorkerRegistrations, //(iOS 11.3, *)
                            ]];
    // All kinds of data
    // NSSet *websiteDataTypes = [WKWebsiteDataStore allWebsiteDataTypes];
    // Date from
    NSDate *dateFrom = [NSDate dateWithTimeIntervalSince1970:0];
    // Execute
    [[WKWebsiteDataStore defaultDataStore] removeDataOfTypes:websiteDataTypes modifiedSince:dateFrom completionHandler:^{
       // Done
        iframeloaded = NO;
        NSString* cachesPath = [NSSearchPathForDirectoriesInDomains(NSCachesDirectory, NSUserDomainMask, YES) lastObject];
        NSString* urlToCall = [cachesPath stringByAppendingPathComponent:@"gameCache/index_ios.html"];
        NSURL *nsurl = [NSURL fileURLWithPath:urlToCall];
        [webview loadFileURL:nsurl allowingReadAccessToURL:[nsurl URLByDeletingLastPathComponent]];
    }];
    
    
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void) createButtons
{
    CGFloat buttonWidth = 0.0f;
    CGSize screenSize = [[UIScreen mainScreen] bounds].size;
    
    buttonWidth = (screenSize.width > screenSize.height) ? screenSize.width / 17.5f : screenSize.height / 17.5f;
    
    _buttonWidth = buttonWidth;
    
    screenSize.width -= buttonWidth * 1.5f;
    screenSize.height -= buttonWidth * 1.5f;
	
    float xMod = 0;
    float yMod = 0;
    if(_closeButtonAnchorY == 0.5)
    {
        if(_closeButtonAnchorX == 1.0)
        {
            xMod = -1;
        }
        else
        {
            xMod = 1;
        }
    }
    else if(_closeButtonAnchorY == 0.0)
    {
        yMod = 1;
    }
    else
    {
        yMod = -1;
    }
    
    CGFloat buttonPaddingX = buttonWidth / 4.0f;
    CGFloat buttonPaddingY = buttonWidth / 4.0f;
    
    if(isDeviceIphoneX() && screenSize.width > screenSize.height)
    {
        buttonPaddingX += ((_closeButtonAnchorX - 0.5) * 2.0f) * (-25.0f);
    }
    
    backButton = [UIButton buttonWithType:UIButtonTypeCustom];
    [backButton addTarget:self action:@selector(buttonClicked:) forControlEvents:UIControlEventTouchUpInside];
    [backButton setFrame:CGRectMake(buttonPaddingX + (screenSize.width * _closeButtonAnchorX), buttonPaddingY + (screenSize.height * _closeButtonAnchorY), buttonWidth, buttonWidth)];
    [backButton setExclusiveTouch:YES];
    [backButton setImage:[UIImage imageNamed:@"res/webview_buttons/close_unelected_v2.png"] forState:UIControlStateNormal];
    if(!isAnonUser())
    {
        _favButton = [UIButton buttonWithType:UIButtonTypeCustom];
        [_favButton addTarget:self action:@selector(buttonClicked:) forControlEvents:UIControlEventTouchUpInside];
        [_favButton setFrame:CGRectMake(buttonPaddingX + (screenSize.width * _closeButtonAnchorX) + (xMod * _buttonWidth), buttonPaddingY + (screenSize.height * _closeButtonAnchorY) + (yMod * _buttonWidth), buttonWidth, buttonWidth)];
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
            [_shareButton setFrame:CGRectMake(buttonPaddingX + (screenSize.width * _closeButtonAnchorX) + 2 * (xMod * _buttonWidth), buttonPaddingY + (screenSize.height * _closeButtonAnchorY) + 2 * (yMod * _buttonWidth), buttonWidth, buttonWidth)];
            [_shareButton setExclusiveTouch:YES];
            [_shareButton setImage:[UIImage imageNamed:@"res/webview_buttons/share_unelected_v2.png"] forState:UIControlStateNormal];
            
            [self.view addSubview:_shareButton];
        }
        [self.view addSubview:_favButton];
    }
    [self.view addSubview:backButton];
    
    _uiExpanded = false;
}

-(void) buttonClicked:(UIButton*)sender
{
    [sender setSelected: !sender.isSelected];
    
    if(sender == backButton)
    {
        if([urlToLoad hasSuffix:@"html"])
        {
            [webview evaluateJavaScript:@"saveLocalDataBeforeExit()" completionHandler:^(id result, NSError *error) {
                if (error == nil) {
                    if (result != nil) {
                        saveLocalStorageData([NSString stringWithFormat:@"%@", result]);
                    }
                }
                [self finishView];
            }];
        }
        else
        {
            [self finishView];
        }
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
        }
    }
    else if(sender == _shareButton)
    {
        if([urlToLoad hasSuffix:@"html"])
        {
            [webview evaluateJavaScript:@"saveLocalDataBeforeExit()" completionHandler:^(id result, NSError *error) {
                if (error == nil) {
                    if (result != nil) {
                        saveLocalStorageData([NSString stringWithFormat:@"%@", result]);
                    }
                }
                [self finishView];
                shareContentInChat();
            }];
        }
        else
        {
            [self finishView];
            shareContentInChat();
        }
        
    }
}

-(void) removeWebViewWhileInBackground
{
    [backButton removeFromSuperview];
	backButton = nil;
    if(!isAnonUser())
    {
        [_favButton removeFromSuperview];
		_favButton = nil;
        if(isChatEntitled())
        {
            [_shareButton removeFromSuperview];
			_shareButton = nil;
        }
    }
    
    [webview evaluateJavaScript:@"saveLocalDataBeforeExit()" completionHandler:^(id result, NSError *error) {
        if (error == nil) {
            if (result != nil) {
                saveLocalStorageData([NSString stringWithFormat:@"%@", result]);
            }
        }
        [webview loadHTMLString:@"" baseURL:nil];
        [webview stopLoading];
        webview.navigationDelegate = nil;
        webview.UIDelegate = nil;
           
        [webview removeFromSuperview];
        [webview release];
        webview = nil;
           
        iframeloaded = NO;
    }];
}

-(void) finishView
{
    if(webview == nil)
    {
        return;
    }
    
    [webview loadHTMLString:@"" baseURL:nil];
    [webview stopLoading];
    webview.navigationDelegate = nil;
    webview.UIDelegate = nil;
    
    [webview removeFromSuperview];
	
	Azoomee::sendProgressMetaDataGame();
	
    [backButton removeFromSuperview];
    backButton = nil;
    if(!isAnonUser())
    {
        [_favButton removeFromSuperview];
        _favButton = nil;
        if(isChatEntitled())
        {
            [_shareButton removeFromSuperview];
            _shareButton = nil;
        }
    }
    
    [useridToUse release];
    [urlToLoad release];
    [webview release];
    webview = nil;
    
    navigateToBaseScene();
}

@end
