#include "cocos2d.h"

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
void mixPanel_emailSubmittedEvent();
void mixPanel_passwordSubmittedEvent();
void mixPanel_pinSubmittedEvent();
void mixPanel_accountCreatedEvent();
void mixPanel_accountCreatedErrorEvent(long errorCode);

//-------------CHILD PROFILE CREATION-------------
void mixPanel_childProfileStartEvent();
void mixPanel_childProfileNameEvent();
void mixPanel_childProdileNameErrorEvent();
void mixPanel_childProfileDOBEvent();
void mixPanel_childProfileDOBErrorEvent(); // NO DOB CHECKING IS COMPLETED
void mixPanel_childProfileOomeeEvent(int oomeeNumber);
void mixPanel_childProfileCreatedSuccessEvent(int oomeeNumber); // HOW TO GET THE OOMEE NUMBER?
void mixPanel_childProfileCreatedErrorEvent(long errorCode);
