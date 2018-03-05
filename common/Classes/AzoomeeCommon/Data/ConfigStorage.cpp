//ConfigStorage is a singleton designed to hold all necessary "burnt-in" information for all visual scenes and layers.

#include "ConfigStorage.h"
#include "Parent/ParentDataProvider.h"
#include "Child/ChildDataProvider.h"
#include "../Analytics/AnalyticsSingleton.h"
#include "../API/API.h"
#include "../Net/Utils.h"
#include "Json.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"

static const std::string kAzoomeeActivityJavaClassName = "org/cocos2dx/cpp/AppActivity";
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "../Utils/IosNativeFunctionsSingleton.h"
#endif

using namespace cocos2d;

namespace Azoomee
{
static ConfigStorage *_sharedConfigStorage = NULL;
    
    const char* const ConfigStorage::kGameHQName = "GAME HQ";
    const char* const ConfigStorage::kVideoHQName = "VIDEO HQ";
    const char* const ConfigStorage::kAudioHQName = "AUDIO HQ";
    const char* const ConfigStorage::kGroupHQName = "GROUP HQ";
    const char* const ConfigStorage::kHomeHQName = "HOME";
    const char* const ConfigStorage::kArtAppHQName = "ARTS APP";
    
    const char* const ConfigStorage::kDefaultHQName = kGameHQName;
    
    const char* const ConfigStorage::kRecentlyPlayedCarouselName = "LAST PLAYED";
    
    const char* const ConfigStorage::kContentTypeVideo = "VIDEO";
    const char* const ConfigStorage::kContentTypeAudio = "AUDIO";
    const char* const ConfigStorage::kContentTypeGame = "GAME";
    const char* const ConfigStorage::kContentTypeGroup = "GROUP";
    const char* const ConfigStorage::kContentTypeAudioGroup = "AUDIOGROUP";
    
    const char* const ConfigStorage::kEstimatedKeyboardHeightPortrait = "Azoomee::MessageComposer::EstimatedKeyboardHeight/Portrait";
    const char* const ConfigStorage::kEstimatedKeyboardHeightLandscape = "Azoomee::MessageComposer::EstimatedKeyboardHeight/Landscape";

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
    
    BaseSceneConfiguration = parseJsonConfigurationFile("BaseSceneConfiguration.json");
    HQSceneConfiguration = parseJsonConfigurationFile("HQSceneConfiguration.json");
    NavigationConfiguration = parseJsonConfigurationFile("NavigationConfiguration.json");
    OomeeAnimationTypes = parseJsonConfigurationFile("OomeeAnimationTypes.json");
    OomeeConfiguration = parseJsonConfigurationFile("OomeeConfiguration.json");
    VersionConfiguration = parseJsonConfigurationFile("Version.json");
    IapConfiguration = parseJsonConfigurationFile("IapConfiguration.json");

    parentSignedRequestTags = {
        API::TagParentPin,
        API::TagVerifyAmazonPayment,
        API::TagVerifyGooglePayment,
        API::TagVerifyApplePayment,
        API::TagUpdateBillingData,
        API::TagGetAvailableChildren,
        API::TagUpdateChild,
        API::TagDeleteChild,
        API::TagGetPendingFriendRequests,
        API::TagFriendRequest,
        API::TagFriendRequestReaction,
        API::TagResetReportedChat,
        API::TagCookieRefresh
    };
    requestTagsRequireImmediateSending = {
        kGroupHQName,
        kVideoHQName,
        kAudioHQName,
        kGameHQName,
        kHomeHQName,
        API::TagLogin,
        API::TagChildLogin,
        API::TagParentPin,
        API::TagVerifyGooglePayment,
        API::TagVerifyAmazonPayment,
        API::TagVerifyApplePayment,
        API::TagGetAvailableChildren
    };
    
    
    return true;
}

std::string ConfigStorage::getFileNameFromUrl(const std::string& url)
{
    int startPoint = (int)url.find_last_of("/") + 1;
    
    int endPoint = (int)url.length();
    if(url.find("?", 0) != url.npos) endPoint = (int)url.find("?", 0);
    int subLength = endPoint - startPoint;
    
    return url.substr(startPoint, subLength);
}

