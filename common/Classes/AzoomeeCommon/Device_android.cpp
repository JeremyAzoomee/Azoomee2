//
//  Device_android.cpp
//  AzoomeeCommon
//
//  Created by Macauley.Scoffins on 17/02/2020.
//

#include "Device.h"
#include "Net/Utils.h"
#include "Analytics/AnalyticsSingleton.h"
#include "Utils/StringFunctions.h"
#include "platform/android/jni/JniHelper.h"
static const std::string kAzoomeeActivityJavaClassName = "org/cocos2dx/cpp/AppActivity";
static const std::string kCommonActivityJavaClassName = "com/tinizine/azoomee/common/AzoomeeActivity";

using namespace cocos2d;

NS_AZOOMEE_BEGIN

std::string Device::getDeviceInformation()
{
    std::string sourceDeviceString1 = "NA";
    std::string sourceDeviceString2 = "NA";
    
    sourceDeviceString1 = JniHelper::callStaticStringMethod(kAzoomeeActivityJavaClassName, "getAndroidDeviceModel");
    sourceDeviceString2 = JniHelper::callStaticStringMethod(kAzoomeeActivityJavaClassName, "getOSBuildManufacturer");
    
    std::string sourceDevice = Net::urlEncode(sourceDeviceString1) + "|" + Net::urlEncode(sourceDeviceString2);
    
    return sourceDevice;
}

std::string Device::getDeviceAdvertisingId()
{
    std::string deviceId = "";

    deviceId = JniHelper::callStaticStringMethod(kAzoomeeActivityJavaClassName, "getAndroidDeviceAdvertisingId");
    
    return deviceId;
}
    
std::string Device::getOSManufacturer()
{
    if(_osManufacturer != "")
    {
        return _osManufacturer;
    }

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
    
    return _osManufacturer;
}
    
std::string Device::getDeviceLanguage()
{
    std::string languageCode = "";
    
    languageCode = JniHelper::callStaticStringMethod(kAzoomeeActivityJavaClassName, "getDeviceLanguage");
    
    return languageCode;
}

std::string Device::getAppVersion() const
{
    return JniHelper::callStaticStringMethod(kAzoomeeActivityJavaClassName, "getAppVersionNum");
}

void Device::identifyMixpanel()
{
    JniHelper::callStaticVoidMethod(kAzoomeeActivityJavaClassName, "identifyMixpanel");
}

void Device::deleteHttpCookies()
{
    //Do Nothing, no local cookie store on android
}

bool Device::biometricAuthenticationAvailable()
{
    return JniHelper::callStaticBooleanMethod(kAzoomeeActivityJavaClassName, "fingerPrintAuthenticationAvailable");
}

void Device::startBiometricAuthentication()
{
    JniHelper::callStaticVoidMethod(kAzoomeeActivityJavaClassName, "startFingerprintAuthentication");
}

void Device::stopBiometricAuthentication()
{
    JniHelper::callStaticVoidMethod(kAzoomeeActivityJavaClassName, "stopFingerprintAuthentication");
}

void Device::setOrientation(Application::Orientation orientation)
{
    switch(orientation)
    {
        case Application::Orientation::Portrait:
            JniHelper::callStaticVoidMethod(kCommonActivityJavaClassName, "setOrientationPortrait");
            break;
        case Application::Orientation::Landscape:
            JniHelper::callStaticVoidMethod(kCommonActivityJavaClassName, "setOrientationLandscape");
            break;
        case Application::Orientation::Any:
            JniHelper::callStaticVoidMethod(kCommonActivityJavaClassName, "setOrientationAny");
            break;
    }
}

NS_AZOOMEE_END
