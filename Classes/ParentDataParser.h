#include <cocos/cocos2d.h>
#include "external/json/document.h"

class ParentDataParser : public cocos2d::Ref
{
    
public:
    static ParentDataParser* getInstance(void);
    
    virtual ~ParentDataParser();
    bool init(void);
    
    bool parseParentLoginData(std::string responseData);
    bool parseAvailableChildren(std::string responseData);
    bool parseUpdateParentData(std::string responseData);
    void parseParentBillingData(std::string responseData);
    void logoutChild();
    void retrieveParentLoginDataFromUserDefaults();
    bool hasParentLoginDataInUserDefaults();
    void clearParentLoginDataFromUserDefaults();
    
private:
    void addParentLoginDataToUserDefaults();
};
