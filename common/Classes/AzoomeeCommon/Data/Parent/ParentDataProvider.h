#ifndef AzoomeeCommon_ParentDataProvider_h
#define AzoomeeCommon_ParentDataProvider_h

#include <cocos/cocos2d.h>
#include "external/json/document.h"


namespace Azoomee
{

class ParentDataProvider : public cocos2d::Ref
{
    
public:
    static ParentDataProvider* getInstance(void);
    
public:
    virtual ~ParentDataProvider();
    bool init(void);
    
    int getAmountOfAvailableChildren();
    std::string getProfileNameForAnAvailableChildren(int childNumber);
    std::string getAvatarForAnAvailableChildren(int childNumber);
    std::string getDOBForAnAvailableChildren(int childNumber);
    std::string getSexForAnAvailableChildren(int childNumber);
    
    std::string getLoggedInParentActorStatus();
    std::string getLoggedInParentId();
    std::string getLoggedInParentApiKey();
    std::string getLoggedInParentApiSecret();
    std::string getParentPin();
    std::string getBillingStatus();
    
    bool isPaidUser();
    bool emailRequiresVerification();
};

}

#endif
