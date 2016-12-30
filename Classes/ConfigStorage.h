#include "cocos2d.h"

class ConfigStorage : public cocos2d::Ref
{
    
public:
    /** Returns the shared instance of the Game Manager */
    static ConfigStorage* getInstance(void);
    
public:
    virtual ~ConfigStorage();
    bool init(void);
    
    //General configuration
    bool autologin = true;
    bool forcereload = true;
    
    //Backend caller configuration
    std::string getServerHost();
    std::string getServerUrl();
    std::string getPathForTag(std::string httpRequestTag);
    
    //BaseScene configuration
    cocos2d::Point getHQScenePositions(std::string hqSceneName);
    
    //HQSceneElementConfiguration
    cocos2d::Size getSizeForContentItemInCategory(std::string category);
    cocos2d::Color4B getBaseColourForContentItemInCategory(std::string category);
    std::string getIconImagesForContentItemInCategory(std::string category);
    cocos2d::Vec2 getHighlightSizeMultiplierForContentItem(int highlightClass);
    
    //NavigationLayer configuration
    cocos2d::Point getCirclePositionForMenuItem(int itemNumber);
    cocos2d::Point getHorizontalPositionForMenuItem(int itemNumber);
    cocos2d::Color4B getColourForMenuItem(int itemNumber);
    std::string getNameForMenuItem(int itemNumber);
    cocos2d::Point getTargetPositionForMove(int itemNumber);
    
    //MainHubScene configuration
    std::vector<cocos2d::Point> getMainHubPositionForHighlightElements(std::string categoryName);
    std::string getRequiredTypesForHighlightCategory(int category);
    
    //ImageContainer configuration
    cocos2d::Color4B getColourForElementType(std::string type);
    std::string getIconNameForCategory(std::string category);
    
    //OomeeLayer animation states
    std::string getRandomIdForAnimationType(std::string animationType);
};
