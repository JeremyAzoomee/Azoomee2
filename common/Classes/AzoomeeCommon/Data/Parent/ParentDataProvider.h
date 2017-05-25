#ifndef AzoomeeCommon_ParentDataProvider_h
#define AzoomeeCommon_ParentDataProvider_h

#include <cocos/cocos2d.h>
#include "../Json.h"


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
    std::string getProfileNameForAnAvailableChildrenById(const std::string& childId);
    std::string getAvatarForAnAvailableChildren(int childNumber);
    std::string getAvatarForAnAvailableChildrenById(const std::string& childId);
    std::string getDOBForAnAvailableChildren(int childNumber);
    std::string getSexForAnAvailableChildren(int childNumber);
    
    std::string getLoggedInParentActorStatus();
    std::string getLoggedInParentId();
    std::string getLoggedInParentApiKey();
    std::string getLoggedInParentApiSecret();
    std::string getParentPin();
    std::string getBillingStatus();
    std::string getBillingDate();
    std::string getBillingProvider();
    
    bool isPaidUser();
    bool emailRequiresVerification();
};

}

#endif
