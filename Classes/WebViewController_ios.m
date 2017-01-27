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
    [self createButton];
}

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
    
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
        
        NSString *htmlFileAddress = [[NSBundle mainBundle] pathForResource:@"res/jwplayer/index" ofType:@"html"];
        urlToCall = [NSString stringWithFormat:@"%@?contentUrl=%@", htmlFileAddress, urlToLoad];
    }
    
    NSURL *nsurl=[NSURL URLWithString:urlToCall];
    NSURLRequest *nsrequest = [NSURLRequest requestWithURL:nsurl];
    
    [webview setDelegate:self];
    [webview loadRequest:nsrequest];
    [self.view addSubview:webview];
}

- (BOOL)webView:(UIWebView *)webView shouldStartLoadWithRequest:(NSURLRequest *)request navigationType:(UIWebViewNavigationType)navigationType {
    NSString *urlString = [[request URL] absoluteString];
    
    if ([urlString hasPrefix:@"senddata:"]) {
        
        NSURLComponents *urlComponents = [NSURLComponents componentsWithURL:request.URL resolvingAgainstBaseURL:NO];
        
        NSString *dataItem = [[urlComponents.string componentsSeparatedByString:@"?title?"] lastObject];
        
        NSString *title = [[[dataItem componentsSeparatedByString:@"?data?"] firstObject] stringByRemovingPercentEncoding];
        NSString *data = [[[dataItem componentsSeparatedByString:@"?data?"] lastObject] stringByRemovingPercentEncoding];
        
        NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
        NSString *writePath = [NSString stringWithFormat:@"%@/scoreCache/%@", [paths objectAtIndex:0], useridToUse];
        
        if(![[NSFileManager defaultManager] fileExistsAtPath:writePath])
        {
            [[NSFileManager defaultManager] createDirectoryAtPath:writePath withIntermediateDirectories:YES attributes:nil error:NULL];
        }
        
        NSString *fullFilePath = [NSString stringWithFormat:@"%@/scoreCache/%@/%@.json", [paths objectAtIndex:0], useridToUse, title];
        
        [data writeToFile:fullFilePath atomically:YES encoding:NSUTF8StringEncoding error:nil];
        
        NSLog(@"File written :%@", fullFilePath);
        
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
    
    return YES;
}

-(void)webViewDidFinishLoad:(UIWebView *)webView
{
    if(!iframeloaded)
    {
        //Clear local storage first!
        [webView stringByEvaluatingJavaScriptFromString:@"clearLocalStorage()"];
        
        //Get and add all local data to localstorage then
        NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
        NSString *scoreCacheDirectory = [NSString stringWithFormat:@"%@/scoreCache/%@", [paths objectAtIndex:0], useridToUse];
        NSLog(@"scorecache directory: %@", scoreCacheDirectory);
        
        if(![[NSFileManager defaultManager] fileExistsAtPath:scoreCacheDirectory])
        {
            [[NSFileManager defaultManager] createDirectoryAtPath:scoreCacheDirectory withIntermediateDirectories:YES attributes:nil error:NULL];
        }
        
        NSArray *directoryContent = [[NSFileManager defaultManager] contentsOfDirectoryAtPath:scoreCacheDirectory error:NULL];
        NSLog(@"amount of files in directory: %d", (int)[directoryContent count]);
        
        for(int i = 0; i < (int)[directoryContent count]; i++)
        {
            NSString *fullFilePath = [NSString stringWithFormat:@"%@/%@", scoreCacheDirectory, [directoryContent objectAtIndex:i]];
            
            NSLog(@"Directorycontent: %@", [directoryContent objectAtIndex:i]);
            NSString *title = [[directoryContent objectAtIndex:i] substringToIndex:[[directoryContent objectAtIndex:i] length] - 5];
            NSString *data = [NSString stringWithContentsOfFile:fullFilePath encoding:NSUTF8StringEncoding error:nil];
            
            NSString *addDataString = [NSString stringWithFormat:@"addDataToLocalStorage(\'%@\', \'%@\')", title, data];
            
            NSLog(@"%@", addDataString);
            
            [webView stringByEvaluatingJavaScriptFromString:addDataString];
        }
        
        NSString *loadString = [NSString stringWithFormat:@"addFrameWithUrl(\"%@\")", urlToLoad];
        [webView stringByEvaluatingJavaScriptFromString:loadString];
        
        //set iframe to true to avoid multiple loads
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
    [webview loadHTMLString:@"" baseURL:nil];
    [webview stopLoading];
    [webview setDelegate:nil];
    
    
    [webview removeFromSuperview];
    [currentButton removeFromSuperview];
    
    [useridToUse release];
    [urlToLoad release];
    [webview release];
    
    navigateToBaseScene();
    
    [self release];
}


@end
