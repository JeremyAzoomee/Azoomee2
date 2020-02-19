#include "AnalyticsSingleton.h"
#include "../Utils/LocaleManager.h"
#include "../Input/TextInputChecker.h"
#include "../Utils/StringFunctions.h"
#include "../Utils/LocaleManager.h"
#include "../Utils/SessionIdManager.h"
#include "../Crashlytics/CrashlyticsConfig.h"
#include "../JWTSigner/HMACSHA256/HMACSHA256.h"
#include "../Data/HQDataObject/ContentItemManager.h"
#include "../Data/AppConfig.h"

NS_TZ_BEGIN

static AnalyticsSingleton *_sharedAnalyticsSingleton = NULL;

AnalyticsSingleton* AnalyticsSingleton::getInstance()
{
    if (! _sharedAnalyticsSingleton)
    {
        _sharedAnalyticsSingleton = new AnalyticsSingleton();
        _sharedAnalyticsSingleton->init();
    }
    
    return _sharedAnalyticsSingleton;
}

AnalyticsSingleton::~AnalyticsSingleton(void)
{
}

bool AnalyticsSingleton::init(void)
{
    _analyticsProperties = AnalyticsProperties::getInstance();
    return true;
}
    
void AnalyticsSingleton::mixPanelSendEventWithStoredProperties(const std::string& eventID)
{
    AnalyticsProperties::getInstance()->updateEpochTime();
    mixPanelSendEventNative(eventID, _analyticsProperties->getStoredGeneralProperties());
}

void AnalyticsSingleton::mixPanelSendEventWithStoredProperties(const std::string& eventID, const std::map<std::string, std::string>& map)
{
    AnalyticsProperties::getInstance()->updateEpochTime();
    std::map<std::string, std::string> mapToBeSent;
    mapToBeSent.insert(map.begin(), map.end());
    const auto& generalProperties = _analyticsProperties->getStoredGeneralProperties();
    mapToBeSent.insert(generalProperties.begin(), generalProperties.end());
    
    mixPanelSendEventNative(eventID, mapToBeSent);
}
  
void AnalyticsSingleton::mixPanelRegisterSuperProperties(const std::string& key, const std::string& property)
{
    _analyticsProperties->addGeneralProperty(key, property);
}

//-------------- SUPER PROPERTIES---------------------

void AnalyticsSingleton::registerAppVersion()
{
    mixPanelRegisterSuperProperties("appVersion",AppConfig::getInstance()->getVersionNumber());
}
    
void AnalyticsSingleton::registerAnonymousIp(const std::string& anonymousIp)
{
    mixPanelRegisterSuperProperties("ip", anonymousIp);
}

void AnalyticsSingleton::registerIdentifier(const std::string &parentId)
{
    const std::string& parentIdHash = HMACSHA256::getInstance()->getHMACSHA256Hash(parentId, parentId);
    mixPanelRegisterSuperProperties("parentID", parentIdHash);
    mixPanelRegisterIdentity(parentIdHash,_analyticsProperties->getStoredGeneralProperties());
}
    
void AnalyticsSingleton::registerNoOfChildren(int noOfChildren)
{
    mixPanelRegisterSuperProperties("noOfChildren",cocos2d::StringUtils::format("%s%d",NUMBER_IDENTIFIER, noOfChildren));
}

void AnalyticsSingleton::registerAzoomeeEmail(std::string emailAddress)
{
    std::string azoomeEmail = "NO";
    
    if (TextInputChecker::hasAzoomeeEmailAddress(emailAddress))
        azoomeEmail = "YES";
    
    mixPanelRegisterSuperProperties("azoomeeEmail",azoomeEmail);
	setCrashlyticsKeyWithString(CrashlyticsConsts::kAzoomeeEmailKey, azoomeEmail);
}

void AnalyticsSingleton::registerAccountStatus(std::string Status)
{
    mixPanelRegisterSuperProperties("accountStatus",Status);
}

void AnalyticsSingleton::registerBillingStatus(std::string Status)
{
    mixPanelRegisterSuperProperties("billingStatus",Status);
}

void AnalyticsSingleton::registerIAPOS(std::string OS_String)
{
    _analyticsProperties->setOSManufacturer(OS_String);
    mixPanelRegisterSuperProperties("iAP_OS",OS_String);
}

void AnalyticsSingleton::registerBillingProvider(std::string provider)
{
    mixPanelRegisterSuperProperties("billingProvider",provider);
}

void AnalyticsSingleton::registerChildGenderAndAge(const ChildRef& child)
{
	if(child)
	{
    	mixPanelRegisterSuperProperties("sex",child->getSex());
    
    	int childAge = TextInputChecker::ageFromDOBString(child->getDOB());
    
    	mixPanelRegisterSuperProperties("age",cocos2d::StringUtils::format("%s%d",NUMBER_IDENTIFIER, childAge));
	}
}
    
void AnalyticsSingleton::registerSessionId(std::string sessionId)
{
    mixPanelRegisterSuperProperties("sessionId", sessionId);
	setCrashlyticsKeyWithString(CrashlyticsConsts::kSessionIdKey, sessionId);
}
    
void AnalyticsSingleton::registerCurrentScene(const std::string &currentScene)
{
    //before registering the new scene as current, reading out the previous one to be able to send previousScene property for mixpanel moveToSceneEvent.
    const std::map<std::string, std::string> &mixPanelProperties = _analyticsProperties->getStoredGeneralProperties();
    std::string previousScene = "NA";
    
    if(mixPanelProperties.find("currentScene") != mixPanelProperties.end())
    {
        previousScene = mixPanelProperties.at("currentScene");
    }
    
    mixPanelRegisterSuperProperties("currentScene", currentScene);
	setCrashlyticsKeyWithString(CrashlyticsConsts::kCurrentSceneKey, currentScene);
    
    moveToSceneEvent(previousScene);
}
    
