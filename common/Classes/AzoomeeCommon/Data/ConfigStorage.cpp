//ConfigStorage is a singleton designed to hold all necessary "burnt-in" information for all visual scenes and layers.

#include "ConfigStorage.h"
#include "Parent/UserAccountManager.h"
#include "../Analytics/AnalyticsSingleton.h"
#include "../API/API.h"
#include "../Net/Utils.h"
#include "Json.h"
#include "../Utils/StringFunctions.h"
#include "../Utils/DirUtil.h"
#include "../Utils/LocaleManager.h"
#include "../Device.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"

static const std::string kAzoomeeActivityJavaClassName = "org/cocos2dx/cpp/AppActivity";
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "../Utils/IosNativeFunctionsSingleton.h"
#endif

using namespace cocos2d;

namespace Azoomee
{
static ConfigStorage *_sharedConfigStorage = NULL;
    
    const char* const ConfigStorage::kGroupHQName = "GROUP HQ";

	const char* const ConfigStorage::kIosWebviewName = "iosWebView";
	const char* const ConfigStorage::kAndroidWebviewName = "androidWebView";
    
    const std::string ConfigStorage::kArtCacheFolder = "artCache/";
	const std::string ConfigStorage::kOomeeMakerCacheFolder = "oomeeMaker/";
    
    const std::string ConfigStorage::kAvatarImageCacheFolder = "avatars";
   
    
    
ConfigStorage* ConfigStorage::getInstance()
{
    if (! _sharedConfigStorage)
    {
        _sharedConfigStorage = new ConfigStorage();
        _sharedConfigStorage->init();
    }
    
    return _sharedConfigStorage;
}

ConfigStorage::~ConfigStorage(void)
{
}

bool ConfigStorage::init(void)
{
    
    VersionConfiguration = parseJsonConfigurationFile("Version.json");
    
    return true;
}
//-------------------------PRIVATE METHOD TO PARSE CONFIG JSON FILE--------

rapidjson::Document ConfigStorage::parseJsonConfigurationFile(const std::string& fileName)
{
    std::string path = "res/configuration/" + fileName;
    
    std::string jsonString;
    
    if(!FileUtils::getInstance()->isFileExist(path)) jsonString = "";
    else jsonString = FileUtils::getInstance()->getStringFromFile(path);
    
    rapidjson::Document returnObject;
    returnObject.Parse(jsonString.c_str());
    
    if(returnObject.HasParseError())
    {
        cocos2d::log("The following file has got a json parse error: %s", fileName.c_str());
        returnObject.SetObject();
        return returnObject;
    }
    else
    {
        return returnObject;
    }
}

//-------------------------BACKEND CALLER CONFIGURATION--------------------
std::string ConfigStorage::getServerHost()
{
#ifdef AZOOMEE_ENVIRONMENT_CI
    return "api.azoomee.ninja";
#endif
    return "api.azoomee.com";
}
    
std::string ConfigStorage::getServerUrlPrefix()
{
#ifdef AZOOMEE_ENVIRONMENT_CI
    return "http://";
#endif
    return "https://";
}

std::string ConfigStorage::getServerUrl()
{
    return getServerUrlPrefix() + getServerHost();
}
    
std::string ConfigStorage::getRemoteWebGameAPIPath()
{
#ifdef AZOOMEE_ENVIRONMENT_CI
    return "https://media.azoomee.ninja/static/webgameapi/";
#else
    return "https://media.azoomee.com/static/webgameapi/";
#endif
}

//---------------------------- Version configuration --------------------------------

std::string ConfigStorage::getVersionNumber()
{
    return getStringFromJson("version", VersionConfiguration);
}

std::string ConfigStorage::getVersionNumberWithPlatform()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    return "Android/" + getVersionNumber();
#else
    return "IOS/" + getVersionNumber();
#endif
}

std::string ConfigStorage::getVersionNumberToDisplay()
{
    return _("Version Number") + " " + getVersionNumber();
}
    
std::string ConfigStorage::getVersionInformationForRequestHeader()
{
    return Device::getInstance()->getOSManufacturer() + "/" + getVersionNumberWithPlatform();
}
    
}
