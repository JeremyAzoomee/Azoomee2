#ifndef AzoomeeCommon_ConfigStorage_h
#define AzoomeeCommon_ConfigStorage_h

//General configuration
//#define FORCE_RELOAD YES;
//#define FORGET_USER_DATA YES;
//#define ALLOW_UNPAID_SIGNUP YES
//#define USINGCI YES

#include <cocos/cocos2d.h>
#include "Json.h"

namespace Azoomee
{

class ConfigStorage : public cocos2d::Ref
{
    
public:
    
    enum HubTargetTagNumber {
        CHAT = 0,
        VIDEO_HQ,
        AUDIO_HQ,
        ME_HQ,
        GAME_HQ,
        ARTS_APP,
        GROUP_HQ
    };
    
    static const char* const kChatHQName;
    static const char* const kGameHQName;
    static const char* const kVideoHQName;
    static const char* const kAudioHQName;
    static const char* const kGroupHQName;
    static const char* const kHomeHQName;
    static const char* const kArtAppHQName;
    static const char* const kMeHQName;
    
    static std::string kDefaultHQName;
    
    static const char* const kRecentlyPlayedCarouselName;
    
    static const char* const kContentNodeName;
    static const char* const kContentLayerName;
    static const char* const kNavigationLayerName;
    
    static const char* const kContentTypeVideo;
    static const char* const kContentTypeAudio;
    static const char* const kContentTypeGame;
    static const char* const kContentTypeGroup;
    static const char* const kContentTypeAudioGroup;
    static const char* const kContentTypeManual;
    static const char* const kContentTypeInternal;
    
    static const char* const kOomeeMakerURI;
    static const char* const kArtAppURI;
    
    static const char* const kEstimatedKeyboardHeightPortrait;
    static const char* const kEstimatedKeyboardHeightLandscape;
    
    static const std::string kArtCacheFolder;
    
    static const std::string kGameDownloadError;
	
	static const std::string kIOSSubURL;
	static const std::string kAndroidSubURL;
	static const std::string kAmazonSubURL;
	
	static const std::string kBillingProviderApple;
	static const std::string kBillingProviderGoogle;
	static const std::string kBillingProviderAmazon;
    
    /** Returns the shared instance of the Game Manager */
    static ConfigStorage* getInstance(void);
    virtual ~ConfigStorage();
    bool init(void);
    
    std::string getFileNameFromUrl(const std::string& url);
    
    std::string getGameCachePath();
    std::string getDefaultHQ();
    
    //Backend caller configuration
    std::string getServerHost();
    std::string getServerUrlPrefix();
    std::string getServerUrl();
    std::string getCTAPackageJsonURL();
    std::string getMediaPrefixForXwalkCookies();
    std::string getPathForTag(const std::string& httpRequestTag);
    std::string getRemoteWebGameAPIPath();
    bool isParentSignatureRequiredForRequest(const std::string& requestTag);
    bool isImmediateRequestSendingRequired(const std::string& requestTag);
    
    //ChildAccountScene settings
    std::string getUrlForOomee(int number);
    std::string getConfigUrlForOomee(int number);
    std::string getLocalImageForOomee(int number);
    std::string getLocalConfigForOomee(int number);
    int getOomeeNumberForUrl(const std::string& url);
    
    //BaseScene configuration
    cocos2d::Point getHQScenePositions(const std::string& hqSceneName);
    
    //HQSceneElementConfiguration
    cocos2d::Size getSizeForContentItemInCategory(const std::string& category);
    std::string getPlaceholderImageForContentItemInCategory(const std::string& type);
    cocos2d::Vec2 getHighlightSizeMultiplierForContentItem(int highlightClass);
    float getScrollviewTitleTextHeight();
    cocos2d::Size getGroupHQLogoSize();
    int getContentItemImageValidityInSeconds();
    float getGroupContentItemTextHeight();
    cocos2d::Color4B getColourForElementType(const std::string& type);
    std::string getIconNameForCategory(const std::string& category);
    std::string getGradientImageForCategory(const std::string& category);
    
    //NavigationLayer configuration
    std::string getHQSceneNameReplacementForPermissionFeed(const std::string &inputHqSceneName) const;
    cocos2d::Point getHorizontalPositionForMenuItem(const std::string& hqName) const;
    float getHorizontalMenuItemsHeight() const;
    cocos2d::Point getHorizontalPositionForMenuItemInGroupHQ(const std::string& hqName) const;
    cocos2d::Color4B getColourForMenuItem(const std::string& hqName) const;
    std::string getNameForMenuItem(const std::string& hqName) const;
    cocos2d::Point getTargetPositionForMove(const std::string& hqName) const;
    std::vector<std::string> getHqNames() const;
    void setNavigationHQs(const std::vector<std::string>& hqs);
    void setDefaultHQ(const std::string& defaultHq);
    
    //MainHubScene configuration
    std::vector<cocos2d::Point> getMainHubPositionForHighlightElements(const std::string& categoryName);
    std::string getRequiredTypesForHighlightCategory(int category);
    
    //OomeeLayer animation states
    std::string getGreetingAnimation();
    std::string getRandomIdForAnimationType(const std::string& animationType);
    
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
    
    //Device-resolution-specific information
    void setIsDeviceIphoneX(bool isDeviceIphoneX);
    bool isDeviceIphoneX() const;
    void setIsDevice18x9(bool isDevice18x9);
    bool isDevice18x9() const;
    
    //Set keyboard height for chat
    void setEstimatedKeyboardHeight(float size);
    
    //Client IP setting
    void setClientAnonymousIp(const std::string& publicIp);
    std::string getClientAnonymousIp() const;
    
private:
    rapidjson::Document parseJsonConfigurationFile(const std::string& fileName);
    
    rapidjson::Document BaseSceneConfiguration;
    rapidjson::Document HQSceneConfiguration;
    rapidjson::Document NavigationConfiguration;
    rapidjson::Document OomeeAnimationTypes;
    rapidjson::Document OomeeConfiguration;
    rapidjson::Document VersionConfiguration;
    rapidjson::Document IapConfiguration;
    
    std::vector<std::string> requestTagsRequireImmediateSending;
    std::vector<std::string> parentSignedRequestTags;
    
    std::vector<std::string> _navigationHQs;
    
    bool _isDeviceIphoneX = false;
    bool _isDevice18x9 = false;
    
    std::string _osManufacturer = "";
    std::string _clientIp = "";
};
  
}

#endif