void AnalyticsSingleton::setPortraitOrientation()
{
    mixPanelRegisterSuperProperties("orientation", "portrait");
}
    
void AnalyticsSingleton::setLandscapeOrientation()
{
    mixPanelRegisterSuperProperties("orientation", "landscape");
}
    
void AnalyticsSingleton::setNumberOfChatFriends(int noOfChatFriends)
{
    mixPanelRegisterSuperProperties("noOfChatFriends",cocos2d::StringUtils::format("%s%d",NUMBER_IDENTIFIER, noOfChatFriends));
}

void AnalyticsSingleton::setChatFriendIsParent(bool chatFriendIsParent)
{
    std::string friendIsParentString = "NO";
    
    if (chatFriendIsParent)
        friendIsParentString = "YES";
    
    mixPanelRegisterSuperProperties("chatFriendIsParent",friendIsParentString);
}
    
void AnalyticsSingleton::setIsUserAnonymous(bool isUserAnonymous)
{
    const std::string& isUserAnonymousString = isUserAnonymous ? "YES" : "NO";
    mixPanelRegisterSuperProperties("isUserAnonymous", isUserAnonymousString);
}

void AnalyticsSingleton::registerAlias(const std::string& newId)
{
	const std::string& newIdHash = HMACSHA256::getInstance()->getHMACSHA256Hash(newId, newId);
	mixPanelRegisterSuperProperties("parentID", newIdHash);
	mixPanelRegisterAlias(newIdHash);
	mixPanelUpdatePeopleProfileData(_analyticsProperties->getStoredGeneralProperties());
}

void AnalyticsSingleton::registerBillingData(const BillingDataRef& billingData)
{
	mixPanelRegisterSuperProperties("billingProvider",billingData->getPaymentProvider());
	mixPanelRegisterSuperProperties("billingStatus",billingData->getBillingStatusStr());
	mixPanelRegisterSuperProperties("billingRenewDate", billingData->getNextBillDate());
	mixPanelRegisterSuperProperties("billingVoucherCode", billingData->getVoucherCode());
	mixPanelRegisterSuperProperties("billingVoucherCampaign", billingData->getCampaign());
	mixPanelRegisterSuperProperties("billingVoucherOrganisation", billingData->getOrganisation());
	mixPanelRegisterSuperProperties("billingSubscribedDuration", cocos2d::StringUtils::format("%d",billingData->getDuration()));
	mixPanelUpdatePeopleProfileData(_analyticsProperties->getStoredGeneralProperties());
}

void AnalyticsSingleton::registerLanguageCode(const std::string& languageCode)
{
	mixPanelRegisterSuperProperties("language", languageCode);
}

//-------------logout events-----------------

void AnalyticsSingleton::logoutChildEvent()
{
    mixPanelRegisterSuperProperties("sex","");
    mixPanelRegisterSuperProperties("age","");
}

void AnalyticsSingleton::logoutParentEvent()
{
    logoutChildEvent();
    mixPanelRegisterSuperProperties("accountStatus","");
    mixPanelRegisterSuperProperties("azoomeeEmail","");
    mixPanelRegisterSuperProperties("billingProvider","");
    mixPanelRegisterSuperProperties("billingStatus","");
}

//-------------Startup--------------------

void AnalyticsSingleton::firstLaunchEvent()
{
    if(!(cocos2d::UserDefault::getInstance()->getBoolForKey("firstTimeLaunchEventSent", false) || cocos2d::UserDefault::getInstance()->getBoolForKey("FirstSlideShowSeen", false)))
    {
        cocos2d::UserDefault::getInstance()->setBoolForKey("firstTimeLaunchEventSent", true);
        mixPanelSendEventWithStoredProperties("firstLaunch");
        appsFlyerSendEvent("firstLaunch");
    }
}

// -------------- SIGN IN FUNCTIONS -----------------
void AnalyticsSingleton::signInSuccessEvent()
{
    mixPanelSendEventWithStoredProperties("parentAppLoginSuccess");
}

void AnalyticsSingleton::signInFailEvent(int errorCode)
{
    mixPanelSendEventWithStoredProperties("parentAppLoginFailure");
}

//-------------ONBOARDING--------------------
void AnalyticsSingleton::OnboardingStartEvent()
{
    mixPanelSendEventWithStoredProperties("startCreateAccount");
}

void AnalyticsSingleton::OnboardingAccountCreatedEvent()
{
    std::map<std::string, std::string> mixPanelProperties;
    mixPanelProperties["Method"] = "App";
    
    mixPanelSendEventWithStoredProperties("accountCreated", mixPanelProperties);
    appsFlyerSendEvent("accountCreated", mixPanelProperties);
}

void AnalyticsSingleton::OnboardingAccountCreatedErrorEvent(long errorCode)
{
    std::map<std::string, std::string> mixPanelProperties;
    mixPanelProperties["ErrorCode"] = cocos2d::StringUtils::format("%ld", errorCode);
    
    mixPanelSendEventWithStoredProperties("accountCreatedError", mixPanelProperties);
}

//-------------CHILD PROFILE CREATION-------------
void AnalyticsSingleton::childProfileStartEvent()
{
    mixPanelSendEventWithStoredProperties("childProfileCreateStart");
}

void AnalyticsSingleton::childProdileNameErrorEvent()
{
    mixPanelSendEventWithStoredProperties("childProfileNameError");
}

void AnalyticsSingleton::childProfileDOBErrorEvent()
{
    //There are no errors defined at present for this function.
    //mixPanelSendEvent("childProfileDOBError");
}

void AnalyticsSingleton::childProfileCreatedSuccessEvent()
{
    std::map<std::string, std::string> mixPanelProperties;
    mixPanelProperties["Method"] = "App";
    
    mixPanelSendEventWithStoredProperties("childProfileCreatedSuccess", mixPanelProperties);
}

