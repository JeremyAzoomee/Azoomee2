#include "cocos2d.h"

//-------------MIX PANEL ----------

void mixPanelSendEvent_android(std::string eventID);
void mixPanelSendEvent_android(std::string eventID, std::map<std::string, std::string> map);

void mixPanel_registerSuperProperties_android(std::map<std::string, std::string> map);

//------------- APPSFLYER -------

void appsFlyerSendEvent_android(std::string eventID);
void appsFlyerSendEvent_android(std::string eventID, std::map<std::string, std::string> map);


//------------- JNI HELPER FUNCTIONS --------------

void androidJNIHelper(std::string eventID, std::string JNIFunction);
void androidJNIHelper(std::string eventID, std::string propertiesJSONString, std::string JNIFunction);

//------------- CONVERSION FUNCTIONS --------------

std::string convertMapToJSONString(std::map<std::string, std::string> map);
