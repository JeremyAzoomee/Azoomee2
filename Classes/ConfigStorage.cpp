#include "ConfigStorage.h"

using namespace cocos2d;

static ConfigStorage *_sharedConfigStorage = NULL;

ConfigStorage* ConfigStorage::getInstance()
{
    if (! _sharedConfigStorage)
    {
        _sharedConfigStorage = new ConfigStorage();
        _sharedConfigStorage->init();
    }
    
    return _sharedConfigStorage;
}

ConfigStorage::~ConfigStorage(void)
{
    
}

bool ConfigStorage::init(void)
{
    return true;
}

//-------------------------BASESCENE CONFIGURATION-------------------------
Point ConfigStorage::getHQScenePositions(std::string hqSceneName)
{
    std::map<std::string, Point> result;
    
    result["HOME"] = Point(0,0);
    result["VIDEO HQ"] = Point(2732, 0);
    result["GAME HQ"] = Point(0, -2048);
    result["AUDIO HQ"] = Point(-2732, 0);
    result["ARTS APP"] = Point(0, 2048);
    result["NavigationLayer"] = Point(0, 0);
    result["contentLayer"] = Point(0,0);
    
    return result[hqSceneName];
}

//-------------------------HQSCENEELEMENT CONFIGURATION-------------------------

cocos2d::Size getSizeForContentItemInCategory(std::string category)
{
    std::map<std::string, Size> sizes;
    sizes["VIDEO HQ"] = Size(500,500);
    sizes["AUDIO HQ"] = Size(500,1000);
    sizes["GAME HQ"] = Size(500,1000);
    sizes["ARTS APP"] = Size(500,1000);
    
    return sizes[category];
}

cocos2d::Color4B getBaseColourForContentItemInCategory(std::string category)
{
    std::map<std::string, Color4B> colours;
    
    colours["VIDEO HQ"] = Color4B(255,0,0, 150);
    colours["AUDIO HQ"] = Color4B(0,255,0, 150);
    colours["GAME HQ"] = Color4B(255,255,0, 150);
    colours["ARTS APP"] = Color4B(0,0,255, 150);
    
    return colours[category];
}

std::string getIconImagesForContentItemInCategory(std::string category)
{
    std::map<std::string, std::string> icons;
    
    icons["VIDEO HQ"] = "res/hqscene/icon_watch.png";
    icons["AUDIO HQ"] = "res/hqscene/icon_watch.png";
    icons["GAME HQ"] = "";
    icons["ARTS APP"] = "res/hqscene/icon_play.png";
    
    return icons[category];
}

cocos2d::Vec2 getHighlightSizeMultiplierForContentItem(int highlightClass)
{
    std::map<int, Vec2> highlightMultipliers;
    
    highlightMultipliers[0] = Vec2(1.0f,1.0f);
    highlightMultipliers[1] = Vec2(1.0f,2.04f);
    highlightMultipliers[2] = Vec2(2.04f,2.04f);
    
    return highlightMultipliers[highlightClass];
}
