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
    
    const char* const ConfigStorage::kEstimatedKeyboardHeightPortrait = "Azoomee::MessageComposer::EstimatedKeyboardHeight/Portrait";
    const char* const ConfigStorage::kEstimatedKeyboardHeightLandscape = "Azoomee::MessageComposer::EstimatedKeyboardHeight/Landscape";
    
    const std::string ConfigStorage::kArtCacheFolder = "artCache/";
	const std::string ConfigStorage::kOomeeMakerCacheFolder = "oomeeMaker/";
    
    const std::string ConfigStorage::kAvatarImageCacheFolder = "avatars";
	
    const std::string ConfigStorage::kOSManufacturerApple = "Apple";
    const std::string ConfigStorage::kOSManufacturerGoogle = "Google";
    const std::string ConfigStorage::kOSManufacturerAmazon = "Amazon";
   
    
    
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
    return getOSManufacturer() + "/" + getVersionNumberWithPlatform();
}
    
//----------------------------- Device specific information -----------------------------

std::string ConfigStorage::getDeviceInformation()
{
    std::string sourceDeviceString1 = "NA";
    std::string sourceDeviceString2 = "NA";
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    sourceDeviceString1 = IosNativeFunctionsSingleton::getInstance()->getIosSystemVersion();
    sourceDeviceString2 = IosNativeFunctionsSingleton::getInstance()->getIosDeviceType();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    sourceDeviceString1 = JniHelper::callStaticStringMethod(kAzoomeeActivityJavaClassName, "getAndroidDeviceModel");
    sourceDeviceString2 = JniHelper::callStaticStringMethod(kAzoomeeActivityJavaClassName, "getOSBuildManufacturer");
#endif
    
    std::string sourceDevice = Net::urlEncode(sourceDeviceString1) + "|" + Net::urlEncode(sourceDeviceString2);
    
    return sourceDevice;
}

std::string ConfigStorage::getDeviceAdvertisingId()
{
    std::string deviceId = "";
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    deviceId = IosNativeFunctionsSingleton::getInstance()->getIosDeviceIDFA();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    deviceId = JniHelper::callStaticStringMethod(kAzoomeeActivityJavaClassName, "getAndroidDeviceAdvertisingId");
#endif
    
    return deviceId;
}
    
std::string ConfigStorage::getOSManufacturer()
{
    if(_osManufacturer != "")
    {
        return _osManufacturer;
    }
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    std::string resultStr = JniHelper::callStaticStringMethod(kAzoomeeActivityJavaClassName, "getOSBuildManufacturer");
    
    if (resultStr == "Amazon")
    {
        AnalyticsSingleton::getInstance()->registerIAPOS("Amazon");
        _osManufacturer = "Amazon";
    }
    else
    {
        AnalyticsSingleton::getInstance()->registerIAPOS("Google");
        _osManufacturer = "Google";
    }
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    AnalyticsSingleton::getInstance()->registerIAPOS("iOS");
    _osManufacturer = "Apple";
#else
    _osManufacturer = "Unknown";
#endif
    
    return _osManufacturer;
}
	
std::string ConfigStorage::getDeviceLanguage()
{
	std::string languageCode = "";
	
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	languageCode = IosNativeFunctionsSingleton::getInstance()->getIosDeviceLanguage();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	languageCode = JniHelper::callStaticStringMethod(kAzoomeeActivityJavaClassName, "getDeviceLanguage");
#endif
	
	return languageCode;
}
    
void ConfigStorage::setIsDeviceIphoneX(bool isDeviceIphoneX)
{
    _isDeviceIphoneX = isDeviceIphoneX;
}
    
bool ConfigStorage::isDeviceIphoneX() const
{
    return _isDeviceIphoneX;
}
    
void ConfigStorage::setIsDevice18x9(bool isDevice18x9)
{
    _isDevice18x9 = isDevice18x9;
}
    
bool ConfigStorage::isDevice18x9() const
{
    return _isDevice18x9;
}

void ConfigStorage::setIsDevicePhone(bool isPhone)
{
	_isDevicePhone = isPhone;
}
	
bool ConfigStorage::isDevicePhone() const
{
	return _isDevicePhone;
}

//------------------------- Set estimated keyboard height for chat ---------------------------
void ConfigStorage::setEstimatedKeyboardHeight(float height)
{
    cocos2d::log("KEYBOARD estimated keyboard height received: %f", height);
    
    // Ignore tiny values
    // Sometimes device reports a 0 or small height (e.g Android devices can report the size of the
    // black bottom bar which sometimes appears with or without the keyboard).
    if(height < 100.0f)
    {
        return;
    }
    
    const Size& nativeScreenSize = Director::getInstance()->getOpenGLView()->getFrameSize();
    const bool currentOrientationPortrait = (nativeScreenSize.height > nativeScreenSize.width);
    
    if(currentOrientationPortrait)
    {
        UserDefault::getInstance()->setFloatForKey(kEstimatedKeyboardHeightPortrait, height);
    }
    else
    {
        UserDefault::getInstance()->setFloatForKey(kEstimatedKeyboardHeightLandscape, height);
    }
}
    
//----------------------- Anonymous IP configuration -------------------------------------------
void ConfigStorage::setClientAnonymousIp(const std::string& publicIp)
{
    std::vector<std::string> ipElementsVector = StringFunctions::splitStringToVector(publicIp, ".");
    if(ipElementsVector.size() != 4)
    {
        _clientIp = "0.0.0.0";
        return;
    }
    
    _clientIp = "";
    ipElementsVector[ipElementsVector.size() - 1] = "0";
    
    for(const std::string &currentElement : ipElementsVector)
    {
        if(_clientIp.length() != 0)
        {
            _clientIp += ".";
        }
        
        _clientIp += currentElement;
    }
}
    
std::string ConfigStorage::getClientAnonymousIp() const
{
    return _clientIp.empty() ? "0.0.0.0" : _clientIp;
}
    
}
