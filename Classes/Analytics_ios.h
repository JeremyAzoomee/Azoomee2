#include "cocos2d.h"

//-----------MIXPANEL------------

void mixPanelSendEvent_ios(std::string eventID);
void mixPanelSendEvent_ios(std::string eventID, std::map<std::string, std::string> map);

void mixPanel_registerSuperProperties_ios(std::map<std::string, std::string> map);

void mixPanel_RegisterIdentity(std::string ParentID, std::map<std::string, std::string> name);

//-----------APPSFLYER-----------
void appsFlyerSendEvent_ios(std::string eventID);
void appsFlyerSendEvent_ios(std::string eventID, std::map<std::string, std::string> map);
