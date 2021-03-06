//ConfigStorage is a singleton designed to hold all necessary "burnt-in" information for all visual scenes and layers.

#include "ConfigStorage.h"
#include "Parent/ParentManager.h"
#include "../Analytics/AnalyticsSingleton.h"
#include "../API/API.h"
#include "../Net/Utils.h"
#include "Json.h"
#include "../Utils/StringFunctions.h"
#include "../Utils/DirUtil.h"
#include "../Strings.h"

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
    
    const char* const ConfigStorage::kChatHQName = "CHAT";
    const char* const ConfigStorage::kGameHQName = "GAME HQ";
    const char* const ConfigStorage::kVideoHQName = "VIDEO HQ";
    const char* const ConfigStorage::kAudioHQName = "AUDIO HQ";
    const char* const ConfigStorage::kGroupHQName = "GROUP HQ";
    const char* const ConfigStorage::kHomeHQName = "HOME";
    const char* const ConfigStorage::kArtAppHQName = "ARTS APP";
    const char* const ConfigStorage::kMeHQName = "ME HQ";
    
    std::string ConfigStorage::kDefaultHQName = ConfigStorage::kMeHQName;
    
    const char* const ConfigStorage::kRecentlyPlayedCarouselName = "LAST PLAYED";
    
    const char* const ConfigStorage::kContentNodeName = "contentNode";
    const char* const ConfigStorage::kContentLayerName = "contentLayer";
    const char* const ConfigStorage::kNavigationLayerName = "NavigationLayer";
	const char* const ConfigStorage::kIosWebviewName = "iosWebView";
	const char* const ConfigStorage::kAndroidWebviewName = "androidWebView";
    
    const char* const ConfigStorage::kContentTypeVideo = "VIDEO";
    const char* const ConfigStorage::kContentTypeAudio = "AUDIO";
    const char* const ConfigStorage::kContentTypeGame = "GAME";
    const char* const ConfigStorage::kContentTypeGroup = "GROUP";
    const char* const ConfigStorage::kContentTypeAudioGroup = "AUDIOGROUP";
    const char* const ConfigStorage::kContentTypeManual = "MANUAL";
    const char* const ConfigStorage::kContentTypeInternal = "INTERNAL";

    const char* const ConfigStorage::kContentLocFeature = "feature";
    const char* const ConfigStorage::kContentLocRecentPlayed = "recentPlayed";
    const char* const ConfigStorage::kContentLocFavourite = "favourite";
    const char* const ConfigStorage::kContentLocEpisodeSelector = "episodeSelector";
    const char* const ConfigStorage::kContentLocCategory = "category";
    const char* const ConfigStorage::kContentLocOffline = "offline";
    
    const char* const ConfigStorage::kOomeeMakerURI = "OOMEE_MAKER";
    const char* const ConfigStorage::kArtAppURI = "AZOOMEE_ART";
    
    const char* const ConfigStorage::kEstimatedKeyboardHeightPortrait = "Azoomee::MessageComposer::EstimatedKeyboardHeight/Portrait";
    const char* const ConfigStorage::kEstimatedKeyboardHeightLandscape = "Azoomee::MessageComposer::EstimatedKeyboardHeight/Landscape";
    
    const std::string ConfigStorage::kArtCacheFolder = "artCache/";
    const std::string ConfigStorage::kGameCacheFolder = "gameCache/";
	const std::string ConfigStorage::kOomeeMakerCacheFolder = "oomeeMaker/";
    
    const std::string ConfigStorage::kAvatarImageCacheFolder = "avatars";
    
    const std::string ConfigStorage::kGameDownloadError = "ERROR";
	
	const std::string ConfigStorage::kIOSSubURL = "https://buy.itunes.apple.com/WebObjects/MZFinance.woa/wa/DirectAction/manageSubscriptions";
	const std::string ConfigStorage::kAndroidSubURL = "https://play.google.com/store/account/subscriptions";
	const std::string ConfigStorage::kAmazonSubURL = "amzn://apps/library/subscriptions";

	const std::string ConfigStorage::kBillingProviderApple = "APPLE";
	const std::string ConfigStorage::kBillingProviderGoogle = "GOOGLE";
	const std::string ConfigStorage::kBillingProviderAmazon = "AMAZON";
	const std::string ConfigStorage::kBillingProviderVodacomSA = "VODACOM_SA";
	
	const std::string ConfigStorage::kAnonLoginPW = "ToBeDecided";
	const char* const ConfigStorage::kAnonEmailKey = "anonEmail";
	const char* const ConfigStorage::kStoredUsernameKey = "username";
	const char* const ConfigStorage::kAnonOnboardingCompleteKey = "anonOnboardingComplete";
	
    
    const std::string ConfigStorage::kOSManufacturerApple = "Apple";
    const std::string ConfigStorage::kOSManufacturerGoogle = "Google";
    const std::string ConfigStorage::kOSManufacturerAmazon = "Amazon";
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
     const std::string ConfigStorage::kSignupPlatformSource = "IOS_INAPP";
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
     const std::string ConfigStorage::kSignupPlatformSource = "ANDROID_INAPP";
