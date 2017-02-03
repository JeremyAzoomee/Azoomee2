#include "MixPanel_ios.h"
#import "Mixpanel.h"

void mixPanelSendiOSEvent(std::map<std::string, std::string> map, std::string eventID)
{
    [[Mixpanel sharedInstance] track:[NSString stringWithUTF8String:eventID.c_str()]
         properties:convertMapToNSDictionary(map)];
}

NSDictionary* convertMapToNSDictionary(std::map<std::string, std::string> map)
{
    NSMutableDictionary * MutableDictionary = [NSMutableDictionary dictionary];
    
    for (auto it = map.begin(); it != map.end(); it++){
        NSString * key = (0 == it->first.length())?(@""):(@(it->first.c_str()));
        NSString * value = (0 == it->second.length())?(@""):(@(it->second.c_str()));
        
        [MutableDictionary setObject:value forKey:key];
        NSLog(@"NSDictonaryFromMap() - key:%@ value:%@", key, value);
        
        key = nil;
        value = nil;
    }
    
    return [NSDictionary dictionaryWithDictionary:MutableDictionary];
}
