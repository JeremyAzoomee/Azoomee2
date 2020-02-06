//
//  Device.cpp
//  AzoomeeCommon
//
//  Created by Macauley.Scoffins on 31/01/2020.
//

#include "Device.h"
#include "Net/Utils.h"
#include "Analytics/AnalyticsSingleton.h"
#include "Utils/StringFunctions.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"
static const std::string kAzoomeeActivityJavaClassName = "org/cocos2dx/cpp/AppActivity";
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "Utils/IosNativeFunctionsSingleton.h"
#endif

using namespace cocos2d;

NS_AZOOMEE_BEGIN

const char* const Device::kEstimatedKeyboardHeightPortrait = "Azoomee::MessageComposer::EstimatedKeyboardHeight/Portrait";
const char* const Device::kEstimatedKeyboardHeightLandscape = "Azoomee::MessageComposer::EstimatedKeyboardHeight/Landscape";

const std::string Device::kOSManufacturerApple = "Apple";
const std::string Device::kOSManufacturerGoogle = "Google";
const std::string Device::kOSManufacturerAmazon = "Amazon";

static std::auto_ptr<Device> sDeviceSharedInstance;

Device* Device::getInstance()
{
    if(!sDeviceSharedInstance.get())
    {
        sDeviceSharedInstance.reset(new Device());
    }
    return sDeviceSharedInstance.get();
}

Device::~Device()
{
    
}

//----------------------------- Device specific information -----------------------------

std::string Device::getDeviceInformation()
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

std::string Device::getDeviceAdvertisingId()
{
    std::string deviceId = "";
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    deviceId = IosNativeFunctionsSingleton::getInstance()->getIosDeviceIDFA();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    deviceId = JniHelper::callStaticStringMethod(kAzoomeeActivityJavaClassName, "getAndroidDeviceAdvertisingId");
#endif
    
    return deviceId;
}
    
std::string Device::getOSManufacturer()
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
    
std::string Device::getDeviceLanguage()
{
    std::string languageCode = "";
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    languageCode = IosNativeFunctionsSingleton::getInstance()->getIosDeviceLanguage();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    languageCode = JniHelper::callStaticStringMethod(kAzoomeeActivityJavaClassName, "getDeviceLanguage");
#endif
    
    return languageCode;
}
    
void Device::setIsDeviceIphoneX(bool isDeviceIphoneX)
{
    _isDeviceIphoneX = isDeviceIphoneX;
}
    
bool Device::isDeviceIphoneX() const
{
    return _isDeviceIphoneX;
}
    
void Device::setIsDevice18x9(bool isDevice18x9)
{
    _isDevice18x9 = isDevice18x9;
}
    
bool Device::isDevice18x9() const
{
    return _isDevice18x9;
}

void Device::setIsDevicePhone(bool isPhone)
{
    _isDevicePhone = isPhone;
}
    
bool Device::isDevicePhone() const
{
    return _isDevicePhone;
}

//------------------------- Set estimated keyboard height for chat ---------------------------
void Device::setEstimatedKeyboardHeight(float height)
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
void Device::setClientAnonymousIp(const std::string& publicIp)
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
    
std::string Device::getClientAnonymousIp() const
{
    return _clientIp.empty() ? "0.0.0.0" : _clientIp;
}

NS_AZOOMEE_END
