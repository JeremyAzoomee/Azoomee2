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
        HOME,
        ARTS_APP,
        GAME_HQ,
        GROUP_HQ
    };
    
    static const char* const kGameHQName;
    static const char* const kVideoHQName;
    static const char* const kAudioHQName;
    static const char* const kGroupHQName;
    static const char* const kHomeHQName;
    static const char* const kArtAppHQName;
    
    static const char* const kDefaultHQName;
    
    static const char* const kContentTypeVideo;
    static const char* const kContentTypeAudio;
    static const char* const kContentTypeGame;
    static const char* const kContentTypeGroup;
    static const char* const kContentTypeAudioGroup;
    
    static const char* const kEstimatedKeyboardHeightPortrait;
    static const char* const kEstimatedKeyboardHeightLandscape;
    
    /** Returns the shared instance of the Game Manager */
    static ConfigStorage* getInstance(void);
    virtual ~ConfigStorage();
    bool init(void);
    
    std::string getFileNameFromUrl(const std::string& url);
    
    //Backend caller configuration
    std::string getServerHost();
    std::string getServerUrlPrefix();
    std::string getServerUrl();
    std::string getCTAPackageJsonURL();
    std::string getMediaPrefixForXwalkCookies();
    std::string getPathForTag(const std::string& httpRequestTag);
    bool isParentSignatureRequiredForRequest(const std::string& requestTag);
    bool isImmediateRequestSendingRequired(const std::string& requestTag);
    
    //ChildAccountScene settings
    std::string getNameForOomee(int number);
    std::string getHumanReadableNameForOomee(int number);
    std::string getUrlForOomee(int number);
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
    std::string getHQSceneNameReplacementForPermissionFeed(const std::string &inputHqSceneName);
    cocos2d::Point getRelativeCirclePositionForMenuItem(int itemNumber);
    cocos2d::Point getHorizontalPositionForMenuItem(int itemNumber);
    float getHorizontalMenuItemsHeight();
    cocos2d::Point getHorizontalPositionForMenuItemInGroupHQ(int itemNumber);
    cocos2d::Color4B getColourForMenuItem(int itemNumber);
    std::string getNameForMenuItem(int itemNumber);
    HubTargetTagNumber getTagNumberForMenuName(const std::string& name);
    cocos2d::Point getTargetPositionForMove(int itemNumber);
    
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
    
    //IAP Configuration
    std::string getIapSkuForProvider(const std::string& provider);
    std::string getDeveloperPublicKey();
    
    //Device-specific information
    std::string getDeviceInformation();
    std::string getDeviceAdvertisingId();
    
    //Device-resolution-specific information
    void setIsDeviceIphoneX(bool isDeviceIphoneX);
    bool isDeviceIphoneX() const;
    
    //Set keyboard height for chat
    void setEstimatedKeyboardHeight(float size);
    
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
    
    bool _isDeviceIphoneX = false;
    
};
  
}

#endif