void AnalyticsSingleton::childProfileCreatedErrorEvent(long errorCode)
{
    std::map<std::string, std::string> mixPanelProperties;
    mixPanelProperties["ErrorCode"] = cocos2d::StringUtils::format("%ld", errorCode);
    
    mixPanelSendEventWithStoredProperties("childProfileCreatedError", mixPanelProperties);
}
    
void AnalyticsSingleton::childProfileUpdateErrorEvent(long errorCode)
{
    std::map<std::string, std::string> mixPanelProperties;
    mixPanelProperties["ErrorCode"] = cocos2d::StringUtils::format("%ld", errorCode);
    
    mixPanelSendEventWithStoredProperties("childProfileUpdateError", mixPanelProperties);
}
    
void AnalyticsSingleton::childProfileCreatedEvent(int age)
{
    std::map<std::string, std::string> mixPanelProperties;
    mixPanelProperties["Method"] = "App";
    mixPanelProperties["Age"] = age;
    
    mixPanelSendEventWithStoredProperties("childProfileCreatedEvent", mixPanelProperties);
}

void AnalyticsSingleton::createChildFlowEvent(const std::string& flowState)
{
	mixPanelSendEventWithStoredProperties("CreateChild_" + flowState);
}

void AnalyticsSingleton::createChildNextPressed()
{
	mixPanelSendEventWithStoredProperties("CreateChild_NextPressed");
}

void AnalyticsSingleton::createChildBackPressed()
{
	mixPanelSendEventWithStoredProperties("CreateChild_BackPressed");
}

//-------------HUB ACTIONS-------------------

void AnalyticsSingleton::navSelectionEvent(const std::string& buttonName)
{
    std::map<std::string, std::string> mixPanelProperties;
    mixPanelProperties["Type"] = buttonName;
    
    mixPanelSendEventWithStoredProperties("contentNavSelection", mixPanelProperties);
}

void AnalyticsSingleton::categoryOpenEvent(bool isOpen, const std::string& categoryName)
{
    const std::string& eventName = isOpen ? "categoryOpen" : "categoryClose";
    mixPanelSendEventWithStoredProperties(eventName, { { "Category Name", categoryName } });
}

//---------------- CONTENT ITEM ACTIONS------------------------
    
void AnalyticsSingleton::contentItemSelectedEvent(const std::string& Type)
{
    MutableHQContentItemObjectRef contentItem = MutableHQContentItemObject::create();
    contentItem->setType(Type);
    contentItemSelectedEvent(contentItem, -1, -1, "");
}
    
void AnalyticsSingleton::contentItemSelectedEvent(const std::string& Type, const std::string& elementShape)
{
    MutableHQContentItemObjectRef contentItem = MutableHQContentItemObject::create();
    contentItem->setType(Type);
    contentItemSelectedEvent(contentItem, -1, -1, "");
}
    
void AnalyticsSingleton::contentItemSelectedEvent(const HQContentItemObjectRef &contentItem, int rowNumber, int elementNumber, const std::string& location)
{
    SessionIdManager::getInstance()->resetBackgroundTimeInContent();
    
    //Set here for Chat to count how long in a conversation
    //for other content this is reset when contentItemWebviewStartedEvent() is called
    _analyticsProperties->setTimeOpenedContent(time(NULL));

    std::map<std::string, std::string> mixPanelProperties;
    mixPanelProperties["Title"] = contentItem->getTitle();
    mixPanelProperties["Description"] = contentItem->getDescription();
    mixPanelProperties["Type"] = contentItem->getType();
    mixPanelProperties["ContentID"] = contentItem->getContentItemId();
    mixPanelProperties["rowNumber"] = cocos2d::StringUtils::format("%d", rowNumber);
    mixPanelProperties["elementNumber"] = cocos2d::StringUtils::format("%d", elementNumber);
    mixPanelProperties["tileLocation"] = location;
    
    // is this piece of content part of a group? If so, set the series title property
    const HQContentItemObjectRef& groupContentItem = ContentItemManager::getInstance()->getParentOfContentItemForId(contentItem->getContentItemId());
    if(groupContentItem)
    {
        mixPanelProperties["SeriesTitle"] = groupContentItem->getTitle();
    }
    
    _analyticsProperties->setStoredContentItemProperties(mixPanelProperties);
    
    mixPanelSendEventWithStoredProperties("contentItemSelectedEvent", mixPanelProperties);
}

void AnalyticsSingleton::contentItemSelectedOutsideCarouselEvent(const HQContentItemObjectRef &contentItem)
{
    contentItemSelectedEvent(contentItem, -1, -1, "");
}
    
void AnalyticsSingleton::updateContentItemDetails(const HQContentItemObjectRef &contentItem)
{
    std::map<std::string, std::string> mixPanelProperties;
    mixPanelProperties["Title"] = contentItem->getTitle();
    mixPanelProperties["Description"] = contentItem->getDescription();
    mixPanelProperties["Type"] = contentItem->getType();
    mixPanelProperties["ContentID"] = contentItem->getContentItemId();
    mixPanelProperties["elementNumber"] = contentItem->getElementNumber();
    
    mixPanelProperties["rowNumber"] = _analyticsProperties->getStoredContentItemProperties()["rowNumber"];
    
    _analyticsProperties->setStoredContentItemProperties(mixPanelProperties);
}
    
void AnalyticsSingleton::contentItemProcessingStartedEvent()
{
    mixPanelSendEventWithStoredProperties("contentItemProcessingStarted", _analyticsProperties->getStoredContentItemProperties());
}
    
void AnalyticsSingleton::contentItemProcessingErrorEvent()
{
    mixPanelSendEventWithStoredProperties("contentItemProcessingError", _analyticsProperties->getStoredContentItemProperties());
}
    
