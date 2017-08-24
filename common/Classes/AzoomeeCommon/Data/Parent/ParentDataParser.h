#ifndef AzoomeeCommon_ParentDataParser_h
#define AzoomeeCommon_ParentDataParser_h

#include <cocos/cocos2d.h>
#include "../Json.h"


namespace Azoomee
{

class ParentDataParser : public cocos2d::Ref
{
    
public:
    static ParentDataParser* getInstance(void);
    
    virtual ~ParentDataParser();
    bool init(void);
    
    bool parseParentLoginData(const std::string &responseData);
    bool parseAvailableChildren(const std::string &responseData);
    bool parseUpdateParentData(const std::string &responseData);
    void parseParentBillingData(const std::string &responseData);
    void logoutChild();
    void retrieveParentLoginDataFromUserDefaults();
    bool hasParentLoginDataInUserDefaults();
    void clearParentLoginDataFromUserDefaults();
    bool parsePendingFriendRequests(const std::string &responseData);
    
private:
    void addParentLoginDataToUserDefaults();
};
  
}

#endif
