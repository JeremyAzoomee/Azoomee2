#include "AnalyticsSingleton.h"
#include "../Utils/StringFunctions.h"
#include <cocos/cocos2d.h>
#include <cocos/platform/android/jni/JniHelper.h>

static const std::string kAzoomeeActivityJavaClassName = "org/cocos2dx/cpp/AppActivity";

namespace Azoomee
{
  
//------------- CONVERSION FUNCTIONS --------------

std::string convertMapToJSONString(const std::map<std::string, std::string>& map)
{
  std::string jsonString = "{";
  
  for (auto mapIterator = map.begin(); mapIterator != map.end(); ++mapIterator)
  {
    std::string structureAfterProperty = "";
    
    if(mapIterator != map.begin())
      structureAfterProperty = ",";
    
    
    std::string propertyData;
    
    //Only way found to pass integer to the Property JSON string
    if (mapIterator->second.find("NUMBER|") != std::string::npos) {
      propertyData = splitStringToVector(mapIterator->second, "|")[1];
    }
    else
      propertyData = cocos2d::StringUtils::format("\"%s\"",mapIterator->second.c_str());
    
    jsonString = cocos2d::StringUtils::format("%s%s\"%s\":%s",jsonString.c_str(),structureAfterProperty.c_str(),mapIterator->first.c_str(),propertyData.c_str());
  }
  
  jsonString = cocos2d::StringUtils::format("%s}",jsonString.c_str());
  
  return jsonString;
}
  
//----------------- JNI HELPERS -------------------

void androidJNIHelper(const std::string& eventID, const std::string& propertiesJSONString, const std::string& JNIFunction)
{
    cocos2d::JniHelper::callStaticVoidMethod(kAzoomeeActivityJavaClassName, JNIFunction, eventID, propertiesJSONString);
}

void androidJNIHelper(const std::string& propertiesJSONString, const std::string& JNIFunction)
{
    cocos2d::JniHelper::callStaticVoidMethod(kAzoomeeActivityJavaClassName, JNIFunction, propertiesJSONString);
}

//-------------MIX PANEL ----------
void AnalyticsSingleton::mixPanelSendEventNative(const std::string& eventID, const std::map<std::string, std::string>& map)
{
#ifndef AZOOMEE_ENVIRONMENT_CI
    androidJNIHelper(eventID, convertMapToJSONString(map), "sendMixPanelWithEventID");
#endif
	
}

void AnalyticsSingleton::mixPanelRegisterIdentity(const std::string& parentID, const std::map<std::string, std::string>& name)
{
#ifndef AZOOMEE_ENVIRONMENT_CI
    androidJNIHelper(parentID, "sendMixPanelPeopleProperties");
#endif
	
}

void AnalyticsSingleton::mixPanelRegisterAlias(const std::string &newId)
{
#ifndef AZOOMEE_ENVIRONMENT_CI
	androidJNIHelper(newId, "setMixpanelAlias");
#endif
	
}
	
void AnalyticsSingleton::mixPanelUpdatePeopleProfileData(const std::map<std::string, std::string>& profileData)
{
#ifndef AZOOMEE_ENVIRONMENT_CI
	androidJNIHelper(convertMapToJSONString(profileData), "updateMixpanelPeopleProperties");
#endif
	
}
	
//------------- APPSFLYER -------

void AnalyticsSingleton::appsFlyerSendEvent(const std::string& eventID)
{
#ifndef AZOOMEE_ENVIRONMENT_CI
    androidJNIHelper(eventID, "", "sendAppsFlyerEvent");
#endif
	
}

void AnalyticsSingleton::appsFlyerSendEvent(const std::string& eventID, const std::map<std::string, std::string>& map)
{
#ifndef AZOOMEE_ENVIRONMENT_CI
    androidJNIHelper(eventID, convertMapToJSONString(map), "sendAppsFlyerEvent");
#endif
	
}
  
}
