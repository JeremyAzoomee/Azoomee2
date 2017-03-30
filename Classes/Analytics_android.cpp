#include "Analytics_android.h"
#include <AzoomeeCommon/StringFunctions.h>

using namespace Azoomee;


//-------------MIX PANEL ----------

void mixPanelSendEvent_android(std::string eventID)
{
    androidJNIHelper(eventID, "", "sendMixPanelWithEventID");
}

void mixPanelSendEvent_android(std::string eventID, std::map<std::string, std::string> map)
{
    androidJNIHelper(eventID, convertMapToJSONString(map), "sendMixPanelWithEventID");
}

void mixPanel_registerSuperProperties_android(std::map<std::string, std::string> map)
{
    androidJNIHelper(convertMapToJSONString(map), "sendMixPanelSuperProperties");
}

void mixPanel_RegisterIdentity(std::string ParentID)
{
    androidJNIHelper(ParentID, "sendMixPanelPeopleProperties");
}

//------------- APPSFLYER -------

void appsFlyerSendEvent_android(std::string eventID)
{
     androidJNIHelper(eventID, "", "sendAppsFlyerEvent");
}

void appsFlyerSendEvent_android(std::string eventID, std::map<std::string, std::string> map)
{
    androidJNIHelper(eventID, convertMapToJSONString(map), "sendAppsFlyerEvent");
}

//----------------- JNI HELPERS -------------------

void androidJNIHelper(std::string eventID, std::string propertiesJSONString, std::string JNIFunction)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    cocos2d::JniMethodInfo methodInfo;
    
    if (! cocos2d::JniHelper::getStaticMethodInfo(methodInfo, "org/cocos2dx/cpp/AppActivity", JNIFunction.c_str(), "(Ljava/lang/String;Ljava/lang/String;)V"))
    {
        return;
    }
    
    jstring jstringEventID= methodInfo.env->NewStringUTF(eventID.c_str());
    jstring jstringJSONProperties= methodInfo.env->NewStringUTF(propertiesJSONString.c_str());
    
    methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, jstringEventID,jstringJSONProperties);
    
    methodInfo.env->DeleteLocalRef(methodInfo.classID);
    
#endif
}

void androidJNIHelper(std::string propertiesJSONString, std::string JNIFunction)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    cocos2d::JniMethodInfo methodInfo;
    
    if (! cocos2d::JniHelper::getStaticMethodInfo(methodInfo, "org/cocos2dx/cpp/AppActivity", JNIFunction.c_str(), "(Ljava/lang/String;)V"))
    {
        return;
    }
    
    jstring jstringJSONProperties= methodInfo.env->NewStringUTF(propertiesJSONString.c_str());
    
    methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID,jstringJSONProperties);
    
    methodInfo.env->DeleteLocalRef(methodInfo.classID);
    
#endif
}

//------------- CONVERSION FUNCTIONS --------------

std::string convertMapToJSONString(std::map<std::string, std::string> map)
{
    std::string jsonString = "{";
    
    for (std::map<std::string, std::string>::iterator mapIterator = map.begin(); mapIterator != map.end(); ++mapIterator)
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