#else
     const std::string ConfigStorage::kSignupPlatformSource = "OTHER";
#endif
   
    
    
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
        API::TagCookieRefresh,
		API::TagAddVoucher,
		API::TagUpdateParentPassword,
		API::TagUpdateParentDetails,
		API::TagUpdateChildNameRequest,
		API::TagGetParentDetails,
		API::TagGetVodacomTransactionId,
		API::TagRegisterParent,
        API::TagGetMarketingAssets
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
    
std::string ConfigStorage::getGameCachePath()
{
    return DirUtil::getCachesPath() + kGameCacheFolder;
}
    
std::string ConfigStorage::getDefaultHQ()
{
    return kDefaultHQName;
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
    
    if(returnObject.HasParseError())
    {
        cocos2d::log("The following file has got a json parse error: %s", fileName.c_str());
        returnObject.SetObject();
        return returnObject;
    }
    else
    {
        return returnObject;
    }
}

//-------------------------BACKEND CALLER CONFIGURATION--------------------
std::string ConfigStorage::getServerHost()
{
#ifdef AZOOMEE_ENVIRONMENT_CI
    return "api.azoomee.ninja";
#endif
    return "api.azoomee.com";
}
    
std::string ConfigStorage::getServerUrlPrefix()
{
#ifdef AZOOMEE_ENVIRONMENT_CI
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
#ifdef AZOOMEE_ENVIRONMENT_CI
  #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    return "https://media.azoomee.ninja/static/popups/android/package.json";
  #elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    return "https://media.azoomee.ninja/static/popups/ios/package.json";
  #endif
#else
  #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    return "https://media.azoomee.com/static/popups/android/package.json";
  #elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    return "https://media.azoomee.com/static/popups/ios/package.json";
  #endif
#endif
}
    
std::string ConfigStorage::getMediaPrefixForXwalkCookies()
{
    return "https://media";
}

std::string ConfigStorage::getPathForTag(const std::string& httpRequestTag)
{
    if(httpRequestTag == API::TagLogin) return "/api/auth/login";
    if(httpRequestTag == API::TagGetAvailableChildren) return StringUtils::format("/api/user/adult/%s/owns", ParentManager::getInstance()->getLoggedInParentId().c_str());
    if(httpRequestTag == API::TagChildLogin) return "/api/auth/switchProfile";
    if(httpRequestTag == API::TagRegisterParent) return "/api/user/v2/signup";
    if(httpRequestTag == API::TagRegisterChild) return "/api/user/child";
    if(httpRequestTag == API::TagDeleteChild) return "/api/user/child/";
    if(httpRequestTag == API::TagParentPin) return StringUtils::format("/api/user/adult/%s", ParentManager::getInstance()->getLoggedInParentId().c_str());
    if(httpRequestTag == API::TagVerifyAmazonPayment) return StringUtils::format("/api/billing/amazon/user/%s/receipt", ParentManager::getInstance()->getLoggedInParentId().c_str());
    if(httpRequestTag == API::TagVerifyApplePayment) return StringUtils::format("/api/billing/apple/user/%s/receipt", ParentManager::getInstance()->getLoggedInParentId().c_str());
    if(httpRequestTag == API::TagVerifyGooglePayment) return StringUtils::format("/api/billing/google/user/%s/receipt", ParentManager::getInstance()->getLoggedInParentId().c_str());
    if(httpRequestTag == API::TagUpdateBillingData) return StringUtils::format("/api/billing/user/%s/billingStatus", ParentManager::getInstance()->getLoggedInParentId().c_str());
    if(httpRequestTag == API::TagGetPendingFriendRequests) return StringUtils::format("/api/user/adult/%s/invite/code/received", ParentManager::getInstance()->getLoggedInParentId().c_str());
    if(httpRequestTag == API::TagCookieRefresh) return "/api/cookie/refresh/adult";
    
    return "";
}
    
