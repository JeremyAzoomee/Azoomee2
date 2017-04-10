//ConfigStorage is a singleton designed to hold all necessary "burnt-in" information for all visual scenes and layers.

#include "ConfigStorage.h"
#include "ParentDataProvider.h"
#include "AnalyticsSingleton.h"

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
    
    visualOrigin = Director::getInstance()->getVisibleOrigin();
    visualSize = Director::getInstance()->getVisibleSize();
    
    BaseSceneConfiguration = parseJsonConfigurationFile("BaseSceneConfiguration.json");
    HQSceneConfiguration = parseJsonConfigurationFile("HQSceneConfiguration.json");
    ImageContainerConfiguration = parseJsonConfigurationFile("ImageContainerConfiguration.json");
    NavigationConfiguration = parseJsonConfigurationFile("NavigationConfiguration.json");
    OomeeAnimationTypes = parseJsonConfigurationFile("OomeeAnimationTypes.json");
    OomeeConfiguration = parseJsonConfigurationFile("OomeeConfiguration.json");
    VersionConfiguration = parseJsonConfigurationFile("Version.json");
    IapConfiguration = parseJsonConfigurationFile("IapConfiguration.json");
    
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

//-------------------------PRIVATE METHOD TO PARSE CONFIG JSON FILE--------

rapidjson::Document ConfigStorage::parseJsonConfigurationFile(std::string fileName)
{
    std::string path = "res/configuration/" + fileName;
    
    std::string jsonString;
    
    if(!FileUtils::getInstance()->isFileExist(path)) jsonString = "";
    else jsonString = FileUtils::getInstance()->getStringFromFile(path);
    
    rapidjson::Document returnObject;
    returnObject.Parse(jsonString.c_str());
    
    return returnObject;
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
    if(httpRequestTag == "iapAmazonPaymentMade") return StringUtils::format("/api/billing/amazon/user/%s/receipt", ParentDataProvider::getInstance()->getLoggedInParentId().c_str());
    if(httpRequestTag == "updateBilling") return StringUtils::format("/api/billing/user/%s/billingStatus", ParentDataProvider::getInstance()->getLoggedInParentId().c_str());
    
    return "";
}

//-------------------------Oomee settings---------------------------
std::string ConfigStorage::getNameForOomee(int number)
{
    std::string keyName = StringUtils::format("%d", number);
    return OomeeConfiguration["nameForOomee"][keyName.c_str()].GetString();
}

std::string ConfigStorage::getOomeePNGName(int number)
{
    std::string path1 = OomeeConfiguration["pathForOomeeImages"].GetString();
    std::string path2 = getNameForOomee(number);
    
    return path1 + path2;
}

std::string ConfigStorage::getHumanReadableNameForOomee(int number)
{
    std::string keyName = StringUtils::format("%d", number);
    return OomeeConfiguration["humanReadableNameForOomee"][keyName.c_str()].GetString();
}

std::string ConfigStorage::getUrlForOomee(int number)
{
    std::string keyName = StringUtils::format("%d", number);
    return OomeeConfiguration["urlForOomee"][keyName.c_str()].GetString();
}

int ConfigStorage::getOomeeNumberForUrl(std::string url)
{
    std::string fileName = getFileNameFromUrl(url);
    
    if(OomeeConfiguration["oomeeNumberForUrl"].HasMember(fileName.c_str())) return OomeeConfiguration["oomeeNumberForUrl"][fileName.c_str()].GetInt();
    else return 0;
}

//-------------------------BASESCENE CONFIGURATION-------------------------
Point ConfigStorage::getHQScenePositions(std::string hqSceneName)
{
    float x = BaseSceneConfiguration["HQScenePositions"][hqSceneName.c_str()]["x"].GetDouble();
    float y = BaseSceneConfiguration["HQScenePositions"][hqSceneName.c_str()]["y"].GetDouble();
    
    return Point(x,y);
}

//-------------------------HQSCENEELEMENT CONFIGURATION-------------------------

cocos2d::Size ConfigStorage::getSizeForContentItemInCategory(std::string category)
{
    float width = HQSceneConfiguration["sizeForContentLayerInCategory"][category.c_str()]["width"].GetDouble();
    float height = HQSceneConfiguration["sizeForContentLayerInCategory"][category.c_str()]["height"].GetDouble();
    
    return Size(width, height);
}

