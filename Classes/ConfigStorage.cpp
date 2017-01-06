//ConfigStorage is a singleton designed to hold all necessary "burnt-in" information for all visual scenes and layers.

#include "ConfigStorage.h"
#include "ParentDataProvider.h"

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
//-------------------------BACKEND CALLER CONFIGURATION--------------------
std::string ConfigStorage::getServerHost()
{
    return "api.elb.ci.azoomee.ninja";
}

std::string ConfigStorage::getServerUrl()
{
    return "http://" + getServerHost();
}

std::string ConfigStorage::getPathForTag(std::string httpRequestTag)
{
    if(httpRequestTag == "parentLogin") return "/api/auth/login";
    if(httpRequestTag == "getChildren") return StringUtils::format("/api/user/adult/%s/owns", ParentDataProvider::getInstance()->getParentLoginValue("id").c_str());
    if(httpRequestTag == "childLogin") return "/api/auth/switchProfile";
    if(httpRequestTag == "getGordon") return "/api/porthole/pixel/gordon.png";
    if(httpRequestTag == "registerParent") return "/api/user/v2/adult";
    if(httpRequestTag == "registerChild") return "/api/user/child";
    
    return "";
}

//-------------------------ChildAccountScene settings---------------------------
std::string ConfigStorage::getNameForOomee(int number)
{
    std::map<int, std::string> oomeeNames;
    
    oomeeNames[0] = "om_Pink";
    oomeeNames[1] = "om_Yellow";
    oomeeNames[2] = "om_Raspberry";
    oomeeNames[3] = "om_Green";
    oomeeNames[4] = "om_Blue";
    
    return oomeeNames[number];
}

std::string ConfigStorage::getUrlForOomee(int number)
{
    std::map<int, std::string> oomeeUrls;
    
    oomeeUrls[0] = "https://media.azoomee.com/static/thumbs/oomee_01.png";
    oomeeUrls[1] = "https://media.azoomee.com/static/thumbs/oomee_03.png";
    oomeeUrls[2] = "https://media.azoomee.com/static/thumbs/oomee_04.png";
    oomeeUrls[3] = "https://media.azoomee.com/static/thumbs/oomee_05.png";
    oomeeUrls[4] = "https://media.azoomee.com/static/thumbs/oomee_06.png";
    
    return oomeeUrls[number];
}

int ConfigStorage::getOomeeNumberForUrl(std::string url)
{
    std::map<std::string, int> oomeeNumbers;
    
    oomeeNumbers["https://media.azoomee.com/static/thumbs/oomee_01.png"] = 0;
    oomeeNumbers["https://media.azoomee.com/static/thumbs/oomee_03.png"] = 1;
    oomeeNumbers["https://media.azoomee.com/static/thumbs/oomee_04.png"] = 2;
    oomeeNumbers["https://media.azoomee.com/static/thumbs/oomee_05.png"] = 3;
    oomeeNumbers["https://media.azoomee.com/static/thumbs/oomee_06.png"] = 4;
    
    if ( oomeeNumbers.find(url) == oomeeNumbers.end() )
    {
        return 0;
    }
    else
    {
        return oomeeNumbers[url];
    }
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
    sizes["VIDEO HQ"] = Size(520,520);
    sizes["AUDIO HQ"] = Size(520,520);
    sizes["GAME HQ"] = Size(520,520);
    sizes["ARTS APP"] = Size(520,520);
    
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

std::vector<Point> ConfigStorage::getMainHubPositionForHighlightElements(std::string categoryName)
{
    std::map<std::string, std::vector<Point>> positions;
    
    positions["PLAY"] = std::vector<Point> {Point(600, 75), Point(400,400)};
    positions["LISTEN"] = std::vector<Point> {Point(-800, -700), Point(-1050, -475)};
    positions["WATCH"] = std::vector<Point> {Point(-1050, 75), Point(-700, 400)};
    
    return positions[categoryName];
}

//--------------------------------IMAGECONTAINER CONFIGURATION----------------

cocos2d::Color4B ConfigStorage::getColourForElementType(std::string type)
{
    std::map<std::string, Color4B> colours;
    
    colours["AUDIO"] = Color4B(58,188,152,150);
    colours["GAME"] = Color4B(86,177,255,150);
    colours["VIDEO"] = Color4B(248, 71, 89, 150);
    
    return colours[type];
}

std::string ConfigStorage::getIconNameForCategory(std::string category)
{
    std::map<std::string, std::string> iconNames;
    iconNames["VIDEO"] = "watch";
    iconNames["AUDIO"] = "watch";
    iconNames["GAME"] = "play";
    
    return iconNames[category];
}

//-----------------------------------OOMEE animation identifier configuration----------------------------------

std::string ConfigStorage::getRandomIdForAnimationType(std::string animationType)
{
    std::vector<std::string> idleAnimations;
    idleAnimations.push_back("Build_Double_Take_Point_Left");
    idleAnimations.push_back("Build_Ear_Twitch");
    idleAnimations.push_back("Build_Fall_Asleep");
    idleAnimations.push_back("Build_Float");
    idleAnimations.push_back("Build_Float2");
    idleAnimations.push_back("Build_Float3");
    idleAnimations.push_back("Build_Look_Top_Left");
    idleAnimations.push_back("Build_Look_Top_Right");
    //idleAnimations.push_back("Build_Point_To_Right");
    idleAnimations.push_back("Build_Point_Bottom_Left");
    idleAnimations.push_back("Build_Scratch");
    idleAnimations.push_back("Build_Simple_Wave");
    idleAnimations.push_back("Build_Smile");
    idleAnimations.push_back("Build_Yawn");
    
    std::vector<std::string> touchAnimations;
    touchAnimations.push_back("Build_Dance_Wave");
    touchAnimations.push_back("Build_Eat");
    touchAnimations.push_back("Build_Glitch");
    touchAnimations.push_back("Build_Pop");
    touchAnimations.push_back("Build_Shower");
    touchAnimations.push_back("Build_Sneeze");
    
    if(animationType == "idle") return idleAnimations.at(random(0, (int)idleAnimations.size() - 1));
    else return touchAnimations.at(random(0, (int)touchAnimations.size() - 1));
}
