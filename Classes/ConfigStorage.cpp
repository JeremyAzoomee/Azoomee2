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
    inArtsApp = 0;
    return true;
}

std::string ConfigStorage::getFileNameFromUrl(std::string url)
{
    int startPoint = (int)url.find_last_of("/") + 1;
    
    int endPoint = (int)url.length();
    if(url.find("?", 0) != url.npos) endPoint = (int)url.find("?", 0);
    int subLength = endPoint - startPoint;
    
    return url.substr(startPoint, subLength);
}

//-------------------------BACKEND CALLER CONFIGURATION--------------------
std::string ConfigStorage::getServerHost()
{
    return "api.azoomee.com";
}

std::string ConfigStorage::getServerUrl()
{
    return "https://" + getServerHost();
}

std::string ConfigStorage::getImagesUrl()
{
    return "https://media.azoomee.com/static/images";
}

std::string ConfigStorage::getPathForTag(std::string httpRequestTag)
{
    if(httpRequestTag == "parentLogin") return "/api/auth/login";
    if(httpRequestTag == "getChildren") return StringUtils::format("/api/user/adult/%s/owns", ParentDataProvider::getInstance()->getLoggedInParentId().c_str());
    if(httpRequestTag == "childLogin") return "/api/auth/switchProfile";
    if(httpRequestTag == "getGordon") return "/api/porthole/pixel/gordon.png";
    if(httpRequestTag == "registerParent") return "/api/user/v2/adult";
    if(httpRequestTag == "registerChild") return "/api/user/child";
    if(httpRequestTag == "HOME") return "/api/electricdreams/view/categories/home";
    if(httpRequestTag == "PreviewHOME") return "/api/electricdreams/preview/view/categories/home";
    if(httpRequestTag == "updateParentPin") return StringUtils::format("/api/user/adult/%s", ParentDataProvider::getInstance()->getLoggedInParentId().c_str());
    if(httpRequestTag == "updateParentActorStatus") return StringUtils::format("/api/user/adult/%s", ParentDataProvider::getInstance()->getLoggedInParentId().c_str());
    
    return "";
}

//-------------------------Oomee settings---------------------------
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

std::string ConfigStorage::getOomeePNGName(int number)
{
    std::map<int, std::string> oomeeNames;
    
    oomeeNames[0] = "res/childSelection/om_Pink.png";
    oomeeNames[1] = "res/childSelection/om_Yellow.png";
    oomeeNames[2] = "res/childSelection/om_Raspberry.png";
    oomeeNames[3] = "res/childSelection/om_Green.png";
    oomeeNames[4] = "res/childSelection/om_Blue.png";
    
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
    
    oomeeNumbers["oomee_01.png"] = 0;
    oomeeNumbers["oomee_03.png"] = 1;
    oomeeNumbers["oomee_04.png"] = 2;
    oomeeNumbers["oomee_05.png"] = 3;
    oomeeNumbers["oomee_06.png"] = 4;
    
    std::string fileName = getFileNameFromUrl(url);
    
    if ( oomeeNumbers.find(fileName) == oomeeNumbers.end() )
    {
        return 0;
    }
    else
    {
        return oomeeNumbers[fileName];
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
    result["GROUP HQ"] = Point(0, 4096);
    
    return result[hqSceneName];
}

//-------------------------HQSCENEELEMENT CONFIGURATION-------------------------

cocos2d::Size ConfigStorage::getSizeForContentItemInCategory(std::string category)
{
    std::map<std::string, Size> sizes;
    //Original Size (693,520)
    sizes["VIDEO HQ"] = Size(590,442);
    sizes["AUDIO HQ"] = Size(590,442);
    sizes["GAME HQ"] = Size(590,442);
    sizes["ARTS APP"] = Size(590,442);
    sizes["GROUP HQ"] = Size(590,442);
    
    return sizes[category];
}

cocos2d::Color4B ConfigStorage::getBaseColourForContentItemInCategory(std::string category)
{
    std::map<std::string, Color4B> colours;
    
    colours["VIDEO HQ"] = Color4B(255,0,0, 150);
    colours["AUDIO HQ"] = Color4B(0,255,0, 150);
    colours["GAME HQ"] = Color4B(0,112,204, 150);
    colours["ARTS APP"] = Color4B(255,255,0, 150);
    colours["GROUP HQ"] = Color4B(255, 0, 0, 150);
    
    return colours[category];
}

std::string ConfigStorage::getIconImagesForContentItemInCategory(std::string category)
{
    std::map<std::string, std::string> icons;
    
    icons["VIDEO HQ"] = "res/hqscene/icon_watch.png";
    icons["AUDIO HQ"] = "res/hqscene/icon_listen.png";
    icons["GAME HQ"] = "res/hqscene/icon_play.png";
    icons["ARTS APP"] = "res/hqscene/icon_play.png";
    icons["GROUP HQ"] = "res/hqscene/icon_watch.png";
    
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

float ConfigStorage::getScrollviewTitleTextHeight()
{
    return 90;
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
    names.push_back("ARTS APP");
    names.push_back("GAME HQ");
    names.push_back("GROUP HQ");
    
    return names.at(itemNumber);
}

int ConfigStorage::getTagNumberForMenuName(std::string name)
{
    std::map<std::string, int> tagNumbers;
    
    tagNumbers["HOME"] = 0;
    tagNumbers["VIDEO HQ"] = 1;
    tagNumbers["AUDIO HQ"] = 2;
    tagNumbers["MAIL APP"] = 3;
    tagNumbers["ARTS APP"] = 4;
    tagNumbers["GAME HQ"] = 5;
    tagNumbers["GROUP HQ"] = 6;
    
    return tagNumbers.at(name);
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
    positions.push_back(Vec2(0, -4096));
    
    return positions.at(itemNumber);
}

std::vector<Point> ConfigStorage::getMainHubPositionForHighlightElements(std::string categoryName)
{
    std::map<std::string, std::vector<Point>> positions;
    
    positions["PLAY"] = std::vector<Point> {Point(600, 75), Point(400,400)};
    positions["LISTEN"] = std::vector<Point> {Point(-800, -700), Point(-1050, -475)};
    positions["WATCH"] = std::vector<Point> {Point(-1050, 75), Point(-700, 400)};
    positions["ART"] = std::vector<Point> {Point(1950, 500), Point(1750, 350)};
    
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
    iconNames["AUDIO"] = "listen";
    iconNames["GAME"] = "play";
    iconNames["GROUP"] = "play";
    
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

//--------------------------- UserDefaults First Time User for Slideshow------------

#define USERDEFAULTS_FIRST_SLIDE_SHOW "FirstSlideShowSeen"
#define SEEN_FIRST_SLIDE_SHOW_YES "YES"

void ConfigStorage::setFirstSlideShowSeen()
{
    UserDefault::getInstance()->setStringForKey(USERDEFAULTS_FIRST_SLIDE_SHOW, SEEN_FIRST_SLIDE_SHOW_YES);
}

bool ConfigStorage::shouldShowFirstSlideShowScene()
{
    if(UserDefault::getInstance()->getStringForKey(USERDEFAULTS_FIRST_SLIDE_SHOW) == SEEN_FIRST_SLIDE_SHOW_YES)
        return false;
    
    return true;
}