cocos2d::Color4B ConfigStorage::getBaseColourForContentItemInCategory(std::string category)
{
    Color4B returnColour;
    
    returnColour.r = HQSceneConfiguration["baseColourForContentItemInCategory"][category.c_str()]["r"].GetInt();
    returnColour.g = HQSceneConfiguration["baseColourForContentItemInCategory"][category.c_str()]["g"].GetInt();
    returnColour.b = HQSceneConfiguration["baseColourForContentItemInCategory"][category.c_str()]["b"].GetInt();
    returnColour.a = HQSceneConfiguration["baseColourForContentItemInCategory"][category.c_str()]["a"].GetInt();
    
    return returnColour;
}

std::string ConfigStorage::getIconImagesForContentItemInCategory(std::string category)
{
    return HQSceneConfiguration["iconImagesForContentItemInCategory"][category.c_str()].GetString();
}

std::string ConfigStorage::getPlaceholderImageForContentItemInCategory(std::string type)
{
    return HQSceneConfiguration["placeholderImageForContentItemInCategory"][type.c_str()].GetString();
}

cocos2d::Vec2 ConfigStorage::getHighlightSizeMultiplierForContentItem(int highlightClass)
{
    std::string keyName = StringUtils::format("%d", highlightClass);
    float x = HQSceneConfiguration["highlightSizeMultiplierForContentItem"][keyName.c_str()]["x"].GetDouble();
    float y = HQSceneConfiguration["highlightSizeMultiplierForContentItem"][keyName.c_str()]["y"].GetDouble();
    
    return Vec2(x, y);
}

float ConfigStorage::getScrollviewTitleTextHeight()
{
    return HQSceneConfiguration["scrollViewTextHeight"].GetDouble();
}

Size ConfigStorage::getGroupHQLogoSize()
{
    float width = HQSceneConfiguration["groupLogoSize"]["width"].GetDouble();
    float height = HQSceneConfiguration["groupLogoSize"]["height"].GetDouble();
    
    return Size(width, height);
}

//------------------NAVIGATIONLAYER CONFIGURATION--------------------------------

cocos2d::Point ConfigStorage::getCirclePositionForMenuItem(int itemNumber)
{
    float x = NavigationConfiguration["circlePositionsForMenuItems"]["positions"][itemNumber]["x"].GetDouble();
    float y = NavigationConfiguration["circlePositionsForMenuItems"]["positions"][itemNumber]["y"].GetDouble();
    
    return Point(x, y);
}

cocos2d::Point ConfigStorage::getHorizontalPositionForMenuItem(int itemNumber)
{
    float x = NavigationConfiguration["horizontalXPositionsForMenuItems"][itemNumber].GetDouble();
    float y = visualOrigin.y + visualSize.height + NavigationConfiguration["horizontalYPositionsForMenuItems"].GetDouble();
    
    return Point(x, y);
}

float ConfigStorage::getHorizontalMenuItemsHeight()
{
    return NavigationConfiguration["horizontalMenuItemsHeight"].GetDouble();
}

cocos2d::Point ConfigStorage::getHorizontalPositionForMenuItemInGroupHQ(int itemNumber)
{
    float x = NavigationConfiguration["horizontalXPositionsForMenuItems"][itemNumber].GetDouble();
    float y = visualOrigin.y + visualSize.height + NavigationConfiguration["horizontalYPositionsForMenuItemsInGroupHQ"].GetDouble();
    
    return Point(x, y);
}

cocos2d::Color4B ConfigStorage::getColourForMenuItem(int itemNumber)
{
    Color4B returnColour;
    
    returnColour.r = NavigationConfiguration["coloursForMenuItems"][itemNumber]["r"].GetInt();
    returnColour.g = NavigationConfiguration["coloursForMenuItems"][itemNumber]["g"].GetInt();
    returnColour.b = NavigationConfiguration["coloursForMenuItems"][itemNumber]["b"].GetInt();
    returnColour.a = NavigationConfiguration["coloursForMenuItems"][itemNumber]["a"].GetInt();
    
    return returnColour;
}

std::string ConfigStorage::getNameForMenuItem(int itemNumber)
{
    return NavigationConfiguration["namesForMenuItems"][itemNumber].GetString();
}

