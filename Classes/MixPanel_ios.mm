#include "MixPanel_ios.h"
#import "Mixpanel.h"
#import <UIKit/UIKit.h>
#import <AppsFlyerLib/AppsFlyerTracker.h>

void mixPanelSendiOSEvent(std::map<std::string, std::string> map, std::string eventID)
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

void mixPanelSendiOSEvent(std::string eventID)
{
    [[Mixpanel sharedInstance] track:[NSString stringWithUTF8String:eventID.c_str()]
                          properties:NULL];
}

void mixPanel_registerSuperProperties_iOS(std::map<std::string, std::string> map)
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

void appsFlyerSendiOSEvent(std::map<std::string, std::string> map, std::string eventID)
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

void appsFlyerSendiOSEvent(std::string eventID)
{
    [[AppsFlyerTracker sharedTracker] trackEvent: [NSString stringWithUTF8String:eventID.c_str()] withValues:NULL];
}
