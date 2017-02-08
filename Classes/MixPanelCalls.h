#include "cocos2d.h"

void createOSSpecficCall(std::string eventID);

void mixPanel_androidJNIHelper(std::string eventID, std::string propertiesJSONString);

//---------------SUPER PROPERTIES

void mixPanel_registerSuperProperties(std::string ParentID, std::string AccountStatus, std::string ChildID, std::string ChildGender,std::string ChildDob);

void mixPanel_OSSpecificSuperPropertiesCall(std::string Key, std::string Property);

void mixPanel_registerParentID(std::string ParentID);
void mixPanel_registerChildID(std::string ChildID);
void mixPanel_registerAzoomeeEmail(std::string emailAddress);

//-------------Startup--------------------

void mixPanel_fistLaunchEvent();

void mixPanel_signInSuccessEvent();
void mixPanel_signInFailEvent(int errorCode);

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
void mixPanel_childProfileDOBErrorEvent();
void mixPanel_childProfileOomeeEvent();
void mixPanel_childProfileCreatedSuccessEvent();
void mixPanel_childProfileCreatedErrorEvent(long errorCode);
