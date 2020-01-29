#ifndef AzoomeeCommon_ConfigStorage_h
#define AzoomeeCommon_ConfigStorage_h

//General configuration
//#define FORCE_RELOAD YES;
//#define FORGET_USER_DATA YES;
//#define ALLOW_UNPAID_SIGNUP YES

#include <cocos/cocos2d.h>
#include "Json.h"

namespace Azoomee
{

class ConfigStorage : public cocos2d::Ref
{
    
public:
    
    static const char* const kGroupHQName;
    
	static const char* const kIosWebviewName;
	static const char* const kAndroidWebviewName;
    
    static const char* const kEstimatedKeyboardHeightPortrait;
    static const char* const kEstimatedKeyboardHeightLandscape;
    
    static const std::string kArtCacheFolder;
    static const std::string kGameCacheFolder;
	static const std::string kOomeeMakerCacheFolder;
    
    static const std::string kAvatarImageCacheFolder;

    static const std::string kOSManufacturerApple;
    static const std::string kOSManufacturerGoogle;
    static const std::string kOSManufacturerAmazon;
    
    static const std::string kSignupPlatformSource;
    
    /** Returns the shared instance of the Game Manager */
    static ConfigStorage* getInstance(void);
    virtual ~ConfigStorage();
    bool init(void);
    
    std::string getFileNameFromUrl(const std::string& url);
    
    std::string getGameCachePath();
    
    //Backend caller configuration
    std::string getServerHost();
    std::string getServerUrlPrefix();
    std::string getServerUrl();
    std::string getRemoteWebGameAPIPath();
    bool isParentSignatureRequiredForRequest(const std::string& requestTag);
    bool isImmediateRequestSendingRequired(const std::string& requestTag);

    //Android helper for arts app
    int inArtsApp;

    //UserDefaults First Time User for Slideshow
    void setFirstSlideShowSeen();
    bool shouldShowFirstSlideShowScene();
    
    //Version configuration
    std::string getVersionNumber();
    std::string getVersionNumberWithPlatform();
    std::string getVersionNumberToDisplay();
    std::string getVersionInformationForRequestHeader();
    
    //IAP Configuration
    std::string getIapSkuForProvider(const std::string& provider);
    std::string getDeveloperPublicKey();
    
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
    
private:
    rapidjson::Document parseJsonConfigurationFile(const std::string& fileName);
    
    rapidjson::Document VersionConfiguration;
    rapidjson::Document IapConfiguration;
    
    std::vector<std::string> requestTagsRequireImmediateSending;
    std::vector<std::string> parentSignedRequestTags;
    
    std::vector<std::string> _navigationHQs;
    
    bool _isDeviceIphoneX = false;
    bool _isDevice18x9 = false;
	bool _isDevicePhone = false;
    
    std::string _osManufacturer = "";
    std::string _clientIp = "";
};
  
}

#endif
