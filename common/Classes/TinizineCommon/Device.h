//
//  Device.h
//  TinizineCommon
//
//  Created by Macauley.Scoffins on 31/01/2020.
//

#ifndef Device_h
#define Device_h

#include "Tinizine.h"
#include <cocos/cocos2d.h>
#include "Application.h"

NS_TZ_BEGIN

class Device
{
public:
    static Device* getInstance();
    virtual ~Device();
    
    static const char* const kEstimatedKeyboardHeightPortrait;
    static const char* const kEstimatedKeyboardHeightLandscape;
    
    static const std::string kOSManufacturerApple;
    static const std::string kOSManufacturerGoogle;
    static const std::string kOSManufacturerAmazon;
    
    //Device-specific information
    std::string getDeviceInformation();
    std::string getDeviceAdvertisingId();
    std::string getOSManufacturer();
    std::string getDeviceLanguage();
    
    //Device-resolution-specific information
    void setIsDeviceIphoneX(bool isDeviceIphoneX);
    bool isDeviceIphoneX() const;
    void setIsDevice18x9(bool isDevice18x9);
    bool isDevice18x9() const;
    void setIsDevicePhone(bool isPhone);
    bool isDevicePhone() const;
    
    //Set keyboard height for chat
    void setEstimatedKeyboardHeight(float size);
    
    //Client IP setting
    void setClientAnonymousIp(const std::string& publicIp);
    std::string getClientAnonymousIp() const;
    
    std::string getAppVersion() const;
    
    void identifyMixpanel();
    void deleteHttpCookies();
    
    bool biometricAuthenticationAvailable();
    void startBiometricAuthentication();
    void stopBiometricAuthentication();
    
    void setOrientation(Application::Orientation orientation);
    
private:
    
    bool _isDeviceIphoneX = false;
    bool _isDevice18x9 = false;
    bool _isDevicePhone = false;
    
    std::string _osManufacturer = "";
    std::string _clientIp = "";
};

NS_TZ_END

#endif /* Device_h */
