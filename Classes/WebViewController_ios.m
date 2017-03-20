//
//  ViewController.m
//  webviewComm
//
//  Created by Tamas Bonis on 20/01/2017.
//  Copyright © 2017 Tamas Bonis. All rights reserved.
//

#import "WebViewController_ios.h"
#import "ios_Cocos2d_Callbacks.h"

@interface WebViewController ()

@end

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
        iframeloaded = 1;
        
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
    webview=[[UIWebView alloc]initWithFrame:CGRectMake(0, 0, self.view.frame.size.width, self.view.frame.size.height)];
    
    NSString *iosurlExtension = [urlToLoad substringFromIndex:MAX((int)[urlToLoad length]-4, 0)];
    NSString *urlToCall;
    
    if([iosurlExtension isEqualToString:@"html"])
    {
        urlToCall = [[NSBundle mainBundle] pathForResource:@"res/webcommApi/index_ios" ofType:@"html"];
    }
    else
    {
        iframeloaded = 1;
        
        NSString *htmlFileAddress = [[NSBundle mainBundle] pathForResource:@"res/jwplayer/index_ios" ofType:@"html"];
        urlToCall = [NSString stringWithFormat:@"%@?contentUrl=%@", htmlFileAddress, urlToLoad];
    }
    
    NSURL *nsurl=[NSURL URLWithString:urlToCall];
    NSURLRequest *nsrequest = [NSURLRequest requestWithURL:nsurl];
    
    [webview scrollView].scrollEnabled = NO;
    [webview scrollView].bounces = NO;
    [webview setDelegate:self];
    [webview loadRequest:nsrequest];
    [self.view addSubview:webview];
}

- (BOOL)webView:(UIWebView *)webView shouldStartLoadWithRequest:(NSURLRequest *)request navigationType:(UIWebViewNavigationType)navigationType {
    NSString *urlString = [[request URL] absoluteString];
    
    if ([urlString hasPrefix:@"savelocaldata:"]) {
        
        NSLog(@"savelocaldatawascalled!");
        
        NSURLComponents *urlComponents = [NSURLComponents componentsWithURL:request.URL resolvingAgainstBaseURL:NO];
        
        NSArray *urlItems = [urlComponents.string componentsSeparatedByString:@"?"];
        NSString *localStorageData = [urlItems objectAtIndex:1];
        
        NSLog(@"second part of the local data is: %@", localStorageData);
        
        const char* localStorageDataChar = [localStorageData cStringUsingEncoding:NSUTF8StringEncoding];
        saveLocalStorageData(localStorageDataChar);
        
        [self finishView];
        
        return NO;
    }
    
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
    
    if ([urlString hasPrefix:@"saveimage:"]) {
        
        NSURLComponents *urlComponents = [NSURLComponents componentsWithURL:request.URL resolvingAgainstBaseURL:NO];
        
        NSString *dataItem = [[urlComponents.string componentsSeparatedByString:@"?title?"] lastObject];
        
        NSString *title = [[[dataItem componentsSeparatedByString:@"?data?"] firstObject] stringByRemovingPercentEncoding];
        NSString *data = [[[dataItem componentsSeparatedByString:@"?data?"] lastObject] stringByRemovingPercentEncoding];
        
        NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
        NSString *writePath = [NSString stringWithFormat:@"%@/artCache/%@", [paths objectAtIndex:0], useridToUse];
        
        if(![[NSFileManager defaultManager] fileExistsAtPath:writePath])
        {
            [[NSFileManager defaultManager] createDirectoryAtPath:writePath withIntermediateDirectories:YES attributes:nil error:NULL];
        }
        
        NSString *fullFilePath = [NSString stringWithFormat:@"%@/artCache/%@/%@", [paths objectAtIndex:0], useridToUse, title];
        [data writeToFile:fullFilePath atomically:YES encoding:NSUTF8StringEncoding error:nil];
        
        return NO;
    }
    
    if ([urlString hasPrefix:@"videoerror"])
    {
        [webview loadHTMLString:@"" baseURL:nil];
        [webview stopLoading];
        [webview setDelegate:nil];
        
        
        [webview removeFromSuperview];
        
        [currentButton removeFromSuperview];
        
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
    
    return YES;
}

-(void)webViewDidFinishLoad:(UIWebView *)webView
{
    if(!iframeloaded)
    {
        [webView stringByEvaluatingJavaScriptFromString:@"clearLocalStorage()"];
            
        NSString *addDataString = [NSString stringWithFormat:@"addDataToLocalStorage(\"%s\")", getLocalStorageForGame()];
        [webView stringByEvaluatingJavaScriptFromString:addDataString];
        
        NSString *loadString = [NSString stringWithFormat:@"addFrameWithUrl(\"%@\")", urlToLoad];
        [webView stringByEvaluatingJavaScriptFromString:loadString];
        
        iframeloaded = true;
    };
}


- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void) createButton
{
    currentButton = [UIButton buttonWithType:UIButtonTypeCustom];
    [currentButton addTarget:self action:@selector(buttonClicked:) forControlEvents:UIControlEventTouchUpInside];
    [currentButton setFrame:CGRectMake(10, 10, 40, 40)];
    [currentButton setExclusiveTouch:YES];
    [currentButton setImage:[UIImage imageNamed:@"res/navigation/back_new.png"] forState:UIControlStateNormal];
    
    [self.view addSubview:currentButton];
}

-(void) buttonClicked:(UIButton*)sender
{
    NSLog(@"before exit is here!");
    [webview stringByEvaluatingJavaScriptFromString:@"saveLocalDataBeforeExit()"];
}

-(void) finishView
{
    [webview loadHTMLString:@"" baseURL:nil];
    [webview stopLoading];
    [webview setDelegate:nil];
    
    
    [webview removeFromSuperview];
    
    [currentButton removeFromSuperview];
    
    [useridToUse release];
    [urlToLoad release];
    [webview release];
    webview = nil;
    
    navigateToBaseScene();
    
    [self release];
}


@end