//-------------------------PRIVATE METHOD TO PARSE CONFIG JSON FILE--------

rapidjson::Document ConfigStorage::parseJsonConfigurationFile(const std::string& fileName)
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
#ifdef USINGCI
    return "api.elb.ci.azoomee.ninja";
#endif
    return "api.azoomee.com";
}
    
std::string ConfigStorage::getServerUrlPrefix()
{
#ifdef USINGCI
    return "http://";
#endif
    return "https://";
}

std::string ConfigStorage::getServerUrl()
{
    return getServerUrlPrefix() + getServerHost();
}
    
std::string ConfigStorage::getCTAPackageJsonURL()
{
#ifdef USINGCI
    return "https://media.azoomee.ninja/static/popups/package.json";
#endif
    return "https://media.azoomee.com/static/popups/package.json";
}
    
std::string ConfigStorage::getMediaPrefixForXwalkCookies()
{
    return "https://media";
}

std::string ConfigStorage::getPathForTag(const std::string& httpRequestTag)
{
    if(httpRequestTag == API::TagLogin) return "/api/auth/login";
    if(httpRequestTag == API::TagAnonymousDeviceLogin) return "/api/anonymoususer/auth/login";
    if(httpRequestTag == API::TagGetAvailableChildren) return StringUtils::format("/api/user/adult/%s/owns", ParentDataProvider::getInstance()->getLoggedInParentId().c_str());
    if(httpRequestTag == API::TagChildLogin) return "/api/auth/switchProfile";
    if(httpRequestTag == API::TagGetGorden) return "/api/porthole/pixel/gordon.png";
    if(httpRequestTag == API::TagRegisterParent) return "/api/user/v2/adult";
    if(httpRequestTag == API::TagRegisterChild) return "/api/user/child";
    if(httpRequestTag == API::TagDeleteChild) return "/api/user/child/";
    if(httpRequestTag == API::TagParentPin) return StringUtils::format("/api/user/adult/%s", ParentDataProvider::getInstance()->getLoggedInParentId().c_str());
    if(httpRequestTag == API::TagVerifyAmazonPayment) return StringUtils::format("/api/billing/amazon/user/%s/receipt", ParentDataProvider::getInstance()->getLoggedInParentId().c_str());
    if(httpRequestTag == API::TagVerifyApplePayment) return StringUtils::format("/api/billing/apple/user/%s/receipt", ParentDataProvider::getInstance()->getLoggedInParentId().c_str());
    if(httpRequestTag == API::TagVerifyGooglePayment) return StringUtils::format("/api/billing/google/user/%s/receipt", ParentDataProvider::getInstance()->getLoggedInParentId().c_str());
    if(httpRequestTag == API::TagUpdateBillingData) return StringUtils::format("/api/billing/user/%s/billingStatus", ParentDataProvider::getInstance()->getLoggedInParentId().c_str());
    if(httpRequestTag == API::TagGetPendingFriendRequests) return StringUtils::format("/api/user/adult/%s/invite/code/received", ParentDataProvider::getInstance()->getLoggedInParentId().c_str());
    if(httpRequestTag == API::TagCookieRefresh) return "/api/cookie/refresh/adult";
    
    return "";
}
    
bool ConfigStorage::isParentSignatureRequiredForRequest(const std::string& requestTag)
{
    auto itemPosition = std::find(parentSignedRequestTags.begin(), parentSignedRequestTags.end(), requestTag);
    return itemPosition != parentSignedRequestTags.end();
}
    
bool ConfigStorage::isImmediateRequestSendingRequired(const std::string& requestTag)
{
    auto itemPosition = std::find(requestTagsRequireImmediateSending.begin(), requestTagsRequireImmediateSending.end(), requestTag);
    return itemPosition != requestTagsRequireImmediateSending.end();
}

