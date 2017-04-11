#ifndef __CONFIGSTORAGE_H__
#define __CONFIGSTORAGE_H__

#include "cocos2d.h"
#include "external/json/document.h"

class ConfigStorage : public cocos2d::Ref
{
    
public:
    /** Returns the shared instance of the Game Manager */
    static ConfigStorage* getInstance(void);
    
public:
    virtual ~ConfigStorage();
    bool init(void);
    
    //General configuration
//#define autologin YES;
//#define forcereload YES;
//#define forgetuserdata YES;
    
    std::string getFileNameFromUrl(std::string url);
    
    //Backend caller configuration
    std::string getServerHost();
    std::string getServerUrl();
    std::string getImagesUrl();
    std::string getPathForTag(std::string httpRequestTag);
    
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
    
    //NavigationLayer configuration
    cocos2d::Point getCirclePositionForMenuItem(int itemNumber);
    cocos2d::Point getHorizontalPositionForMenuItem(int itemNumber);
    float getHorizontalMenuItemsHeight();
    cocos2d::Point getHorizontalPositionForMenuItemInGroupHQ(int itemNumber);
    cocos2d::Color4B getColourForMenuItem(int itemNumber);
    std::string getNameForMenuItem(int itemNumber);
    int getTagNumberForMenuName(std::string name);
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
    
    cocos2d::Point visualOrigin;
    cocos2d::Size visualSize;
};

#endif
