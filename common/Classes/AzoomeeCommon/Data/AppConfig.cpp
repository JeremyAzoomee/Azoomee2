//
//  AppConfig.cpp
//  AzoomeeCommon
//
//  Created by Macauley.Scoffins on 03/02/2020.
//

#include "AppConfig.h"
#include "../Device.h"
#include "../Utils/LocaleManager.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"
static const std::string kAzoomeeActivityJavaClassName = "org/cocos2dx/cpp/AppActivity";
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "../Utils/IosNativeFunctionsSingleton.h"
#endif

NS_AZOOMEE_BEGIN

const std::string AppConfig::kArtCacheFolder = "artCache/";

static std::auto_ptr<AppConfig> sAppConfigSharedInstance;

AppConfig* AppConfig::getInstance()
{
    if(!sAppConfigSharedInstance.get())
    {
        sAppConfigSharedInstance.reset(new AppConfig());
        sAppConfigSharedInstance->init();
    }
    return sAppConfigSharedInstance.get();
}

AppConfig::~AppConfig()
{
    
}

void AppConfig::init()
{
#ifdef AZOOMEE_ENVIRONMENT_CI
    _env = "ci";
#else
    _env = "prod";
#endif
    
    const std::string& configPath = "res/configuration/AppConfig.json";
    _appConfigData.Parse(cocos2d::FileUtils::getInstance()->getStringFromFile(configPath).c_str());
}
   
//API configuration
std::string AppConfig::getServerHost()
{
    return getStringFromJson("serverHost", _appConfigData[_env.c_str()]);
}

std::string AppConfig::getServerUrlPrefix()
{
    return getStringFromJson("serverUrlPrefix", _appConfigData[_env.c_str()]);
}

std::string AppConfig::getServerUrl()
{
    return getServerUrlPrefix() + getServerHost();
}

std::string AppConfig::getRemoteWebGameAPIPath()
{
    return getStringFromJson("webcomAPIUrl", _appConfigData[_env.c_str()]);
}
   
//Version configuration
std::string AppConfig::getVersionNumber()
{
    std::string versionNum = "0.0.0 (0)";
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    versionNum = IosNativeFunctionsSingleton::getInstance()->getAppVersionNum();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    versionNum = cocos2d::JniHelper::callStaticStringMethod(kAzoomeeActivityJavaClassName, "getAppVersionNum");
#endif
    return versionNum;
}

std::string AppConfig::getVersionNumberWithPlatform()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    return "Android/" + getVersionNumber();
#else
    return "IOS/" + getVersionNumber();
#endif
}

std::string AppConfig::getVersionNumberToDisplay()
{
    return _("Version Number") + " " + getVersionNumber();
}

std::string AppConfig::getVersionInformationForRequestHeader()
{
    return Device::getInstance()->getOSManufacturer() + "/" + getVersionNumberWithPlatform();
}
   
//Data download congiguration
std::string AppConfig::getLocaleZipUrl()
{
    return getStringFromJson("localeZipUrl", _appConfigData[_env.c_str()]);
}

NS_AZOOMEE_END