void AnalyticsSingleton::contentItemIncompatibleEvent()
{
    mixPanelSendEventWithStoredProperties("contentItemIncompatible", _analyticsProperties->getStoredContentItemProperties());
}
    
void AnalyticsSingleton::contentItemWebviewStartedEvent()
{
    _analyticsProperties->setTimeOpenedContent(time(NULL));
    mixPanelSendEventWithStoredProperties("contentItemWebviewStarted", _analyticsProperties->getStoredContentItemProperties());
}

void AnalyticsSingleton::contentItemClosedEvent()
{
    time_t now;
    time(&now);
    double secondsOpened = difftime(now,_analyticsProperties->getTimeOpenedContent());
    
    secondsOpened -= SessionIdManager::getInstance()->getBackgroundTimeInContent();
	
	
    _analyticsProperties->addPropertyToStoredContentItemProperties(AnalyticsProperties::kSecondsInContentKey, cocos2d::StringUtils::format("%s%.f",NUMBER_IDENTIFIER, secondsOpened));
    
    mixPanelSendEventWithStoredProperties("contentItemClosed", _analyticsProperties->getStoredContentItemProperties());
    
}
    
void AnalyticsSingleton::lockedContentItemInRecentlyPlayedEvent(const HQContentItemObjectRef &contentItem)
{
    const std::map<std::string, std::string>& mixPanelProperties = {
        {"ContentID", contentItem->getContentItemId()}
    };
    mixPanelSendEventWithStoredProperties("lockedContentInRecentlyPlayed", mixPanelProperties);
}

//------------- PREVIEW ACTIONS ---------------
void AnalyticsSingleton::previewContentClickedEvent(std::string Title, std::string Description, std::string Type)
{
    std::map<std::string, std::string> mixPanelProperties;
    mixPanelProperties["Title"] = Title;
    mixPanelProperties["Description"] = Description;
    mixPanelProperties["Type"] = Type;
    
    mixPanelSendEventWithStoredProperties("previewContentItemClicked", mixPanelProperties);
}

void AnalyticsSingleton::previewPopupCancelledEvent()
{
    mixPanelSendEventWithStoredProperties("previewEmailPopUpDismissed");
}

//---------------MEDIA ACTIONS -----------------
void AnalyticsSingleton::mediaPlayerQualityEvent(std::string quality)
{
    std::map<std::string, std::string> mixPanelProperties = _analyticsProperties->getStoredContentItemProperties();
    
    mixPanelProperties["Quality"] = quality;
    
    mixPanelSendEventWithStoredProperties("mediaQuality", mixPanelProperties);
}

void AnalyticsSingleton::mediaPlayerProgressEvent(int percentComplete)
{
    std::map<std::string, std::string> mixPanelProperties = _analyticsProperties->getStoredContentItemProperties();
    mixPanelProperties["Progress"] = cocos2d::StringUtils::format("%d", percentComplete);
    
    mixPanelSendEventWithStoredProperties("mediaProgress", mixPanelProperties);
}

void AnalyticsSingleton::mediaPlayerPausedEvent()
{
    mixPanelSendEventWithStoredProperties("mediaPause", _analyticsProperties->getStoredContentItemProperties());
}

void AnalyticsSingleton::mediaPlayerEndEvent(int SecondsMediaPlayed)
{
    std::map<std::string, std::string> mixPanelProperties = _analyticsProperties->getStoredContentItemProperties();
    
    mixPanelProperties["SecondsMediaPlayed"] = cocos2d::StringUtils::format("%s%d",NUMBER_IDENTIFIER, SecondsMediaPlayed);

    mixPanelSendEventWithStoredProperties("mediaEnd", mixPanelProperties);
}

void AnalyticsSingleton::mediaPlayerFirstFrameEvent(std::string loadTimeMS)
{
    std::map<std::string, std::string> mixPanelProperties = _analyticsProperties->getStoredContentItemProperties();
    mixPanelProperties["LoadTime"] = cocos2d::StringUtils::format("%s%s",NUMBER_IDENTIFIER, loadTimeMS.c_str());
    
    mixPanelSendEventWithStoredProperties("mediaFirstFrame", mixPanelProperties);
}
    
void AnalyticsSingleton::mediaPlayerNewPlaylistItemSetEvent(int itemNumber)
{
    std::map<std::string, std::string> mixPanelProperties = _analyticsProperties->getStoredContentItemProperties();
    mixPanelProperties["playlistElementNumber"] = cocos2d::StringUtils::format("%d", itemNumber);

    mixPanelSendEventWithStoredProperties("mediaNewPlaylistItem", mixPanelProperties);
}
    
void AnalyticsSingleton::mediaPlayerVideoPlayEvent()
{
    mixPanelSendEventWithStoredProperties("mediaVideoPlay", _analyticsProperties->getStoredContentItemProperties());
}
    
void AnalyticsSingleton::mediaPlayerVideoCompletedEvent()
{
    mixPanelSendEventWithStoredProperties("mediaVideoCompleted", _analyticsProperties->getStoredContentItemProperties());
}
    
void AnalyticsSingleton::mediaPlayerPlaylistCompletedEvent()
{
    mixPanelSendEventWithStoredProperties("mediaPlaylistCompleted", _analyticsProperties->getStoredContentItemProperties());
}

//---------------OTHER ACTION------------------
void AnalyticsSingleton::genericButtonPressEvent(std::string buttonName)
{
    std::map<std::string, std::string> mixPanelProperties;
    mixPanelProperties["ButtonName"] = buttonName;
    
    mixPanelSendEventWithStoredProperties("tapButton", mixPanelProperties);
}

void AnalyticsSingleton::messageBoxShowEvent(std::string messageTitle, long errorCode)
{
    std::map<std::string, std::string> mixPanelProperties;
    mixPanelProperties["MessageTitle"] = messageTitle;
    mixPanelProperties["ErrorCode"] = cocos2d::StringUtils::format("%ld", errorCode);
    
    mixPanelSendEventWithStoredProperties("messageBoxDisplayed", mixPanelProperties);
}

