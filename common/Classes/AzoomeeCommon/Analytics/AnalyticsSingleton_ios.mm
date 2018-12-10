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

void AnalyticsSingleton::mixPanelSendEventNative(const std::string& eventID,const std::map<std::string, std::string>& map)
{
#ifndef USINGCI
	 [[Mixpanel sharedInstance] track:[NSString stringWithUTF8String:eventID.c_str()]
						  properties:mapToNSDictionary(map)];
#endif
	
}

void AnalyticsSingleton::mixPanelRegisterIdentity(const std::string& parentID, const std::map<std::string, std::string>& name)
{
#ifndef USINGCI
	[[Mixpanel sharedInstance] identify:[NSString stringWithUTF8String:parentID.c_str()]];
	[[Mixpanel sharedInstance].people set:mapToNSDictionary(name)];
#endif
}
	
void AnalyticsSingleton::mixPanelRegisterAlias(const std::string &newId)
{
#ifndef USINGCI
	[[Mixpanel sharedInstance] createAlias:[NSString stringWithUTF8String:newId.c_str()] forDistinctID:[Mixpanel sharedInstance].distinctId];
	[[Mixpanel sharedInstance] identify:[Mixpanel sharedInstance].distinctId];
#endif
	
}
	
void AnalyticsSingleton::mixPanelUpdatePeopleProfileData(const std::map<std::string, std::string>& profileData)
{
#ifndef USINGCI
	[[Mixpanel sharedInstance].people set:mapToNSDictionary(profileData)];
#endif
	
}

//--------------- APPSFLYER ---------------

void AnalyticsSingleton::appsFlyerSendEvent(const std::string& eventID, const std::map<std::string, std::string>& map)
{
#ifndef USINGCI
    [[AppsFlyerTracker sharedTracker] trackEvent: [NSString stringWithUTF8String:eventID.c_str()] withValues:mapToNSDictionary(map)];
#endif
}

void AnalyticsSingleton::appsFlyerSendEvent(const std::string& eventID)
{
#ifndef USINGCI
	[[AppsFlyerTracker sharedTracker] trackEvent: [NSString stringWithUTF8String:eventID.c_str()] withValues:NULL];
#endif
}
  
}
