#include "cocos2d.h"

void mixPanelSendEvent_android(std::map<std::string, std::string> map, std::string eventID);
void mixPanelSendEvent_android(std::string eventID);

void mixPanel_registerSuperProperties_android(std::map<std::string, std::string> map);

void appsFlyerSendEvent_android(std::map<std::string, std::string> map, std::string eventID);

void appsFlyerSendEvent_android(std::string eventID);

//------------- JNI HELPER FUNCTIONS --------------

void mixPanel_androidJNIHelper(std::string eventID, std::string propertiesJSONString);

void mixPanel_SuperProperties_androidJNIHelper(std::map<std::string, std::string> map);

void appsFlyer_androidJNIHelper(std::string eventID, std::string propertiesJSONString);

//------------- CONVERSION FUNCTIONS --------------

std::string convertMapToJSONString(std::map<std::string, std::string> map);