void AnalyticsSingleton::localisedStringErrorEvent(std::string stringRequested, std::string languageUsed)
{
    std::map<std::string, std::string> mixPanelProperties;
    mixPanelProperties["StringRequestFor"] = stringRequested;
    mixPanelProperties["WithLanguage"] = languageUsed;
    
    mixPanelSendEventWithStoredProperties("localisedStringError", mixPanelProperties);
}

void AnalyticsSingleton::introVideoTimedOutError(std::string errorMessage)
{
    std::map<std::string, std::string> mixPanelProperties;
    mixPanelProperties["ErrorMessage"] = errorMessage;
    
    mixPanelSendEventWithStoredProperties("introVideoTimedOutError", mixPanelProperties);
}
    
void AnalyticsSingleton::enteredBackgroundEvent()
{
    mixPanelSendEventWithStoredProperties("enteredBackgroundEvent");
}
void AnalyticsSingleton::enteredForegroundEvent()
{
    mixPanelSendEventWithStoredProperties("enteredForegroundEvent");
}
    
void AnalyticsSingleton::sessionIdHasChanged(std::string oldSessionId)
{
    std::map<std::string, std::string> mixPanelProperties;
    mixPanelProperties["oldSessionId"] = oldSessionId;
    
    mixPanelSendEventWithStoredProperties("sessionIdHasChanged", mixPanelProperties);
}
    
void AnalyticsSingleton::httpRequestFailed(std::string requestTag, long responseCode, std::string qid)
{
    std::map<std::string, std::string> mixPanelProperties;
    mixPanelProperties["requestTag"] = requestTag;
    mixPanelProperties["responseCode"] = cocos2d::StringUtils::format("%ld", responseCode);;
    mixPanelProperties["qid"] = qid;
    
    mixPanelSendEventWithStoredProperties("httpRequestFailed", mixPanelProperties);
}
    
void AnalyticsSingleton::moveToSceneEvent(const std::string &previousScene)
{
    std::map<std::string, std::string> mixPanelProperties;
    mixPanelProperties["previousScene"] = previousScene;
    
    mixPanelSendEventWithStoredProperties("moveToSceneEvent", mixPanelProperties);
}
    
void AnalyticsSingleton::settingsOnlineSafetySlideChangeEvent(int NewSlideNumber)
{
    std::map<std::string, std::string> mixPanelProperties;
    mixPanelProperties["NewSlideNumber"] = cocos2d::StringUtils::format("%d", NewSlideNumber);
    
    mixPanelSendEventWithStoredProperties("settingsOnlineSafetySlideChange", mixPanelProperties);
}

//---------------IAP ACTIONS------------------
  
void AnalyticsSingleton::displayIAPUpsaleEvent(std::string fromLocation)
{
    std::map<std::string, std::string> mixPanelProperties;
    mixPanelProperties["DisplayedFrom"] = fromLocation;
    
    mixPanelSendEventWithStoredProperties("displayIAPUpsale", mixPanelProperties);
}

void AnalyticsSingleton::iapSubscriptionSuccessEvent()
{
    mixPanelSendEventWithStoredProperties("iapSubscriptionSuccess");
    
    std::map<std::string, std::string> mixPanelProperties;
    mixPanelProperties["iAP_OS"] = _analyticsProperties->getOSManufacturer();
    
    appsFlyerSendEvent("iapSubscriptionSuccess", mixPanelProperties);
}

void AnalyticsSingleton::iapSubscriptionErrorEvent(std::string errorDescription)
{
    std::map<std::string, std::string> mixPanelProperties;
    mixPanelProperties["ErrorType"] = errorDescription;
    
    mixPanelSendEventWithStoredProperties("iapSubscriptionError", mixPanelProperties);
}

void AnalyticsSingleton::iapSubscriptionFailedEvent(const std::string& failureDetails)
{
    std::map<std::string, std::string> mixPanelProperties = { {"FailureDetails", failureDetails} };
    mixPanelSendEventWithStoredProperties("iapSubscriptionFailed", mixPanelProperties);
}

void AnalyticsSingleton::iapUserDataFailedEvent()
{
    mixPanelSendEventWithStoredProperties("iapUserDataFailed");
}

void AnalyticsSingleton::iapSubscriptionDoublePurchaseEvent()
{
    mixPanelSendEventWithStoredProperties("iapSubscriptionDoublePurchase");
}

void AnalyticsSingleton::iapBackEndRequestFailedEvent(long errorCode)
{
    std::map<std::string, std::string> mixPanelProperties;
    mixPanelProperties["ErrorCode"] = cocos2d::StringUtils::format("%ld", errorCode);
    
    mixPanelSendEventWithStoredProperties("iapBackendRequestFailedError", mixPanelProperties);
}

void AnalyticsSingleton::iapAppleAutoRenewSubscriptionEvent()
{
    mixPanelSendEventWithStoredProperties("iapAppleAutoRenewSubscription");
}
    
//---------------DEEPLINKING ACTIONS------------------
void AnalyticsSingleton::deepLinkingDetailsSetEvent()
{
    mixPanelSendEventWithStoredProperties("deepLinkingDetailsSet");
}
    
void AnalyticsSingleton::deepLinkingMoveToEvent(std::string moveTo)
{
    std::map<std::string, std::string> mixPanelProperties;
    mixPanelProperties["MoveTo"] = moveTo;
    
    mixPanelSendEventWithStoredProperties("deepLinkingMoveToEvent", mixPanelProperties);
}
    