int ConfigStorage::getTagNumberForMenuName(std::string name)
{
    return NavigationConfiguration["tagNumberForMenuItems"][name.c_str()].GetInt();
}

Point ConfigStorage::getTargetPositionForMove(int itemNumber)
{
    float x = NavigationConfiguration["targetPositionsForMove"][itemNumber]["x"].GetDouble();
    float y = NavigationConfiguration["targetPositionsForMove"][itemNumber]["y"].GetDouble();
    
    return Point(x, y);
}

//--------------------------------IMAGECONTAINER CONFIGURATION----------------

std::vector<Point> ConfigStorage::getMainHubPositionForHighlightElements(std::string categoryName)
{
    float x1 = ImageContainerConfiguration["MainHubPositionsForHighlightElements"][categoryName.c_str()]["Points"][0]["x"].GetDouble();
    float y1 = ImageContainerConfiguration["MainHubPositionsForHighlightElements"][categoryName.c_str()]["Points"][0]["y"].GetDouble();
    float x2 = ImageContainerConfiguration["MainHubPositionsForHighlightElements"][categoryName.c_str()]["Points"][1]["x"].GetDouble();
    float y2 = ImageContainerConfiguration["MainHubPositionsForHighlightElements"][categoryName.c_str()]["Points"][1]["y"].GetDouble();
    
    return std::vector<Point> {Point(x1, y1), Point(x2, y2)};
}

cocos2d::Color4B ConfigStorage::getColourForElementType(std::string type)
{
    Color4B returnColor;
    returnColor.r = ImageContainerConfiguration["colourForElementType"][type.c_str()]["r"].GetInt();
    returnColor.g = ImageContainerConfiguration["colourForElementType"][type.c_str()]["g"].GetInt();
    returnColor.b = ImageContainerConfiguration["colourForElementType"][type.c_str()]["b"].GetInt();
    returnColor.a = ImageContainerConfiguration["colourForElementType"][type.c_str()]["a"].GetInt();
    
    return returnColor;
}

std::string ConfigStorage::getIconNameForCategory(std::string category)
{
    return ImageContainerConfiguration["iconNameForCategory"][category.c_str()].GetString();
}

std::string ConfigStorage::getGradientImageForCategory(std::string category)
{
    if(category == "VIDEO HQ" || category == "GROUP HQ" )
        return "res/hqscene/gradient_overlay.png";
    else
        return "res/hqscene/gradient_overlay_large.png";
}

//-----------------------------------OOMEE animation identifier configuration----------------------------------

std::string ConfigStorage::getGreetingAnimation()
{
    return "Build_Simple_Wave";
}

std::string ConfigStorage::getRandomIdForAnimationType(std::string animationType)
{
    if(animationType == "idle") return OomeeAnimationTypes["idleAnimations"][random(0, (int)OomeeAnimationTypes["idleAnimations"].Size() - 1)].GetString();
    else return OomeeAnimationTypes["touchAnimations"][random(0, (int)OomeeAnimationTypes["touchAnimations"].Size() - 1)].GetString();
}

//--------------------------- UserDefaults First Time User for Slideshow------------

#define USERDEFAULTS_FIRST_SLIDE_SHOW_SEEN "FirstSlideShowSeen"

void ConfigStorage::setFirstSlideShowSeen()
{
    UserDefault::getInstance()->setBoolForKey(USERDEFAULTS_FIRST_SLIDE_SHOW_SEEN, true);
    AnalyticsSingleton::getInstance()->firstLaunchEvent();
}

bool ConfigStorage::shouldShowFirstSlideShowScene()
{
    return !UserDefault::getInstance()->getBoolForKey(USERDEFAULTS_FIRST_SLIDE_SHOW_SEEN, false);
}

//---------------------------- Version configuration --------------------------------

std::string ConfigStorage::getVersionNumber()
{
    return VersionConfiguration["version"].GetString();
}

std::string ConfigStorage::getVersionNumberWithPlatform()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    return "Android/" + getVersionNumber();
#else
    return "IOS/" + getVersionNumber();
#endif
}

std::string ConfigStorage::getVersionNumberToDisplay()
{
    return "Version Number " + getVersionNumber();
}

//----------------------------- IAP Configuration -------------------------------------

std::string ConfigStorage::getIapSkuForProvider(std::string provider)
{
    return IapConfiguration[provider.c_str()].GetString();
}
