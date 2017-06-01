#include "AnalyticsSingleton.h"
#include "../Utils/StringFunctions.h"
#include <cocos/cocos2d.h>


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
  cocos2d::JniMethodInfo methodInfo;
  
  if (! cocos2d::JniHelper::getStaticMethodInfo(methodInfo, "org/cocos2dx/cpp/AppActivity", JNIFunction.c_str(), "(Ljava/lang/String;Ljava/lang/String;)V"))
  {
    return;
  }
  
  jstring jstringEventID= methodInfo.env->NewStringUTF(eventID.c_str());
  jstring jstringJSONProperties= methodInfo.env->NewStringUTF(propertiesJSONString.c_str());
  
  methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, jstringEventID,jstringJSONProperties);
  
  methodInfo.env->DeleteLocalRef(methodInfo.classID);
}

void androidJNIHelper(const std::string& propertiesJSONString, const std::string& JNIFunction)
{
  cocos2d::JniMethodInfo methodInfo;
  
  if (! cocos2d::JniHelper::getStaticMethodInfo(methodInfo, "org/cocos2dx/cpp/AppActivity", JNIFunction.c_str(), "(Ljava/lang/String;)V"))
  {
    return;
  }
  
  jstring jstringJSONProperties= methodInfo.env->NewStringUTF(propertiesJSONString.c_str());
  
  methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID,jstringJSONProperties);
  
  methodInfo.env->DeleteLocalRef(methodInfo.classID);
}

//-------------MIX PANEL ----------

void AnalyticsSingleton::mixPanelSendEvent(const std::string& eventID)
{
#ifdef USINGCI
    return;
#endif
    androidJNIHelper(eventID, "", "sendMixPanelWithEventID");
}

void AnalyticsSingleton::mixPanelSendEvent(const std::string& eventID, const std::map<std::string, std::string>& map)
{
#ifdef USINGCI
    return;
#endif
    androidJNIHelper(eventID, convertMapToJSONString(map), "sendMixPanelWithEventID");
}

void AnalyticsSingleton::mixPanelRegisterSuperProperties(const std::map<std::string, std::string>& map)
{
#ifdef USINGCI
    return;
#endif
    androidJNIHelper(convertMapToJSONString(map), "sendMixPanelSuperProperties");
}

void AnalyticsSingleton::mixPanelRegisterIdentity(const std::string& parentID, const std::map<std::string, std::string>& name)
{
#ifdef USINGCI
    return;
#endif
    androidJNIHelper(parentID, "sendMixPanelPeopleProperties");
}

//------------- APPSFLYER -------

void AnalyticsSingleton::appsFlyerSendEvent(const std::string& eventID)
{
#ifdef USINGCI
    return;
#endif
     androidJNIHelper(eventID, "", "sendAppsFlyerEvent");
}

void AnalyticsSingleton::appsFlyerSendEvent(const std::string& eventID, const std::map<std::string, std::string>& map)
{
#ifdef USINGCI
    return;
#endif
    androidJNIHelper(eventID, convertMapToJSONString(map), "sendAppsFlyerEvent");
}
  
}
