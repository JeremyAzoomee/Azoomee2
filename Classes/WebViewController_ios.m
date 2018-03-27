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
    urlToLoad = url;
    useridToUse = userid;
    _closeButtonAnchorX = closeButtonAnchorX;
    _closeButtonAnchorY = closeButtonAnchorY;
    
    [urlToLoad retain];
    [useridToUse retain];
    
    [self addWebViewToScreen];
    [self createButton];
}

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
    
}

- (void)addWebViewToScreen {
    if(webview) return;
    
    float width = self.view.frame.size.width;
    float height = self.view.frame.size.height;
    
    if(isDeviceIphoneX())
    {
        height < width ? width -= 50.0f : height -= 50.0f;
    }

    webview = [[UIWebView alloc]initWithFrame:CGRectMake(0, 0, width, height)];
    
    NSString *urlToCall;
    
    
    if([urlToLoad hasSuffix:@"html"]) //this is a game
    {
        if([urlToLoad hasPrefix:@"http"]) //game is loaded remotely
        {
            urlToCall = [NSString stringWithFormat:@"%@%@", getRemoteWebGameAPIPath(), @"index_ios.html"];
        }
        else //game is loaded locally
        {
            urlToCall = [[NSBundle mainBundle] pathForResource:@"res/webcommApi/index_ios" ofType:@"html"];
        }
    }
    else
    {
        NSString *htmlFileAddress = [[NSBundle mainBundle] pathForResource:@"res/jwplayer/index_ios" ofType:@"html"];
        urlToCall = [NSString stringWithFormat:@"%@?contentUrl=%@", htmlFileAddress, urlToLoad];
    }
    
    NSURL *nsurl=[NSURL URLWithString:urlToCall];
    NSURLRequest *nsrequest = [NSURLRequest requestWithURL:nsurl];
    
    [webview setAllowsInlineMediaPlayback:YES];
    [webview setMediaPlaybackRequiresUserAction:NO];
    [webview scrollView].scrollEnabled = NO;
    [webview scrollView].bounces = NO;
    [webview setDelegate:self];
    [webview loadRequest:nsrequest];
    [webview setBackgroundColor:[UIColor blackColor]];
    [self.view addSubview:webview];
}

- (BOOL)webView:(UIWebView *)webView shouldStartLoadWithRequest:(NSURLRequest *)request navigationType:(UIWebViewNavigationType)navigationType {
    NSString *urlString = [[request URL] absoluteString];
    
    if ([urlString hasPrefix:@"apirequest:"]) {
        
        NSURLComponents *urlComponents = [NSURLComponents componentsWithURL:request.URL resolvingAgainstBaseURL:NO];
        
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
        
        if([method isEqualToString:@"sendLog"])
        {
            sendData = [urlItems objectAtIndex:6];
        }
        
        const char* returnString = sendGameApiRequest([method cStringUsingEncoding:NSUTF8StringEncoding], [responseId cStringUsingEncoding:NSUTF8StringEncoding], [sendData cStringUsingEncoding:NSUTF8StringEncoding]);
        NSLog(@"Sending string back to web: %s", returnString);
        
        NSString *callString = [NSString stringWithFormat:@"answerMessageReceivedFromAPI(\"%s\")", returnString];
        NSLog(@"callstring is: %@", callString);
        [webView stringByEvaluatingJavaScriptFromString:callString];
        
        return NO;
    }
    
    if ([urlString hasPrefix:@"videoerror"])
    {
        [webview loadHTMLString:@"" baseURL:nil];
        [webview stopLoading];
        [webview setDelegate:nil];
        
        
        [webview removeFromSuperview];
        
        [backButton removeFromSuperview];
        
        [useridToUse release];
        [urlToLoad release];
        [webview release];
        webview = nil;
        
        navigateToLoginScene();
        
        [self release];
        
        return NO;
    }
    
    if ([urlString hasPrefix:@"videoevent"])
    {
        NSLog(@"VideoEvent received");
        NSLog(@"Query: %@", [[request URL] query]);
        NSLog(@"Host: %@", [[request URL] host]);
        
        NSString *urlHost = [[request URL] host];
        NSString *urlQuery = [[request URL] query];
        
        sendMixPanelData([urlHost cStringUsingEncoding:NSUTF8StringEncoding], [urlQuery cStringUsingEncoding:NSUTF8StringEncoding]);
        
        return NO;
    }
    
    if ([urlString hasPrefix:@"finishview"])
    {
        [self finishView];
    }
    
    return YES;
}

