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
    [self createButtons];
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
    [backButton setImage:[UIImage imageNamed:@"res/webview_buttons/close_unselected.png"] forState:UIControlStateNormal];
    [backButton setImage:[UIImage imageNamed:@"res/webview_buttons/close_selected.png"] forState:UIControlStateSelected];
    if(!isAnonUser())
    {
        _burgerButton = [UIButton buttonWithType:UIButtonTypeCustom];
        [_burgerButton addTarget:self action:@selector(buttonClicked:) forControlEvents:UIControlEventTouchUpInside];
        [_burgerButton setFrame:CGRectMake(buttonPaddingX + (screenSize.width * _closeButtonAnchorX) + (xMod * _buttonWidth), buttonPaddingY + (screenSize.height * _closeButtonAnchorY) + (yMod * _buttonWidth), buttonWidth, buttonWidth)];
        [_burgerButton setExclusiveTouch:YES];
        [_burgerButton setImage:[UIImage imageNamed:@"res/webview_buttons/menu_unselected.png"] forState:UIControlStateNormal];
        [_burgerButton setImage:[UIImage imageNamed:@"res/webview_buttons/menu_selected.png"] forState:UIControlStateSelected];
        
        _favButton = [UIButton buttonWithType:UIButtonTypeCustom];
        [_favButton addTarget:self action:@selector(buttonClicked:) forControlEvents:UIControlEventTouchUpInside];
        [_favButton setFrame:CGRectMake(buttonPaddingX + (screenSize.width * _closeButtonAnchorX) + (xMod * _buttonWidth), buttonPaddingY + (screenSize.height * _closeButtonAnchorY) + (yMod * _buttonWidth), buttonWidth, buttonWidth)];
        [_favButton setExclusiveTouch:YES];
        [_favButton setImage:[UIImage imageNamed:@"res/webview_buttons/favourite_unselected.png"] forState:UIControlStateNormal];
        [_favButton setImage:[UIImage imageNamed:@"res/webview_buttons/favourite_selected.png"] forState:UIControlStateSelected];
        if(isFavContent())
        {
            [_favButton setSelected: true];
        }
        
        if(isChatEntitled())
        {
            _shareButton = [UIButton buttonWithType:UIButtonTypeCustom];
            [_shareButton addTarget:self action:@selector(buttonClicked:) forControlEvents:UIControlEventTouchUpInside];
            [_shareButton setFrame:CGRectMake(buttonPaddingX + (screenSize.width * _closeButtonAnchorX) + (xMod * _buttonWidth), buttonPaddingY + (screenSize.height * _closeButtonAnchorY) + (yMod * _buttonWidth), buttonWidth, buttonWidth)];
            [_shareButton setExclusiveTouch:YES];
            [_shareButton setImage:[UIImage imageNamed:@"res/webview_buttons/share_unselected.png"] forState:UIControlStateNormal];
            [_shareButton setImage:[UIImage imageNamed:@"res/webview_buttons/share_selected.png"] forState:UIControlStateSelected];
            
            [self.view addSubview:_shareButton];
        }
        [self.view addSubview:_favButton];
        [self.view addSubview:_burgerButton];
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
            NSString *htmlData = [webview stringByEvaluatingJavaScriptFromString:@"saveLocalDataBeforeExit()"];
            saveLocalStorageData(htmlData);
            [self finishView];
        }
        else
        {
            [self finishView];
        }
        shareContentInChat();
    }
    else if(sender == _burgerButton)
    {
        [self animateButtons];
    }
}

-(void) removeWebViewWhileInBackground
{
    [backButton removeFromSuperview];
    if(!isAnonUser())
    {
        [_favButton removeFromSuperview];
        if(isChatEntitled())
        {
            [_shareButton removeFromSuperview];
        }
        [_burgerButton removeFromSuperview];
    }
    
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
    if(!isAnonUser())
    {
        [_favButton removeFromSuperview];
        if(isChatEntitled())
        {
            [_shareButton removeFromSuperview];
        }
        [_burgerButton removeFromSuperview];
    }
    
    [useridToUse release];
    [urlToLoad release];
    [webview release];
    webview = nil;
    
    navigateToBaseScene();
    
    [self release];
}

-(void) animateButtons
{
    CGSize screenSize = [[UIScreen mainScreen] bounds].size;
    screenSize.width -= _buttonWidth * 1.5f;
    screenSize.height -= _buttonWidth * 1.5f;
    
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
    
    CGFloat buttonPaddingX = _buttonWidth / 4.0f;
    CGFloat buttonPaddingY = _buttonWidth / 4.0f;
    
    if(isDeviceIphoneX() && screenSize.width > screenSize.height)
    {
        buttonPaddingX += ((_closeButtonAnchorX - 0.5) * 2.0f) * (-25.0f);
    }
    
    if(_uiExpanded)
    {
        // animate
        [UIView animateWithDuration:0.5 animations:^{
            _favButton.frame = CGRectMake(buttonPaddingX + (screenSize.width * _closeButtonAnchorX) + (xMod * _buttonWidth), buttonPaddingY + (screenSize.height * _closeButtonAnchorY) + (yMod * _buttonWidth), _buttonWidth, _buttonWidth);
            if(isChatEntitled())
            {
                _shareButton.frame = CGRectMake(buttonPaddingX + (screenSize.width * _closeButtonAnchorX) + (xMod * _buttonWidth), buttonPaddingY + (screenSize.height * _closeButtonAnchorY) + (yMod * _buttonWidth), _buttonWidth, _buttonWidth);
            }
        }];
        _uiExpanded = false;
    }
    else
    {
        // animate
        [UIView animateWithDuration:0.5 animations:^{
            _favButton.frame = CGRectMake(buttonPaddingX + (screenSize.width * _closeButtonAnchorX) + 2 * (xMod * _buttonWidth), buttonPaddingY + (screenSize.height * _closeButtonAnchorY) + 2 * ( yMod * _buttonWidth), _buttonWidth, _buttonWidth);
            if(isChatEntitled())
            {
                _shareButton.frame = CGRectMake(buttonPaddingX + (screenSize.width * _closeButtonAnchorX) + 3 * (xMod * _buttonWidth), buttonPaddingY + (screenSize.height * _closeButtonAnchorY) + 3 * ( yMod * _buttonWidth), _buttonWidth, _buttonWidth);
            }
        }];
        _uiExpanded = true;
    }
    
}


@end