void AnalyticsSingleton::deepLinkingContentEvent()
{
    mixPanelSendEventWithStoredProperties("deepLinkingContentEvent");
}
    
    //-----------------CHAT INVITATION EVENTS------------------
    void AnalyticsSingleton::settingsConfirmationTabNotificationShown()
    {
        mixPanelSendEventWithStoredProperties("settingsConfirmationTabNotificationShown");
    }
    
    void AnalyticsSingleton::settingsConfirmationTabNotificationError(long errorCode)
    {
        std::map<std::string, std::string> mixPanelProperties;
        mixPanelProperties["ErrorCode"] = cocos2d::StringUtils::format("%ld", errorCode);
        
        mixPanelSendEventWithStoredProperties("settingsConfirmationTabNotificationError", mixPanelProperties);
    }
    
    void AnalyticsSingleton::settingsKidInviteRequestSuccess()
    {
        mixPanelSendEventWithStoredProperties("settingsKidInviteRequestSuccess");
    }
    
    void AnalyticsSingleton::settingsKidInviteRequestError(long errorCode)
    {
        std::map<std::string, std::string> mixPanelProperties;
        mixPanelProperties["ErrorCode"] = cocos2d::StringUtils::format("%ld", errorCode);
        
        mixPanelSendEventWithStoredProperties("settingsKidInviteRequestError", mixPanelProperties);
    }
    
    void AnalyticsSingleton::settingsPendingFriendRequestsRefreshError(long errorCode)
    {
        std::map<std::string, std::string> mixPanelProperties;
        mixPanelProperties["ErrorCode"] = cocos2d::StringUtils::format("%ld", errorCode);
        
        mixPanelSendEventWithStoredProperties("settingsPendingFriendRequestsRefreshError", mixPanelProperties);
    }

    void AnalyticsSingleton::settingsConfirmationRejectedSuccess()
    {
        mixPanelSendEventWithStoredProperties("settingsConfirmationRejectedSuccess");
    }
    
    void AnalyticsSingleton::settingsConfirmationApprovedSuccess()
    {
        mixPanelSendEventWithStoredProperties("settingsConfirmationApprovedSuccess");
    }
    
    void AnalyticsSingleton::settingsConfirmationError(long errorCode)
    {
        std::map<std::string, std::string> mixPanelProperties;
        mixPanelProperties["ErrorCode"] = cocos2d::StringUtils::format("%ld", errorCode);
        
        mixPanelSendEventWithStoredProperties("settingsConfirmationError", mixPanelProperties);
    }
    
    void AnalyticsSingleton::chatKeyboardEvent(bool isOnScreen)
    {
        std::string isOnScreenString = "NO";
        
        if (isOnScreen)
            isOnScreenString = "YES";

        std::map<std::string, std::string> mixPanelProperties;
        mixPanelProperties["keyboardOnScreen"] = isOnScreenString;
        
        mixPanelSendEventWithStoredProperties("chatKeyboardOnScreenEvent", mixPanelProperties);
    }
    
    void AnalyticsSingleton::chatSelectedSticker(std::string stickerURL)
    {
        std::map<std::string, std::string> mixPanelProperties;
        mixPanelProperties["stickerURL"] = stickerURL;
        
        mixPanelSendEventWithStoredProperties("chatSelectedSticker", mixPanelProperties);
    }
    
    void AnalyticsSingleton::chatSelectedStickerTab(std::string categoryID)
    {
        std::map<std::string, std::string> mixPanelProperties;
        mixPanelProperties["stickerCategoryID"] = categoryID;
        
        mixPanelSendEventWithStoredProperties("chatSelectedStickerTab", mixPanelProperties);
    }
    
    void AnalyticsSingleton::chatIncomingMessageEvent(std::string messageType)
    {
        std::map<std::string, std::string> mixPanelProperties;
        mixPanelProperties["messageType"] = messageType;
        
        mixPanelSendEventWithStoredProperties("chatIncomingMessageEvent", mixPanelProperties);
    }
    
    void AnalyticsSingleton::chatOutgoingMessageEvent(std::string messageType)
    {
        std::map<std::string, std::string> mixPanelProperties;
        mixPanelProperties["messageType"] = messageType;
        
        mixPanelSendEventWithStoredProperties("chatOutgoingMessageEvent", mixPanelProperties);
    }
    
    void AnalyticsSingleton::unreadMessagesNotificationReceived()
    {
        mixPanelSendEventWithStoredProperties("unreadMessagesNotificationReceived");
    }
    
    void AnalyticsSingleton::chatReportedEvent()
    {
        mixPanelSendEventWithStoredProperties("chatReportedEvent");
    }
    
    void AnalyticsSingleton::chatResetReportedEvent()
    {
        mixPanelSendEventWithStoredProperties("chatResetReportedEvent");
    }

void AnalyticsSingleton::chatOpenSharedContentEvent(const std::string& contentId)
{
    mixPanelSendEventWithStoredProperties("chatOpenSharedContentEvent", {{"contentId", contentId}});
}

//-------------------------------------CTA ACTIONS-----------------------------
    
void AnalyticsSingleton::ctaButtonPressed(const std::string &buttonId, const std::string &title)
{
    const std::map<std::string, std::string> &mixPanelProperties = {
        { "buttonId", buttonId },
        { "groupId", _analyticsProperties->getCtaWindowGroupId() },
        {"nodeId", _analyticsProperties->getCtaWindowNodeId()},
        {"recommendedTitle",title}
    };
    mixPanelSendEventWithStoredProperties("ctaContentButtonPressed", mixPanelProperties);
}
    
void AnalyticsSingleton::ctaWindowAppeared(const std::string &groupId, const std::string &nodeId)
{
    _analyticsProperties->setCtaWindowNodeId(nodeId);
    _analyticsProperties->setCtaWindowGroupId(groupId);
    
    const std::map<std::string, std::string> &mixPanelProperties = {
        { "groupId", groupId },
        {"nodeId", nodeId},
        {"sourceButton", _analyticsProperties->getCtaSourceButton()},
        {"sourceContentId", _analyticsProperties->getCtaSourceContentId()},
        {"sourceMediaType", _analyticsProperties->getCtaSourceMediaType()},
        {"recommendedContentId", _analyticsProperties->getCtaRecommendedContentId()}
    };
    
    mixPanelSendEventWithStoredProperties("ctaWindowAppeared", mixPanelProperties);
}
    
