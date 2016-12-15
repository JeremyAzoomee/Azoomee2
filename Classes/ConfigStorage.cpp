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

cocos2d::Size ConfigStorage::getSizeForContentItemInCategory(std::string category)
{
    std::map<std::string, Size> sizes;
    sizes["VIDEO HQ"] = Size(500,500);
    sizes["AUDIO HQ"] = Size(500,1000);
    sizes["GAME HQ"] = Size(500,1000);
    sizes["ARTS APP"] = Size(500,1000);
    
    return sizes[category];
}

cocos2d::Color4B ConfigStorage::getBaseColourForContentItemInCategory(std::string category)
{
    std::map<std::string, Color4B> colours;
    
    colours["VIDEO HQ"] = Color4B(255,0,0, 150);
    colours["AUDIO HQ"] = Color4B(0,255,0, 150);
    colours["GAME HQ"] = Color4B(255,255,0, 150);
    colours["ARTS APP"] = Color4B(0,0,255, 150);
    
    return colours[category];
}

std::string ConfigStorage::getIconImagesForContentItemInCategory(std::string category)
{
    std::map<std::string, std::string> icons;
    
    icons["VIDEO HQ"] = "res/hqscene/icon_watch.png";
    icons["AUDIO HQ"] = "res/hqscene/icon_watch.png";
    icons["GAME HQ"] = "";
    icons["ARTS APP"] = "res/hqscene/icon_play.png";
    
    return icons[category];
}

cocos2d::Vec2 ConfigStorage::getHighlightSizeMultiplierForContentItem(int highlightClass)
{
    std::map<int, Vec2> highlightMultipliers;
    
    highlightMultipliers[0] = Vec2(1.0f,1.0f);
    highlightMultipliers[1] = Vec2(1.0f,2.04f);
    highlightMultipliers[2] = Vec2(2.04f,2.04f);
    
    return highlightMultipliers[highlightClass];
}

//------------------NAVIGATIONLAYER CONFIGURATION--------------------------------

cocos2d::Point ConfigStorage::getCirclePositionForMenuItem(int itemNumber)
{
    std::vector<Point> positions;
    
    positions.push_back(Point(1366, 1458));
    positions.push_back(Point(989, 1241));
    positions.push_back(Point(990, 805));
    positions.push_back(Point(1369, 589));
    positions.push_back(Point(1745, 810));
    positions.push_back(Point(1741, 1244));
    
    return positions.at(itemNumber);
}

cocos2d::Point ConfigStorage::getHorizontalPositionForMenuItem(int itemNumber)
{
    std::vector<Point> positions;
    
    positions.push_back(Point(775, 1675));
    positions.push_back(Point(1011, 1675));
    positions.push_back(Point(1248, 1675));
    positions.push_back(Point(1957, 1675));
    positions.push_back(Point(1721, 1675));
    positions.push_back(Point(1484, 1675));
    
    return positions.at(itemNumber);
}

cocos2d::Color4B ConfigStorage::getColourForMenuItem(int itemNumber)
{
    std::vector<Color4B> colours;
    
    colours.push_back(Color4B(0,0,0,0));
    colours.push_back(Color4B(248,71,89,255));
    colours.push_back(Color4B(58,188,152,255));
    colours.push_back(Color4B(0,0,0,0));
    colours.push_back(Color4B(246,185,66,255));
    colours.push_back(Color4B(86,177,255,255));
    
    return colours.at(itemNumber);
}

std::string ConfigStorage::getNameForMenuItem(int itemNumber)
{
    std::vector<std::string> names;
    
    names.push_back("HOME");
    names.push_back("VIDEO HQ");
    names.push_back("AUDIO HQ");
    names.push_back("MAIL APP");
    names.push_back("ART APP");
    names.push_back("GAME HQ");
    
    return names.at(itemNumber);
}

Point ConfigStorage::getTargetPositionForMove(int itemNumber)
{
    std::vector<Point> positions;
    
    positions.push_back(Vec2(0,0));
    positions.push_back(Vec2(-2732, 0));
    positions.push_back(Vec2(2732, 0));
    positions.push_back(Vec2(0, 0));
    positions.push_back(Vec2(0, -2048));
    positions.push_back(Vec2(0, 2048));
    
    return positions.at(itemNumber);
}
