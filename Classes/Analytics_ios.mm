#include "Analytics_ios.h"
#import "Mixpanel.h"
#import <UIKit/UIKit.h>
#import <AppsFlyerLib/AppsFlyerTracker.h>

void mixPanelSendEvent_ios(std::map<std::string, std::string> map, std::string eventID)
{
    NSMutableDictionary * MutableDictionary = [NSMutableDictionary dictionary];
    
    NSString * key;
    NSString * value;
    
    for (auto it = map.begin(); it != map.end(); it++){
        key = (0 == it->first.length())?(@""):(@(it->first.c_str()));
        value = (0 == it->second.length())?(@""):(@(it->second.c_str()));
        
        [MutableDictionary setObject:value forKey:key];
        NSLog(@"NSDictonaryFromMap() - key:%@ value:%@", key, value);
        
        key = nil;
        value = nil;
    }
    
    [[Mixpanel sharedInstance] track:[NSString stringWithUTF8String:eventID.c_str()]
         properties:[NSDictionary dictionaryWithDictionary:MutableDictionary]];
    
    MutableDictionary = nil;
}

void mixPanelSendEvent_ios(std::string eventID)
{
    [[Mixpanel sharedInstance] track:[NSString stringWithUTF8String:eventID.c_str()]
                          properties:NULL];
}

void mixPanel_registerSuperProperties_ios(std::map<std::string, std::string> map)
{
    NSMutableDictionary * MutableDictionary = [NSMutableDictionary dictionary];
    
    NSString * key;
    NSString * value;
    
    for (auto it = map.begin(); it != map.end(); it++){
        key = (0 == it->first.length())?(@""):(@(it->first.c_str()));
        value = (0 == it->second.length())?(@""):(@(it->second.c_str()));
        
        [MutableDictionary setObject:value forKey:key];
        NSLog(@"NSDictonaryFromMap() - key:%@ value:%@", key, value);
        
        key = nil;
        value = nil;
    }
    
    [[Mixpanel sharedInstance] registerSuperProperties:MutableDictionary];
    
    MutableDictionary = nil;
}

void appsFlyerSendEvent_ios(std::map<std::string, std::string> map, std::string eventID)
{
    NSMutableDictionary * MutableDictionary = [NSMutableDictionary dictionary];
    
    NSString * key;
    NSString * value;
    
    for (auto it = map.begin(); it != map.end(); it++){
        key = (0 == it->first.length())?(@""):(@(it->first.c_str()));
        value = (0 == it->second.length())?(@""):(@(it->second.c_str()));
        
        [MutableDictionary setObject:value forKey:key];
        NSLog(@"NSDictonaryFromMap() - key:%@ value:%@", key, value);
        
        key = nil;
        value = nil;
    }

    [[AppsFlyerTracker sharedTracker] trackEvent: [NSString stringWithUTF8String:eventID.c_str()] withValues:[NSDictionary dictionaryWithDictionary:MutableDictionary]];
}

void appsFlyerSendEvent_ios(std::string eventID)
{
    [[AppsFlyerTracker sharedTracker] trackEvent: [NSString stringWithUTF8String:eventID.c_str()] withValues:NULL];
}
