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

- (void)startBuildingWebView:(NSString*)url userid:(NSString*)userid
{
    urlToLoad = url;
    useridToUse = userid;
    
    [urlToLoad retain];
    [useridToUse retain];
    
    [self addWebViewToScreen];
    //[self addWKWebViewToScreen];
    [self createButton];
}

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
    
}

- (void)addWKWebViewToScreen {
    WKWebViewConfiguration *theConfiguration = [[WKWebViewConfiguration alloc] init];
    WKWebView *webView = [[WKWebView alloc] initWithFrame:self.view.frame configuration:theConfiguration];
    
    
    NSString *iosurlExtension = [urlToLoad substringFromIndex:MAX((int)[urlToLoad length]-4, 0)];
    NSString *urlToCall;
    
    if([iosurlExtension isEqualToString:@"html"])
    {
        urlToCall = [[NSBundle mainBundle] pathForResource:@"res/webcommApi/index_ios" ofType:@"html"];
    }
    else
    {
        NSString *htmlFileAddress = [[NSBundle mainBundle] pathForResource:@"res/jwplayer/index_ios" ofType:@"html"];
        urlToCall = [NSString stringWithFormat:@"%@?contentUrl=%@", htmlFileAddress, urlToLoad];
    }
    
    webView.navigationDelegate = self;
    NSURL *nsurl=[NSURL URLWithString:urlToCall];
    NSURLRequest *nsrequest = [NSURLRequest requestWithURL:nsurl];
    
    [webView loadRequest:nsrequest];
    [self.view addSubview:webView];
}

- (void)addWebViewToScreen {
    if(webview) return;
    
    webview=[[UIWebView alloc]initWithFrame:CGRectMake(0, 0, self.view.frame.size.width, self.view.frame.size.height)];
    
    NSString *iosurlExtension = [urlToLoad substringFromIndex:MAX((int)[urlToLoad length]-4, 0)];
    NSString *urlToCall;
    
    if([iosurlExtension isEqualToString:@"html"])
    {
        urlToCall = [[NSBundle mainBundle] pathForResource:@"res/webcommApi/index_ios" ofType:@"html"];
    }
    else
    {
        NSString *htmlFileAddress = [[NSBundle mainBundle] pathForResource:@"res/jwplayer/index_ios" ofType:@"html"];
        urlToCall = [NSString stringWithFormat:@"%@?contentUrl=%@", htmlFileAddress, urlToLoad];
    }
    
    NSURL *nsurl=[NSURL URLWithString:urlToCall];
    NSURLRequest *nsrequest = [NSURLRequest requestWithURL:nsurl];
    
    [webview setAllowsInlineMediaPlayback:true];
    [webview setMediaPlaybackRequiresUserAction:NO];
    [webview scrollView].scrollEnabled = NO;
    [webview scrollView].bounces = NO;
    [webview setDelegate:self];
    [webview loadRequest:nsrequest];
    [self.view addSubview:webview];
}

- (BOOL)webView:(UIWebView *)webView shouldStartLoadWithRequest:(NSURLRequest *)request navigationType:(UIWebViewNavigationType)navigationType {
    NSString *urlString = [[request URL] absoluteString];
    
    if ([urlString hasPrefix:@"apirequest:"]) {
        
        NSURLComponents *urlComponents = [NSURLComponents componentsWithURL:request.URL resolvingAgainstBaseURL:NO];
        
        NSArray *urlItems = [urlComponents.string componentsSeparatedByString:@"?"];
        NSString *method = [urlItems objectAtIndex:2];
        NSString *responseId = [urlItems objectAtIndex:4];
        NSString *score = @"null";
        if([method isEqualToString:@"updateHighScore"]) score = [urlItems objectAtIndex:6];
        
        const char* returnString = sendGameApiRequest([method cStringUsingEncoding:NSUTF8StringEncoding], [responseId cStringUsingEncoding:NSUTF8StringEncoding], [score cStringUsingEncoding:NSUTF8StringEncoding]);
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
        NSString *iosurlExtension = [urlToLoad substringFromIndex:MAX((int)[urlToLoad length]-4, 0)];
        
        if([iosurlExtension isEqualToString:@"html"])
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
    backButton = [UIButton buttonWithType:UIButtonTypeCustom];
    [backButton addTarget:self action:@selector(buttonClicked:) forControlEvents:UIControlEventTouchUpInside];
    [backButton setFrame:CGRectMake(10, 10, 40, 40)];
    [backButton setExclusiveTouch:YES];
    [backButton setImage:[UIImage imageNamed:@"res/navigation/back_new.png"] forState:UIControlStateNormal];
    
    [self.view addSubview:backButton];
}

-(void) buttonClicked:(UIButton*)sender
{
    NSString *iosurlExtension = [urlToLoad substringFromIndex:MAX((int)[urlToLoad length]-4, 0)];
    if([iosurlExtension isEqualToString:@"html"])
    {
        NSString *htmlData = [webview stringByEvaluatingJavaScriptFromString:@"saveLocalDataBeforeExit()"];
        NSLog(@"ART APP FILE DATA DIRECT LENGTH: %lu", htmlData.length);
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
    
    NSString *iosurlExtension = [urlToLoad substringFromIndex:MAX((int)[urlToLoad length]-4, 0)];
    if(![iosurlExtension isEqualToString:@"html"]) return;
    
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