std::string ConfigStorage::getRemoteWebGameAPIPath()
{
#ifdef AZOOMEE_ENVIRONMENT_CI
    return "https://media.azoomee.ninja/static/webgameapi/";
#else
    return "https://media.azoomee.com/static/webgameapi/";
#endif
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
std::string ConfigStorage::getUrlForOomee(int number)
{
    std::string keyName = StringUtils::format("%d", number);
    const rapidjson::Value &oomeeurls = OomeeConfiguration["urlForOomee"];
    return getStringFromJson(keyName, oomeeurls);
}

std::string ConfigStorage::getConfigUrlForOomee(int number)
{
    std::string keyName = StringUtils::format("%d", number);
    const rapidjson::Value &configurls = OomeeConfiguration["configForOomee"];
    return getStringFromJson(keyName, configurls);
}
std::string ConfigStorage::getLocalImageForOomee(int number)
{
    std::string keyName = StringUtils::format("%d", number);
    const rapidjson::Value &oomeeurls = OomeeConfiguration["localImageForOomee"];
    return getStringFromJson(keyName, oomeeurls);
}
    
std::string ConfigStorage::getLocalConfigForOomee(int number)
{
    std::string keyName = StringUtils::format("%d", number);
    const rapidjson::Value &configurls = OomeeConfiguration["localConfigForOomee"];
    return getStringFromJson(keyName, configurls);
}

int ConfigStorage::getOomeeNumberForUrl(const std::string& url)
{
    std::string fileName = getFileNameFromUrl(url);
    const rapidjson::Value &oomeeNumbers = OomeeConfiguration["oomeeNumberForUrl"];
    return getIntFromJson(fileName, oomeeNumbers, 0);
}

//-------------------------BASESCENE CONFIGURATION-------------------------
Point ConfigStorage::getHQScenePositions(const std::string& hqSceneName)
{
    
    
    if(BaseSceneConfiguration["HQScenePositions"].HasMember(hqSceneName.c_str()))
    {
        const rapidjson::Value &scenePositionsns = BaseSceneConfiguration["HQScenePositions"][hqSceneName.c_str()];
        float x = getDoubleFromJson("x", scenePositionsns);
        float y = getDoubleFromJson("y", scenePositionsns);
        
        return Point(x,y);
    }
    else
    {
        return Point(0,0);
    }
}

//-------------------------HQSCENEELEMENT CONFIGURATION-------------------------

cocos2d::Size ConfigStorage::getSizeForContentItemInCategory(const std::string& category)
{
    if(HQSceneConfiguration["sizeForContentLayerInCategory"].HasMember(category.c_str()))
    {
        const rapidjson::Value &contentLayerSizes = HQSceneConfiguration["sizeForContentLayerInCategory"][category.c_str()];
        float width = getDoubleFromJson("width", contentLayerSizes);
        float height = getDoubleFromJson("height", contentLayerSizes);
        
        return Size(width, height);
    }
    else
    {
        return Size(0,0);
    }
}

std::string ConfigStorage::getPlaceholderImageForContentItemInCategory(const std::string& type)
{
    const rapidjson::Value &placeholders = HQSceneConfiguration["placeholderImageForContentItemInCategory"];
    return getStringFromJson(type, placeholders);
}

float ConfigStorage::getScrollviewTitleTextHeight()
{
    return getDoubleFromJson("scrollViewTextHeight", HQSceneConfiguration);
}

Size ConfigStorage::getGroupHQLogoSize()
{
    const rapidjson::Value &groupLogoSize = HQSceneConfiguration["groupLogoSize"];
    float width = getDoubleFromJson("width", groupLogoSize);
    float height = getDoubleFromJson("height", groupLogoSize);
    
    return Size(width, height);
}
    
int ConfigStorage::getContentItemImageValidityInSeconds()
{
    return getIntFromJson("ContentItemImageValidityInSeconds", HQSceneConfiguration);
}
    
float ConfigStorage::getGroupContentItemTextHeight()
{
    return getDoubleFromJson("groupContentItemTextHeight", HQSceneConfiguration);
}

std::vector<Point> ConfigStorage::getMainHubPositionForHighlightElements(const std::string& categoryName)
{
    if(HQSceneConfiguration["MainHubPositionsForHighlightElements"].HasMember(categoryName.c_str()))
    {
        const rapidjson::Value &points = HQSceneConfiguration["MainHubPositionsForHighlightElements"][categoryName.c_str()]["Points"];
        
        const rapidjson::Value &p1 = points[0];
        const rapidjson::Value &p2 = points[1];
        
        return std::vector<Point> {Point(getDoubleFromJson("x", p1), getDoubleFromJson("y", p1)), Point(getDoubleFromJson("x", p2), getDoubleFromJson("y", p2))};
    }
    else
    {
        return std::vector<Point> {Point(0,0), Point(0,0)};
    }
}
    
cocos2d::Color4B ConfigStorage::getColourForElementType(const std::string& type)
{
    Color4B returnColor = Color4B(0, 0, 0, 0);
    
    if(HQSceneConfiguration["colourForElementType"].HasMember(type.c_str()))
    {
        const rapidjson::Value &typeColour = HQSceneConfiguration["colourForElementType"][type.c_str()];
        
        returnColor.r = getIntFromJson("r", typeColour);
        returnColor.g = getIntFromJson("g", typeColour);
        returnColor.b = getIntFromJson("b", typeColour);
        returnColor.a = getIntFromJson("a", typeColour);
    }
        
    return returnColor;
}
    
std::string ConfigStorage::getIconNameForCategory(const std::string& category)
{
    return getStringFromJson(category, HQSceneConfiguration["iconNameForCategory"]);
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

std::string ConfigStorage::getHQSceneNameReplacementForPermissionFeed(const std::string &inputHqSceneName) const
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

cocos2d::Point ConfigStorage::getHorizontalPositionForMenuItem(const std::string& hqName) const
{
    cocos2d::Point visualOrigin = Vec2(0,0);//Director::getInstance()->getVisibleOrigin();
    cocos2d::Size visualSize = Director::getInstance()->getVisibleSize();
    
    float x = 0;
    float y = 0;
    
    if(NavigationConfiguration["horizontalXPositionsForMenuItems"].HasMember(hqName.c_str()))
    {
        x = NavigationConfiguration["horizontalXPositionsForMenuItems"][hqName.c_str()].GetDouble();
        y = getDoubleFromJson("horizontalYPositionsForMenuItems", NavigationConfiguration) + visualOrigin.y + visualSize.height;
    }
    
    return Point(x, y);
}

float ConfigStorage::getHorizontalMenuItemsHeight() const
{
    return getDoubleFromJson("horizontalMenuItemsHeight", NavigationConfiguration);
}

cocos2d::Point ConfigStorage::getHorizontalPositionForMenuItemInGroupHQ(const std::string& hqName) const
{
    cocos2d::Point visualOrigin = Director::getInstance()->getVisibleOrigin();
    cocos2d::Size visualSize = Director::getInstance()->getVisibleSize();
    
    float x = 0;
    float y = 0;
    
    if(NavigationConfiguration["horizontalXPositionsForMenuItems"].HasMember(hqName.c_str()))
    {
        x = NavigationConfiguration["horizontalXPositionsForMenuItems"][hqName.c_str()].GetDouble();
        y = visualOrigin.y + visualSize.height + getDoubleFromJson("horizontalYPositionsForMenuItemsInGroupHQ", NavigationConfiguration);
    }
    
    return Point(x, y);
}

cocos2d::Color4B ConfigStorage::getColourForMenuItem(const std::string& hqName) const
{
    Color4B returnColour = Color4B(0,0,0,0);
    
    if(NavigationConfiguration["coloursForMenuItems"].HasMember(hqName.c_str()))
    {
        const rapidjson::Value &itemColour = NavigationConfiguration["coloursForMenuItems"][hqName.c_str()];
        
        returnColour.r = getIntFromJson("r", itemColour);
        returnColour.g = getIntFromJson("g", itemColour);
        returnColour.b = getIntFromJson("b", itemColour);
        returnColour.a = getIntFromJson("a", itemColour);
    }
    
    return returnColour;
}

std::string ConfigStorage::getNameForMenuItem(const std::string& hqName) const
{
    if(NavigationConfiguration["namesForMenuItems"].HasMember(hqName.c_str()))
    {
        return NavigationConfiguration["namesForMenuItems"][hqName.c_str()].GetString();
    }
    else
    {
        return "";
    }
}

Point ConfigStorage::getTargetPositionForMove(const std::string& hqName) const
{
    float x = 0;
    float y = 0;
    
    if(NavigationConfiguration["targetPositionsForMove"].HasMember(hqName.c_str()))
    {
        x = getDoubleFromJson("x", NavigationConfiguration["targetPositionsForMove"][hqName.c_str()]);
        y = getDoubleFromJson("y", NavigationConfiguration["targetPositionsForMove"][hqName.c_str()]);
    }
    
    return Point(x, y);
}
    
std::vector<std::string> ConfigStorage::getHqNames() const
{
    if(_navigationHQs.size() > 0)
    {
        return _navigationHQs;
    }
    return getStringArrayFromJson(NavigationConfiguration["namesForMenuItems"]);
}

void ConfigStorage::setNavigationHQs(const std::vector<std::string>& hqs)
{
    _navigationHQs = hqs;
}

void ConfigStorage::setDefaultHQ(const std::string &defaultHq)
{
    kDefaultHQName = defaultHq;
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
    return getStringFromJson("version", VersionConfiguration);
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
    return _("Version Number") + " " + getVersionNumber();
}
    
std::string ConfigStorage::getVersionInformationForRequestHeader()
{
    return getOSManufacturer() + "/" + getVersionNumberWithPlatform();
}

//----------------------------- IAP Configuration -------------------------------------

std::string ConfigStorage::getIapSkuForProvider(const std::string& provider)
{
    return getStringFromJson(provider, IapConfiguration);
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
	
std::string ConfigStorage::getDeviceLanguage()
{
	std::string languageCode = "";
	
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	languageCode = IosNativeFunctionsSingleton::getInstance()->getIosDeviceLanguage();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	languageCode = JniHelper::callStaticStringMethod(kAzoomeeActivityJavaClassName, "getDeviceLanguage");
#endif
	
	return languageCode;
}
    
void ConfigStorage::setIsDeviceIphoneX(bool isDeviceIphoneX)
{
    _isDeviceIphoneX = isDeviceIphoneX;
}
    
bool ConfigStorage::isDeviceIphoneX() const
{
    return _isDeviceIphoneX;
}
    
void ConfigStorage::setIsDevice18x9(bool isDevice18x9)
{
    _isDevice18x9 = isDevice18x9;
}
    
bool ConfigStorage::isDevice18x9() const
{
    return _isDevice18x9;
}

void ConfigStorage::setIsDevicePhone(bool isPhone)
{
	_isDevicePhone = isPhone;
}
	
bool ConfigStorage::isDevicePhone() const
{
	return _isDevicePhone;
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
    
//----------------------- Anonymous IP configuration -------------------------------------------
void ConfigStorage::setClientAnonymousIp(const std::string& publicIp)
{
    std::vector<std::string> ipElementsVector = splitStringToVector(publicIp, ".");
    if(ipElementsVector.size() != 4)
    {
        _clientIp = "0.0.0.0";
        return;
    }
    
    _clientIp = "";
    ipElementsVector[ipElementsVector.size() - 1] = "0";
    
    for(const std::string &currentElement : ipElementsVector)
    {
        if(_clientIp.length() != 0)
        {
            _clientIp += ".";
        }
        
        _clientIp += currentElement;
    }
}
    
std::string ConfigStorage::getClientAnonymousIp() const
{
    return _clientIp.empty() ? "0.0.0.0" : _clientIp;
}
    
}
