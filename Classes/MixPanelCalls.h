#include "cocos2d.h"

enum percentageComplete
{
    quater = 25,
    half = 50,
    complete = 100
};

void createOSSpecficCall(std::string eventID);

void mixPanel_androidJNIHelper(std::string eventID, std::string propertiesJSONString);

//---------------SUPER PROPERTIES

void mixPanel_OSSpecificSuperPropertiesCall(std::string Key, std::string Property);

void mixPanel_registerAppVersion();
void mixPanel_registerParentID(std::string ParentID);
void mixPanel_registerNoOfChildren(int noOfChildren);
void mixPanel_registerAzoomeeEmail(std::string emailAddress);
void mixPanel_registerAccountStatus(std::string Status);   //NEED TO BE ADDED TO CODE

void mixPanel_registerChildID(std::string ChildID);
void mixPanel_registerChildGender(std::string ChildGender); //NEED TO BE ADDED TO CODE
void mixPanel_registerChildDOB(std::string ChildDOB); //NEED TO BE ADDED TO CODE

void mixPanel_logoutChild();
void mixPanel_logoutParent();

//-------------Startup--------------------

void mixPanel_fistLaunchEvent();

void mixPanel_signInSuccessEvent();
void mixPanel_signInFailEvent(int errorCode); // NEED TO UNDERSTAND HOW TO PASS THE ERROR CODE

//-------------ONBOARDING--------------------
void mixPanel_OnboardingStartEvent();
void mixPanel_OnboardingEmailSubmittedEvent();
void mixPanel_OnboardingPasswordSubmittedEvent();
void mixPanel_OnboardingPinSubmittedEvent();
void mixPanel_OnboardingAccountCreatedEvent();
void mixPanel_OnboardingAccountCreatedErrorEvent(long errorCode);

//-------------CHILD PROFILE CREATION-------------
void mixPanel_childProfileStartEvent();
void mixPanel_childProfileNameEvent();
void mixPanel_childProdileNameErrorEvent();
void mixPanel_childProfileDOBEvent();
void mixPanel_childProfileDOBErrorEvent(); // NO DOB CHECKING IS COMPLETED
void mixPanel_childProfileOomeeEvent(int oomeeNumber);
void mixPanel_childProfileCreatedSuccessEvent(int oomeeNumber); // HOW TO GET THE OOMEE NUMBER?
void mixPanel_childProfileCreatedErrorEvent(long errorCode);

//-------------HUB ACTIONS-------------------
void mixPanel_hubTapOomee(int oomeeNumber, std::string oomeeAction);
void mixPanel_navSelectionEvent(std::string hubOrTop, int buttonNumber);
void mixPanel_openContentEvent(std::string Title,std::string Description, std::string Type, std::string contentID);
void mixPanel_closeContentEvent(std::string Title,std::string Description, std::string Type, std::string contentID,int SecondsInContent);

//------------- PREVIEW ACTIONS ---------------
void mixPanel_previewContentClickedEvent(std::string Title,std::string Description, std::string Type);
void mixPanel_previewPopupCancelledEvent();

//---------------MEDIA ACTIONS -----------------
void mixPanel_mediaQuality(std::string Title,std::string Description, std::string Type, std::string contentID,int quality);
void mixPanel_mediaProgress(std::string Title,std::string Description, std::string Type,std::string contentID, percentageComplete percentComplete);
void mixPanel_mediaPausedEvent();
void mixPanel_mediaEnd(std::string Title,std::string Description, std::string Type,std::string contentID, std::string NextAutoPlayMediaID, int SecondsMediaPlayed);

//---------------OTHER ACTION------------------
void mixPanel_genericButtonPress(std::string buttonName);
void mixPanel_messageBoxShow(std::string messageTitle);


