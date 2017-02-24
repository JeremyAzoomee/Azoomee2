#include "Analytics_ios.h"
#import "Mixpanel.h"
#import <UIKit/UIKit.h>
#import <AppsFlyerLib/AppsFlyerTracker.h>

NSDictionary* mapToNSDictionary(std::map<std::string, std::string> map)
{
    NSMutableDictionary * MutableDictionary = [NSMutableDictionary dictionary];
    
    NSString * key;
    NSString * value;
    
    for (auto it = map.begin(); it != map.end(); it++){
        key = (0 == it->first.length())?(@""):(@(it->first.c_str()));
        value = (0 == it->second.length())?(@""):(@(it->second.c_str()));
        
        if ([value rangeOfString:@"NUMBER|"].location == NSNotFound) {
            [MutableDictionary setObject:value forKey:key];
        }
        else
        {
            //value is an integer
            NSNumberFormatter *f = [[NSNumberFormatter alloc] init];
            NSNumber *myNumber = [f numberFromString:[[value componentsSeparatedByString:@"|"] objectAtIndex:1]];

            [MutableDictionary setValue:myNumber forKey:key];
        }
        
        key = nil;
        value = nil;
    }
    
    return [NSDictionary dictionaryWithDictionary:MutableDictionary];
}

void mixPanelSendEvent_ios(std::string eventID,std::map<std::string, std::string> map)
{
    [[Mixpanel sharedInstance] track:[NSString stringWithUTF8String:eventID.c_str()]
         properties:mapToNSDictionary(map)];
}

void mixPanelSendEvent_ios(std::string eventID)
{
    [[Mixpanel sharedInstance] track:[NSString stringWithUTF8String:eventID.c_str()]
                          properties:NULL];
}

void mixPanel_registerSuperProperties_ios(std::map<std::string, std::string> map)
{
    [[Mixpanel sharedInstance] registerSuperProperties:mapToNSDictionary(map)];
}

//--------------- APPSFLYER---------------

void appsFlyerSendEvent_ios(std::string eventID, std::map<std::string, std::string> map)
{
    [[AppsFlyerTracker sharedTracker] trackEvent: [NSString stringWithUTF8String:eventID.c_str()] withValues:mapToNSDictionary(map)];
}

void appsFlyerSendEvent_ios(std::string eventID)
{
    [[AppsFlyerTracker sharedTracker] trackEvent: [NSString stringWithUTF8String:eventID.c_str()] withValues:NULL];
}