-(void)webViewDidFinishLoad:(UIWebView *)webView
{
    if(!iframeloaded)
    {
        if([urlToLoad hasSuffix:@"html"])
        {
            [webView stringByEvaluatingJavaScriptFromString:@"clearLocalStorage()"];
            
            NSString *localStorageData = [NSString stringWithFormat: @"%s", getLocalStorageForGame()];
            
            NSString *addDataString = [NSString stringWithFormat:@"addDataToLocalStorage(\"%@\")", localStorageData];
            NSLog(@"addDataString: %@", addDataString);
            [webView stringByEvaluatingJavaScriptFromString:addDataString];
            
            NSString *loadString = [NSString stringWithFormat:@"addFrameWithUrl(\"%@\")", urlToLoad];
            [webView stringByEvaluatingJavaScriptFromString:loadString];
        }
        else
        {
            NSString *loadString = [NSString stringWithFormat:@"startBuildingPlayer(\"%@\")", getVideoPlaylist()];
            [webView stringByEvaluatingJavaScriptFromString:loadString];
        }
        
        iframeloaded = true;
    };
}


- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void) createButton
{
    CGFloat buttonWidth = 0.0f;
    CGSize screenSize = [[UIScreen mainScreen] bounds].size;
    
    screenSize.width > screenSize.height ? buttonWidth = screenSize.width / 15.0f : buttonWidth = screenSize.height / 15.0f ;
    
    screenSize.width -= buttonWidth * 1.5f;
    screenSize.height -= buttonWidth * 1.5f;
    
    backButton = [UIButton buttonWithType:UIButtonTypeCustom];
    [backButton addTarget:self action:@selector(buttonClicked:) forControlEvents:UIControlEventTouchUpInside];
    [backButton setFrame:CGRectMake(buttonWidth/4 + (screenSize.width * _closeButtonAnchorX), buttonWidth/4 + (screenSize.height * _closeButtonAnchorY), buttonWidth, buttonWidth)];
    [backButton setExclusiveTouch:YES];

    //Check if has html, then opening game.
    if([urlToLoad containsString:@"html"])
    {
        [backButton setImage:[UIImage imageNamed:@"res/navigation/close_button.png"] forState:UIControlStateNormal];
    }
    else
    {
        [backButton setImage:[UIImage imageNamed:@"res/navigation/back_button.png"] forState:UIControlStateNormal];
    }
    
    [self.view addSubview:backButton];
}

-(void) buttonClicked:(UIButton*)sender
{
    if([urlToLoad hasSuffix:@"html"])
    {
        NSString *htmlData = [webview stringByEvaluatingJavaScriptFromString:@"saveLocalDataBeforeExit()"];
        saveLocalStorageData(htmlData);
        [self finishView];
        return;
    }
    else
    {
        [self finishView];
    }
}

-(void) removeWebViewWhileInBackground
{
    [backButton removeFromSuperview];
    
    if(![urlToLoad hasSuffix:@"html"])
    {
        return;
    }
    
    NSString *htmlData = [webview stringByEvaluatingJavaScriptFromString:@"saveLocalDataBeforeExit()"];
    saveLocalStorageData(htmlData);
    
    [webview loadHTMLString:@"" baseURL:nil];
    [webview stopLoading];
    [webview setDelegate:nil];
    
    [webview removeFromSuperview];
    [webview release];
    webview = nil;
    
    iframeloaded = NO;
}

-(void) finishView
{
    [webview loadHTMLString:@"" baseURL:nil];
    [webview stopLoading];
    [webview setDelegate:nil];
    
    
    [webview removeFromSuperview];
    
    [backButton removeFromSuperview];
    
    [useridToUse release];
    [urlToLoad release];
    [webview release];
    webview = nil;
    
    navigateToBaseScene();
    
    [self release];
}


@end
