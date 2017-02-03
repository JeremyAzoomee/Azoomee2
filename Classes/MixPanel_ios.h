#include "cocos2d.h"
#import <UIKit/UIKit.h>

void mixPanelSendiOSEvent(std::map<std::string, std::string> map, std::string eventID);
NSDictionary* convertMapToNSDictionary(std::map<std::string, std::string> map);
