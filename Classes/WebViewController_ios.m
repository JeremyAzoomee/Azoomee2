//
//  ViewController.m
//  webviewComm
//
//  Created by Tamas Bonis on 20/01/2017.
//  Copyright © 2017 Tamas Bonis. All rights reserved.
//

#import "ViewController.h"

@interface ViewController ()

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
    
    UIWebView *webview=[[UIWebView alloc]initWithFrame:CGRectMake(20, 20, 300,600)];
    
    NSString *htmlFilePath = [[NSBundle mainBundle] pathForResource:@"webres/index" ofType:@"html"];
    NSURL *url = [NSURL fileURLWithPath:htmlFilePath];
    
    NSURLRequest *nsrequest = [NSURLRequest requestWithURL:url];
    
    
    //NSString *url=@"http://www.google.com";
    //NSURL *nsurl=[NSURL URLWithString:url];
    //NSURLRequest *nsrequest=[NSURLRequest requestWithURL:nsurl];
    
    [webview setDelegate:self];
    [webview loadRequest:nsrequest];
    [self.view addSubview:webview];
}

- (BOOL)webView:(UIWebView *)webView shouldStartLoadWithRequest:(NSURLRequest *)request navigationType:(UIWebViewNavigationType)navigationType {
    NSString *urlString = [[request URL] absoluteString];
    
    if ([urlString hasPrefix:@"senddata:"]) {
        NSURLComponents *urlComponents = [NSURLComponents componentsWithURL:request.URL resolvingAgainstBaseURL:NO];
        NSArray *queryItems = [urlComponents.string componentsSeparatedByString:@"data="];
        
        NSString *param1 = [queryItems lastObject];
        NSString *toWrite = [param1 stringByRemovingPercentEncoding];
        
        NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
        NSString *documentsDirectory = [paths objectAtIndex:0];
        
        NSString *filePath = [documentsDirectory stringByAppendingPathComponent:@"data.txt"];
        NSLog(@"%@", filePath);
        
        [toWrite writeToFile:filePath atomically:YES encoding:NSUTF8StringEncoding error:nil];
        
        // NOW CALL YOUR OBJECTIVE-C METHOD WITH param1 !
        
        return NO;
    }
    
    return YES;
}

-(void)webViewDidFinishLoad:(UIWebView *)webView
{
    [webView stringByEvaluatingJavaScriptFromString:@"setLocalData(\"Fuck you\")"];
}


- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}


@end
