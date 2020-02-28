//
//  AppConfig.cpp
//  TinizineCommon
//
//  Created by Macauley.Scoffins on 03/02/2020.
//

#include "AppConfig.h"
#include "../Device.h"
#include "../Utils/LocaleManager.h"

NS_TZ_BEGIN

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
    return TZ::Device::getInstance()->getAppVersion();
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
    return TZ::Device::getInstance()->getOSManufacturer() + "/" + getVersionNumberWithPlatform();
}
   
//Data download congiguration
std::string AppConfig::getLocaleZipUrl()
{
    return getStringFromJson("localeZipUrl", _appConfigData[_env.c_str()]);
}

NS_TZ_END
