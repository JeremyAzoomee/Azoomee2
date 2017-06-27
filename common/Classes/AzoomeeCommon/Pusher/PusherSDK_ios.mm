#include "PusherSDK.h"
#include "../API/API.h"
#include "../Data/Parent/ParentDataProvider.h"
#import <Foundation/Foundation.h>
#import <PusherSwift/PusherSwift-Swift.h>

using namespace Azoomee;


#pragma mark - Pusher (Objective-C)

@interface PusheriOSDelegate : NSObject <PusherDelegate>

+ (PusheriOSDelegate*) sharedInstance;

- (void) debugLogWithMessage:(NSString* _Nonnull)message;
- (void) subscribedToInterestWithName:(NSString* _Nonnull)name;
- (void) unsubscribedFromInterestWithName:(NSString* _Nonnull)name;
- (void) changedConnectionStateFrom:(enum ConnectionState)old to:(enum ConnectionState)new_;
- (void) subscribedToChannelWithName:(NSString* _Nonnull)name;
- (void) failedToSubscribeToChannelWithName:(NSString* _Nonnull)name response:(NSURLResponse* _Nullable)response data:(NSString* _Nullable)data error:(NSError* _Nullable)error;

@end

@implementation PusheriOSDelegate

+ (PusheriOSDelegate*) sharedInstance
{
    static PusheriOSDelegate* instance = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        instance = [[PusheriOSDelegate alloc] init];
    });
    return instance;
}

- (void) debugLogWithMessage:(NSString* _Nonnull)message
{
    NSLog(@"PusheriOSDelegate.debugLogWithMessage: %@", message);
}

- (void) subscribedToInterestWithName:(NSString* _Nonnull)name
{
    NSLog(@"PusheriOSDelegate.subscribedToInterestWithName: %@", name);
}

- (void) unsubscribedFromInterestWithName:(NSString* _Nonnull)name
{
    NSLog(@"PusheriOSDelegate.unsubscribedFromInterestWithName: %@", name);
}

- (void) changedConnectionStateFrom:(enum ConnectionState)old to:(enum ConnectionState)new_
{
    NSLog(@"PusheriOSDelegate.subscribedToInterestWithName: old=%ld, new=%ld", (long)old, (long)new_);
}

- (void) subscribedToChannelWithName:(NSString* _Nonnull)name
{
    NSLog(@"PusheriOSDelegate.subscribedToChannelWithName: %@", name);
}

- (void) failedToSubscribeToChannelWithName:(NSString* _Nonnull)name response:(NSURLResponse* _Nullable)response data:(NSString* _Nullable)data error:(NSError* _Nullable)error
{
    NSLog(@"PusheriOSDelegate.failedToSubscribeToChannelWithName: %@, error=%@", name, error);
}

@end

@interface AzoomeePusherAuthRequestBuilder : NSObject <AuthRequestBuilderProtocol>

- (NSURLRequest*) requestForSocketID:(NSString*)socketID channelName:(NSString*)channelName;

@end

@implementation AzoomeePusherAuthRequestBuilder

- (NSURLRequest*) requestForSocketID:(NSString*)socketID channelName:(NSString*)channelName
{
    // Create the Azoomee signed request
    ParentDataProvider* parentData = ParentDataProvider::getInstance();
    HttpRequestCreator* httpReqCreator = API::PusherAuthRequest(parentData->getLoggedInParentId(), [channelName UTF8String], [socketID UTF8String], nullptr);
    
    // Convert cocos HttpRequest to a NSURLRequest
    cocos2d::network::HttpRequest* httpRequest = httpReqCreator->buildHttpRequest();
    const char* url = httpRequest->getUrl();
    const std::vector<std::string>& headers = httpRequest->getHeaders();
    cocos2d::network::HttpRequest::Type method = httpRequest->getRequestType();
    
    // Build the NSURLRequest
    NSMutableURLRequest* request = [NSMutableURLRequest requestWithURL:[NSURL URLWithString:[NSString stringWithUTF8String:url]]];
    request.HTTPMethod = (method == cocos2d::network::HttpRequest::Type::POST) ? @"POST" : @"GET";
    if([request.HTTPMethod isEqualToString:@"POST"])
    {
        request.HTTPBody = [NSData dataWithBytes:httpRequest->getRequestData() length:httpRequest->getRequestDataSize()];
    }
    for(const std::string& header : headers)
    {
        size_t splitIndex = header.find_first_of(":");
        const std::string& key = header.substr(0, splitIndex);
        const std::string& value = header.substr(splitIndex + 1, header.length() - splitIndex);
        [request addValue:[NSString stringWithUTF8String:key.c_str()] forHTTPHeaderField:[NSString stringWithUTF8String:value.c_str()]];
    }
    
    return request;
}

@end

static Pusher* sPusherClient = nil;

@interface PusheriOS : NSObject

+ (Pusher*) initialise:(NSString*)apiKey;

@end

@implementation PusheriOS

+ (Pusher*) initialise:(NSString*)apiKey
{
    OCAuthMethod* authMethod = [[OCAuthMethod alloc] initWithAuthRequestBuilder:[[AzoomeePusherAuthRequestBuilder alloc] init]];
    OCPusherHost* host = [[OCPusherHost alloc] initWithCluster:@"eu"];
    PusherClientOptions* options = [[PusherClientOptions alloc]
                                    initWithOcAuthMethod:authMethod
                                    attemptToReturnJSONObject:YES
                                    autoReconnect:YES
                                    ocHost:host
                                    port:nil
                                    encrypted:YES];
    
    Pusher* pusherClient = [[Pusher alloc] initWithAppKey:apiKey options:options];
    pusherClient.delegate = [PusheriOSDelegate sharedInstance];
    
    [pusherClient bind:^void (NSDictionary *data) {
        NSLog(@"Pusher.client.callback: data=%@", data);
    }];
    
    [pusherClient connect];
    return pusherClient;
}

@end


#pragma mark - PusherSDK


NS_AZOOMEE_BEGIN

void PusherSDK::subscribeToChannel(const std::string& channelName)
{
    if(!sPusherClient)
    {
        sPusherClient = [PusheriOS initialise:[NSString stringWithUTF8String:_appKey.c_str()]];
    }
    
    _subscribedChannels.push_back(channelName);
    
    PusherChannel* channel = [sPusherClient subscribeWithChannelName:[NSString stringWithUTF8String:channelName.c_str()]];
    [channel bindWithEventName:@"message" callback:^void (NSDictionary *data) {
        NSString* message = data[@"message"];
        NSLog(@"PusherSDK.bindWithEventName.callback: message=%@", message);
    }];
}

void PusherSDK::closeChannel(const std::string& channelName)
{
    if(!sPusherClient)
    {
        return;
    }
    
    [sPusherClient unsubscribe:[NSString stringWithUTF8String:channelName.c_str()]];
    
    const auto& it = std::find(_subscribedChannels.begin(), _subscribedChannels.end(), channelName);
    if(it != _subscribedChannels.end())
    {
        _subscribedChannels.erase(it);
    }
}


NS_AZOOMEE_END
