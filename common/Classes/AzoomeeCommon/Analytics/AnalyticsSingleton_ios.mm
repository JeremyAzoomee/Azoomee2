#include "AnalyticsSingleton.h"
#import <Mixpanel/Mixpanel.h>
#import <UIKit/UIKit.h>
#import <AppsFlyerLib/AppsFlyerTracker.h>


namespace Azoomee
{
  
//----------------- CONVERSION FUNCTIONS -------------------

NSDictionary* mapToNSDictionary(const std::map<std::string, std::string>& map)
{
  NSMutableDictionary * mutableDictionary = [NSMutableDictionary dictionary];
  
  NSString * key;
  NSString * value;
  
  for (auto it = map.begin(); it != map.end(); it++){
    key = (0 == it->first.length())?(@""):(@(it->first.c_str()));
    value = (0 == it->second.length())?(@""):(@(it->second.c_str()));
    
    if ([value rangeOfString:@"NUMBER|"].location == NSNotFound) {
      [mutableDictionary setObject:value forKey:key];
    }
    else
    {
      //value is an integer
      NSNumberFormatter *f = [[NSNumberFormatter alloc] init];
      NSNumber *myNumber = [f numberFromString:[[value componentsSeparatedByString:@"|"] objectAtIndex:1]];
      
      [mutableDictionary setValue:myNumber forKey:key];
    }
    
    key = nil;
    value = nil;
  }
  
  return [NSDictionary dictionaryWithDictionary:mutableDictionary];
}
  
//--------------- MIXPANEL ---------------

void AnalyticsSingleton::mixPanelSendEvent(const std::string& eventID,const std::map<std::string, std::string>& map)
{
    [[Mixpanel sharedInstance] track:[NSString stringWithUTF8String:eventID.c_str()]
         properties:mapToNSDictionary(map)];
}

void AnalyticsSingleton::mixPanelSendEvent(const std::string& eventID)
{
    [[Mixpanel sharedInstance] track:[NSString stringWithUTF8String:eventID.c_str()]
                          properties:NULL];
}

void AnalyticsSingleton::mixPanelRegisterSuperProperties(const std::map<std::string, std::string>& map)
{
    [[Mixpanel sharedInstance] registerSuperProperties:mapToNSDictionary(map)];
}

void AnalyticsSingleton::mixPanelRegisterIdentity(const std::string& parentID, const std::map<std::string, std::string>& name)
{
    [[Mixpanel sharedInstance] identify:[NSString stringWithUTF8String:parentID.c_str()]];
    // [[Mixpanel sharedInstance].people set:mapToNSDictionary(name)];
}

//--------------- APPSFLYER ---------------

void AnalyticsSingleton::appsFlyerSendEvent(const std::string& eventID, const std::map<std::string, std::string>& map)
{
    [[AppsFlyerTracker sharedTracker] trackEvent: [NSString stringWithUTF8String:eventID.c_str()] withValues:mapToNSDictionary(map)];
}

void AnalyticsSingleton::appsFlyerSendEvent(const std::string& eventID)
{
    [[AppsFlyerTracker sharedTracker] trackEvent: [NSString stringWithUTF8String:eventID.c_str()] withValues:NULL];
}
  
}
