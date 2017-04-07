#ifndef AzoomeeCommon_ParentDataStorage_h
#define AzoomeeCommon_ParentDataStorage_h

#include <cocos/cocos2d.h>
#include "external/json/document.h"


namespace Azoomee
{
  
class ParentDataStorage : public cocos2d::Ref
{
    
public:
    static ParentDataStorage* getInstance(void);

    virtual ~ParentDataStorage();
    bool init(void);
    
    rapidjson::Document parentLoginData;
    rapidjson::Document availableChildrenData;
    
    std::string loggedInParentId;
    std::string loggedInParentCdnSessionId;
    std::string loggedInParentApiSecret;
    std::string loggedInParentApiKey;
    std::string loggedInParentActorStatus;
    std::string loggedInParentPin;
    std::string loggedInParentBillingStatus;
    
    std::vector<std::map<std::string, std::string>> availableChildren;          //array of maps, where each child has profileName and avatar keys
};

}

#endif
