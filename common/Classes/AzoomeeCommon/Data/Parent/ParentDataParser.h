#ifndef AzoomeeCommon_ParentDataParser_h
#define AzoomeeCommon_ParentDataParser_h

#include <cocos/cocos2d.h>
#include "../Json.h"
#include "../Child/Child.h"


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
    void parseParentSessionData(const std::string &responseData);
    void parseParentDetails(const std::string &responseData);
    void parseChildUpdateData(const ChildRef& child, const std::string& responseData);
    void logoutChild();
    void retrieveParentLoginDataFromUserDefaults();
    bool hasParentLoginDataInUserDefaults();
    void clearParentLoginDataFromUserDefaults();
    bool parsePendingFriendRequests(const std::string &responseData);
    void setBillingDataAvailable(bool isAvailable);
    void setLoggedInParentCountryCode(const std::string &countryCode);
	
	void saveAnonCredentialsToDevice(const std::string& userId);
    
private:
    void addParentLoginDataToUserDefaults();
};
  
}

#endif