//-------------------------Oomee settings---------------------------
std::string ConfigStorage::getNameForOomee(int number)
{
    std::string keyName = StringUtils::format("%d", number);
    return OomeeConfiguration["nameForOomee"][keyName.c_str()].GetString();
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

int ConfigStorage::getOomeeNumberForUrl(const std::string& url)
{
    std::string fileName = getFileNameFromUrl(url);
    
    if(OomeeConfiguration["oomeeNumberForUrl"].HasMember(fileName.c_str())) return OomeeConfiguration["oomeeNumberForUrl"][fileName.c_str()].GetInt();
    else return 0;
}

//-------------------------BASESCENE CONFIGURATION-------------------------
Point ConfigStorage::getHQScenePositions(const std::string& hqSceneName)
{
    float x = BaseSceneConfiguration["HQScenePositions"][hqSceneName.c_str()]["x"].GetDouble();
    float y = BaseSceneConfiguration["HQScenePositions"][hqSceneName.c_str()]["y"].GetDouble();
    
    return Point(x,y);
}

//-------------------------HQSCENEELEMENT CONFIGURATION-------------------------

cocos2d::Size ConfigStorage::getSizeForContentItemInCategory(const std::string& category)
{
    float width = HQSceneConfiguration["sizeForContentLayerInCategory"][category.c_str()]["width"].GetDouble();
    float height = HQSceneConfiguration["sizeForContentLayerInCategory"][category.c_str()]["height"].GetDouble();
    
    return Size(width, height);
}

std::string ConfigStorage::getPlaceholderImageForContentItemInCategory(const std::string& type)
{
    return HQSceneConfiguration["placeholderImageForContentItemInCategory"][type.c_str()].GetString();
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
    
int ConfigStorage::getContentItemImageValidityInSeconds()
{
    return HQSceneConfiguration["ContentItemImageValidityInSeconds"].GetInt();
}
    
float ConfigStorage::getGroupContentItemTextHeight()
{
    return HQSceneConfiguration["groupContentItemTextHeight"].GetDouble();
}

std::vector<Point> ConfigStorage::getMainHubPositionForHighlightElements(const std::string& categoryName)
{
    float x1 = HQSceneConfiguration["MainHubPositionsForHighlightElements"][categoryName.c_str()]["Points"][0]["x"].GetDouble();
    float y1 = HQSceneConfiguration["MainHubPositionsForHighlightElements"][categoryName.c_str()]["Points"][0]["y"].GetDouble();
    float x2 = HQSceneConfiguration["MainHubPositionsForHighlightElements"][categoryName.c_str()]["Points"][1]["x"].GetDouble();
    float y2 = HQSceneConfiguration["MainHubPositionsForHighlightElements"][categoryName.c_str()]["Points"][1]["y"].GetDouble();
        
    return std::vector<Point> {Point(x1, y1), Point(x2, y2)};
}
    
cocos2d::Color4B ConfigStorage::getColourForElementType(const std::string& type)
{
    Color4B returnColor;
    returnColor.r = HQSceneConfiguration["colourForElementType"][type.c_str()]["r"].GetInt();
    returnColor.g = HQSceneConfiguration["colourForElementType"][type.c_str()]["g"].GetInt();
    returnColor.b = HQSceneConfiguration["colourForElementType"][type.c_str()]["b"].GetInt();
    returnColor.a = HQSceneConfiguration["colourForElementType"][type.c_str()]["a"].GetInt();
        
    return returnColor;
}
    
std::string ConfigStorage::getIconNameForCategory(const std::string& category)
{
    return HQSceneConfiguration["iconNameForCategory"][category.c_str()].GetString();
}
    
std::string ConfigStorage::getGradientImageForCategory(const std::string& category)
{
    if(category == kVideoHQName || category == kGroupHQName || category == kHomeHQName )
    {
        return "res/hqscene/gradient_overlay.png";
    }
    else
    {
        return "res/hqscene/gradient_overlay_large.png";
    }
}
    
//------------------NAVIGATIONLAYER CONFIGURATION--------------------------------

std::string ConfigStorage::getHQSceneNameReplacementForPermissionFeed(const std::string &inputHqSceneName)
{
    const rapidjson::Value& permissionField = NavigationConfiguration["hqNamesReplacementForPermissionFeed"];
    const std::string& returnValue = getStringFromJson(inputHqSceneName, permissionField);
    
    if(returnValue == "")
    {
        return inputHqSceneName;
    }
    else
    {
        return returnValue;
    }
}

cocos2d::Point ConfigStorage::getRelativeCirclePositionForMenuItem(int itemNumber)
{
    //Gets the relative position to keep the navigation buttons in a circle
    float x = NavigationConfiguration["relativeCirclePositionsForMenuItems"]["positions"][itemNumber]["x"].GetDouble();
    float y = NavigationConfiguration["relativeCirclePositionsForMenuItems"]["positions"][itemNumber]["y"].GetDouble();
    
    return Point(x, y);
}

cocos2d::Point ConfigStorage::getHorizontalPositionForMenuItem(int itemNumber)
{
    cocos2d::Point visualOrigin = Director::getInstance()->getVisibleOrigin();
    cocos2d::Size visualSize = Director::getInstance()->getVisibleSize();
    
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
    cocos2d::Point visualOrigin = Director::getInstance()->getVisibleOrigin();
    cocos2d::Size visualSize = Director::getInstance()->getVisibleSize();
    
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

ConfigStorage::HubTargetTagNumber ConfigStorage::getTagNumberForMenuName(const std::string& name)
{
    return (HubTargetTagNumber)NavigationConfiguration["tagNumberForMenuItems"][name.c_str()].GetInt();
}

Point ConfigStorage::getTargetPositionForMove(int itemNumber)
{
    float x = NavigationConfiguration["targetPositionsForMove"][itemNumber]["x"].GetDouble();
    float y = NavigationConfiguration["targetPositionsForMove"][itemNumber]["y"].GetDouble();
    
    return Point(x, y);
}


//-----------------------------------OOMEE animation identifier configuration----------------------------------

std::string ConfigStorage::getGreetingAnimation()
{
    return "Build_Simple_Wave";
}

std::string ConfigStorage::getRandomIdForAnimationType(const std::string& animationType)
{
    if(animationType == "idle") return OomeeAnimationTypes["idleAnimations"][random(0, (int)OomeeAnimationTypes["idleAnimations"].Size() - 1)].GetString();
    else if(animationType == "button") return OomeeAnimationTypes["buttonIdleAnimations"][random(0, (int)OomeeAnimationTypes["buttonIdleAnimations"].Size() - 1)].GetString();
    else return OomeeAnimationTypes["touchAnimations"][random(0, (int)OomeeAnimationTypes["touchAnimations"].Size() - 1)].GetString();
}

//--------------------------- UserDefaults First Time User for Slideshow------------

#define USERDEFAULTS_FIRST_SLIDE_SHOW_SEEN "FirstSlideShowSeen"

void ConfigStorage::setFirstSlideShowSeen()
{
    UserDefault::getInstance()->setBoolForKey(USERDEFAULTS_FIRST_SLIDE_SHOW_SEEN, true);
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
    
std::string ConfigStorage::getVersionInformationForRequestHeader()
{
    return getOSManufacturer() + "/" + getVersionNumberWithPlatform();
}

//----------------------------- IAP Configuration -------------------------------------

std::string ConfigStorage::getIapSkuForProvider(const std::string& provider)
{
    return IapConfiguration[provider.c_str()].GetString();
}

std::string ConfigStorage::getDeveloperPublicKey()
{
    std::string devKey = "MJKBJlAODglshlkG:y0CCQFHABQCBS8BOIJDChMCBSEB{uhXV3FZlUQTjZZT+[m6O:XUsUevgKjNMROPZAkoiYYuNX6Ws905wZtWCFiGyz[OAjroRYmtEUOKaD;b8HVK:pxUSUrfI6VgsAEfHKzHTO6WgaF7ZjV/3{WxPOSHijdf7vEn[D4kkB8j2QGqmGVV4F5UtURi-my2jSsNIZEhV0jQbmRabR4GRivkmuDWKiqTp0pQ9wc+iH8H|OuYnFYlCmfu1Ysk7n[/[;OioIsHGUhd{UiBJa6Y3hFPqV5TqRNyz3TVdUgqSPCq4vjtTDmWV/BLfXh3V7vZonPKbkRjGcZ5V5ZJHDfoL[9EgFbSBo1w::Qd8eNwjGMDlIGOQGN\\MRKDBSAC";
    
    for(int i = 0; i < devKey.size(); i++)
    {
        devKey[i] -= i % 3;
    }
    
    return(devKey);
}
    
//----------------------------- Device specific information -----------------------------

std::string ConfigStorage::getDeviceInformation()
{
    std::string sourceDeviceString1 = "NA";
    std::string sourceDeviceString2 = "NA";
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    sourceDeviceString1 = IosNativeFunctionsSingleton::getInstance()->getIosSystemVersion();
    sourceDeviceString2 = IosNativeFunctionsSingleton::getInstance()->getIosDeviceType();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    sourceDeviceString1 = JniHelper::callStaticStringMethod(kAzoomeeActivityJavaClassName, "getAndroidDeviceModel");
    sourceDeviceString2 = JniHelper::callStaticStringMethod(kAzoomeeActivityJavaClassName, "getOSBuildManufacturer");
#endif
    
    std::string sourceDevice = Net::urlEncode(sourceDeviceString1) + "|" + Net::urlEncode(sourceDeviceString2);
    
    return sourceDevice;
}

std::string ConfigStorage::getDeviceAdvertisingId()
{
    std::string deviceId = "";
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    deviceId = IosNativeFunctionsSingleton::getInstance()->getIosDeviceIDFA();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    deviceId = JniHelper::callStaticStringMethod(kAzoomeeActivityJavaClassName, "getAndroidDeviceAdvertisingId");
#endif
    
    return deviceId;
}
    
std::string ConfigStorage::getOSManufacturer()
{
    if(_osManufacturer != "")
    {
        return _osManufacturer;
    }
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
    std::string resultStr = JniHelper::callStaticStringMethod(kAzoomeeActivityJavaClassName, "getOSBuildManufacturer");
    
    if (resultStr == "Amazon")
    {
        AnalyticsSingleton::getInstance()->registerIAPOS("Amazon");
        _osManufacturer = "Amazon";
    }
    else
    {
        AnalyticsSingleton::getInstance()->registerIAPOS("Google");
        _osManufacturer = "Google";
    }
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    AnalyticsSingleton::getInstance()->registerIAPOS("iOS");
    _osManufacturer = "Apple";
#else
    _osManufacturer = "Unknown";
#endif
    
    return _osManufacturer;
}
    
void ConfigStorage::setIsDeviceIphoneX(bool isDeviceIphoneX)
{
    _isDeviceIphoneX = isDeviceIphoneX;
}
    
bool ConfigStorage::isDeviceIphoneX() const
{
    return _isDeviceIphoneX;
}

//------------------------- Set estimated keyboard height for chat ---------------------------
void ConfigStorage::setEstimatedKeyboardHeight(float height)
{
    cocos2d::log("KEYBOARD estimated keyboard height received: %f", height);
    
    // Ignore tiny values
    // Sometimes device reports a 0 or small height (e.g Android devices can report the size of the
    // black bottom bar which sometimes appears with or without the keyboard).
    if(height < 100.0f)
    {
        return;
    }
    
    const Size& nativeScreenSize = Director::getInstance()->getOpenGLView()->getFrameSize();
    const bool currentOrientationPortrait = (nativeScreenSize.height > nativeScreenSize.width);
    
    if(currentOrientationPortrait)
    {
        UserDefault::getInstance()->setFloatForKey(kEstimatedKeyboardHeightPortrait, height);
    }
    else
    {
        UserDefault::getInstance()->setFloatForKey(kEstimatedKeyboardHeightLandscape, height);
    }
}

    
}
