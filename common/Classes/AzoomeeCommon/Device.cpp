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

using namespace cocos2d;

NS_TZ_BEGIN

const char* const Device::kEstimatedKeyboardHeightPortrait = "TZ::MessageComposer::EstimatedKeyboardHeight/Portrait";
const char* const Device::kEstimatedKeyboardHeightLandscape = "TZ::MessageComposer::EstimatedKeyboardHeight/Landscape";

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

NS_TZ_END
