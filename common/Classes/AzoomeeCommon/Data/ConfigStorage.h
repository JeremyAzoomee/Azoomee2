#ifndef AzoomeeCommon_ConfigStorage_h
#define AzoomeeCommon_ConfigStorage_h

//General configuration
//#define forcereload YES;
//#define forgetuserdata YES;
#define USINGCI YES

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
    
    /** Returns the shared instance of the Game Manager */
    static ConfigStorage* getInstance(void);
    virtual ~ConfigStorage();
    bool init(void);
    
    std::string getFileNameFromUrl(std::string url);
    
    //Backend caller configuration
    std::string getServerHost();
    std::string getServerUrlPrefix();
    std::string getServerUrl();
    std::string getImagesUrl();
    std::string getCTAPackageJsonURL();
    std::string getMediaPrefixForXwalkCookies();
    std::string getPathForTag(std::string httpRequestTag);
    bool isParentSignatureRequiredForRequest(std::string requestTag);
    bool isImmediateRequestSendingRequired(std::string requestTag);
    
    //ChildAccountScene settings
    std::string getNameForOomee(int number);
    std::string getOomeePNGName(int number);
    std::string getHumanReadableNameForOomee(int number);
    std::string getUrlForOomee(int number);
    int getOomeeNumberForUrl(std::string url);
    
    //BaseScene configuration
    cocos2d::Point getHQScenePositions(std::string hqSceneName);
    
    //HQSceneElementConfiguration
    cocos2d::Size getSizeForContentItemInCategory(std::string category);
    cocos2d::Color4B getBaseColourForContentItemInCategory(std::string category);
    std::string getIconImagesForContentItemInCategory(std::string category);
    std::string getPlaceholderImageForContentItemInCategory(std::string type);
    cocos2d::Vec2 getHighlightSizeMultiplierForContentItem(int highlightClass);
    float getScrollviewTitleTextHeight();
    cocos2d::Size getGroupHQLogoSize();
    int getContentItemImageValidityInSeconds();
    float getGroupContentItemTextHeight();
    
    //NavigationLayer configuration
    std::string getHQSceneNameReplacementForPermissionFeed(const std::string &inputHqSceneName);
    cocos2d::Point getRelativeCirclePositionForMenuItem(int itemNumber);
    cocos2d::Point getHorizontalPositionForMenuItem(int itemNumber);
    float getHorizontalMenuItemsHeight();
    cocos2d::Point getHorizontalPositionForMenuItemInGroupHQ(int itemNumber);
    cocos2d::Color4B getColourForMenuItem(int itemNumber);
    std::string getNameForMenuItem(int itemNumber);
    HubTargetTagNumber getTagNumberForMenuName(std::string name);
    cocos2d::Point getTargetPositionForMove(int itemNumber);
    
    //MainHubScene configuration
    std::vector<cocos2d::Point> getMainHubPositionForHighlightElements(std::string categoryName);
    std::string getRequiredTypesForHighlightCategory(int category);
    
    //ImageContainer configuration
    cocos2d::Color4B getColourForElementType(std::string type);
    std::string getIconNameForCategory(std::string category);
    std::string getGradientImageForCategory(std::string category);
    
    //OomeeLayer animation states
    std::string getGreetingAnimation();
    std::string getRandomIdForAnimationType(std::string animationType);
    
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
    std::string getIapSkuForProvider(std::string provider);
    std::string getDeveloperPublicKey();
    
    //Device-specific information
    std::string getDeviceInformation();
    std::string getDeviceAdvertisingId();
    
private:
    rapidjson::Document parseJsonConfigurationFile(std::string fileName);
    
    rapidjson::Document BaseSceneConfiguration;
    rapidjson::Document HQSceneConfiguration;
    rapidjson::Document ImageContainerConfiguration;
    rapidjson::Document NavigationConfiguration;
    rapidjson::Document OomeeAnimationTypes;
    rapidjson::Document OomeeConfiguration;
    rapidjson::Document VersionConfiguration;
    rapidjson::Document IapConfiguration;
    
    std::vector<std::string> requestTagsRequireImmediateSending;
    std::vector<std::string> parentSignedRequestTags;
    
};
  
}

#endif