void AnalyticsSingleton::registerCTASource(const std::string& buttonId, const std::string& contentId, const std::string& mediaType, const std::string& recommendedContentId)
{
    _analyticsProperties->setCtaSourceButton(buttonId);
    _analyticsProperties->setCtaSourceContentId(contentId);
    _analyticsProperties->setCtaSourceMediaType(mediaType);
    _analyticsProperties->setCtaRecommendedContentId(recommendedContentId);
}

//--------------ME HQ Events--------------------------------

void AnalyticsSingleton::favouriteContentItem(const std::string& contentItemId)
{
    mixPanelSendEventWithStoredProperties("contentItemFavourited", {{"favItemId",contentItemId}});
}
void AnalyticsSingleton::unFavouriteContentItem(const std::string& contentItemId)
{
    mixPanelSendEventWithStoredProperties("contentItemUnfavourited", {{"unfavItemId",contentItemId}});
}
void AnalyticsSingleton::shareContentItemButtonPressed(const std::string& contentItemId)
{
    mixPanelSendEventWithStoredProperties("shareContentItemButtonPressed", {{"sharedItemId",contentItemId}});
}

//-------------OomeeMaker Events----------------------------

void AnalyticsSingleton::makeAvatarSuccess(const std::string& origin)
{
    mixPanelSendEventWithStoredProperties("makeAvatarSuccess", {{"origin",origin}});
}
void AnalyticsSingleton::shareOomee()
{
    mixPanelSendEventWithStoredProperties("shareOomee");
}
void AnalyticsSingleton::editOomee()
{
    mixPanelSendEventWithStoredProperties("editOomee");
}
void AnalyticsSingleton::deleteOomee()
{
    mixPanelSendEventWithStoredProperties("deleteOomee");
}
void AnalyticsSingleton::newOomee()
{
    mixPanelSendEventWithStoredProperties("newOomee");
}
void AnalyticsSingleton::saveOomee(const std::string& oomeeDataString)
{
    mixPanelSendEventWithStoredProperties("saveOomee",{{"oomeeData",oomeeDataString}});
}
void AnalyticsSingleton::reportNumberOfOomees(int numOomees)
{
    mixPanelSendEventWithStoredProperties("reportNumOomeesEvent",{{"numOomees", cocos2d::StringUtils::format("%d", numOomees)}});
}

void AnalyticsSingleton::accessorySelectedEvent(const std::string& method)
{
    mixPanelSendEventWithStoredProperties("accessorySelected" ,{{"method", method}});
}

//----------------Art app events------------------------------

void AnalyticsSingleton::stickerSelectedEvent(const std::string &stickerFilename)
{
    auto splitString = StringFunctions::splitStringToVector(stickerFilename, "/");
    if(splitString.size() >= 2)
    {
        mixPanelSendEventWithStoredProperties("artAppStickerSelected",{{"stickerCategory",splitString.at(splitString.size() - 2)},{"stickerName",splitString.at(splitString.size() - 1)}} );
    }
}

//---------------I18n events----------------------------------
void AnalyticsSingleton::languageChangedEvent(const std::string& languageCode)
{
	mixPanelSendEventWithStoredProperties("languageChanged" ,{{"languageCode", languageCode}});
}

//-------------Vodacom events-------------------------------
void AnalyticsSingleton::vodacomOnboardingFlowStartedEvent()
{
	mixPanelSendEventWithStoredProperties("vodacomOnboardingFlowStarted");
}

void AnalyticsSingleton::vodacomOnboardingFlowExitEvent()
{
	mixPanelSendEventWithStoredProperties("vodacomOnboardingFlowFinished");
}
void AnalyticsSingleton::vodacomOnboardingFlowMoveToScreen(const std::string& newScreen)
{
	mixPanelSendEventWithStoredProperties("vodacomOnboardingMoveToScreen" ,{{"newScreen", newScreen}});
}
void AnalyticsSingleton::vodacomOnboardingVoucherAdded(const std::string& voucherCode)
{
	mixPanelSendEventWithStoredProperties("vodacomOnboardingVoucherAdded" ,{{"voucherCode", voucherCode}});
}

//-------------Rewards events-------------------------------
void AnalyticsSingleton::rewardRedeemedEvent(int value)
{
	mixPanelSendEventWithStoredProperties("RewardRedeemed" ,{{"rewardValue", cocos2d::StringUtils::format("%d",value)}});
}
void AnalyticsSingleton::rewardAnimBeginEvent(int value)
{
	mixPanelSendEventWithStoredProperties("RewardAnimationBegin" ,{{"rewardValue", cocos2d::StringUtils::format("%d",value)}});
}
void AnalyticsSingleton::rewardAnimCloseEvent(int value)
{
	mixPanelSendEventWithStoredProperties("RewardAnimationClose" ,{{"rewardValue", cocos2d::StringUtils::format("%d",value)}});
}
void AnalyticsSingleton::rewardAnimGoToShopEvent(int value)
{
	mixPanelSendEventWithStoredProperties("RewardAnimationGoToShop" ,{{"rewardValue", cocos2d::StringUtils::format("%d",value)}});
}

//-------------Shop events----------------------------------
void AnalyticsSingleton::shopPageTurned(int pageNumber)
{
	mixPanelSendEventWithStoredProperties("ShopPageTurned" ,{{"pageNumber", cocos2d::StringUtils::format("%d",pageNumber)}});
}

void AnalyticsSingleton::shopItemPressed(int itemPos, const ShopDisplayItemRef& item)
{
	const std::vector<std::string>& tags = item->getTags();
	mixPanelSendEventWithStoredProperties("ShopItemPressed" ,{
		{"itemPos", cocos2d::StringUtils::format("%d",itemPos)},
		{"itemName", item->getInventoryItem()->getName()},
		{"itemPrice", cocos2d::StringUtils::format("%d",item->getPrice())},
		{"itemShape", item->getShape()},
		{"isNew", std::find(tags.begin(), tags.end(), "NEW") != tags.end() ?  "YES" : "NO"},
		{"isFeatured", std::find(tags.begin(), tags.end(), "FEATURED") != tags.end() ?  "YES" : "NO"}
	});
}

void AnalyticsSingleton::shopLockedItemPressed(int itemPos, const ShopDisplayItemRef& item)
{
	const std::vector<std::string>& tags = item->getTags();
	mixPanelSendEventWithStoredProperties("ShopLockedItemPressed" ,{
		{"itemPos", cocos2d::StringUtils::format("%d",itemPos)},
		{"itemName", item->getInventoryItem()->getName()},
		{"itemPrice", cocos2d::StringUtils::format("%d",item->getPrice())},
		{"itemShape", item->getShape()},
		{"isNew", std::find(tags.begin(), tags.end(), "NEW") != tags.end() ?  "YES" : "NO"},
		{"isFeatured", std::find(tags.begin(), tags.end(), "FEATURED") != tags.end() ?  "YES" : "NO"}
	});
}

void AnalyticsSingleton::shopUnaffordableItemPressed(int itemPos, const ShopDisplayItemRef& item)
{
	const std::vector<std::string>& tags = item->getTags();
	mixPanelSendEventWithStoredProperties("ShopUnaffordableItemPressed" ,{
		{"itemPos", cocos2d::StringUtils::format("%d",itemPos)},
		{"itemName", item->getInventoryItem()->getName()},
		{"itemPrice", cocos2d::StringUtils::format("%d",item->getPrice())},
		{"itemShape", item->getShape()},
		{"isNew", std::find(tags.begin(), tags.end(), "NEW") != tags.end() ?  "YES" : "NO"},
		{"isFeatured", std::find(tags.begin(), tags.end(), "FEATURED") != tags.end() ?  "YES" : "NO"}
	});
}

void AnalyticsSingleton::shopItemPurchased(const ShopDisplayItemRef& item)
{
	const std::vector<std::string>& tags = item->getTags();
	mixPanelSendEventWithStoredProperties("ShopItemPurchased" ,{
		{"itemName", item->getInventoryItem()->getName()},
		{"itemPrice", cocos2d::StringUtils::format("%d",item->getPrice())},
		{"itemShape", item->getShape()},
		{"isNew", std::find(tags.begin(), tags.end(), "NEW") != tags.end() ?  "YES" : "NO"},
		{"isFeatured", std::find(tags.begin(), tags.end(), "FEATURED") != tags.end() ?  "YES" : "NO"}
	});
}

void AnalyticsSingleton::shopPurchasePopupClosed(const ShopDisplayItemRef& item)
{
	const std::vector<std::string>& tags = item->getTags();
	mixPanelSendEventWithStoredProperties("ShopPurchasePopupClosed" ,{
		{"itemName", item->getInventoryItem()->getName()},
		{"itemPrice", cocos2d::StringUtils::format("%d",item->getPrice())},
		{"itemShape", item->getShape()},
		{"isNew", std::find(tags.begin(), tags.end(), "NEW") != tags.end() ?  "YES" : "NO"},
		{"isFeatured", std::find(tags.begin(), tags.end(), "FEATURED") != tags.end() ?  "YES" : "NO"}
	});
}

void AnalyticsSingleton::shopPurchseAnimClosed(const ShopDisplayItemRef& item)
{
	const std::vector<std::string>& tags = item->getTags();
	mixPanelSendEventWithStoredProperties("ShopPurchasedAnimClosed" ,{
		{"itemName", item->getInventoryItem()->getName()},
		{"itemPrice", cocos2d::StringUtils::format("%d",item->getPrice())},
		{"itemShape", item->getShape()},
		{"isNew", std::find(tags.begin(), tags.end(), "NEW") != tags.end() ?  "YES" : "NO"},
		{"isFeatured", std::find(tags.begin(), tags.end(), "FEATURED") != tags.end() ?  "YES" : "NO"}
	});
}

void AnalyticsSingleton::shopPurchasedAnimUsePressed(const ShopDisplayItemRef& item)
{
	const std::vector<std::string>& tags = item->getTags();
	mixPanelSendEventWithStoredProperties("ShopPurchasedAnimUsePressed" ,{
		{"itemName", item->getInventoryItem()->getName()},
		{"itemPrice", cocos2d::StringUtils::format("%d",item->getPrice())},
		{"itemShape", item->getShape()},
		{"isNew", std::find(tags.begin(), tags.end(), "NEW") != tags.end() ?  "YES" : "NO"},
		{"isFeatured", std::find(tags.begin(), tags.end(), "FEATURED") != tags.end() ?  "YES" : "NO"}
	});
}

void AnalyticsSingleton::openShopEvent()
{
    mixPanelSendEventWithStoredProperties("shopOpen");
}

void AnalyticsSingleton::coinCounterPressedEvent()
{
	mixPanelSendEventWithStoredProperties("coinCounterPressed");
}

//-------------DEBUG----------------------------------------
void AnalyticsSingleton::backendRequestCompleteEvent(const std::string& requestTag, const std::string& qid)
{
	mixPanelSendEventWithStoredProperties("backendRequestComplete" ,{{"requestTag", requestTag},{"qid",qid}});
}
void AnalyticsSingleton::debugEvent(const std::string& eventName, const std::map<std::string,std::string>& params)
{
	mixPanelSendEventWithStoredProperties("DEBUG_" + eventName, params);
}

NS_TZ_END
