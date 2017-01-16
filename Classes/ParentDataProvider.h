#include "cocos2d.h"
#include "external/json/document.h"

class ParentDataProvider : public cocos2d::Ref
{
    
public:
    static ParentDataProvider* getInstance(void);
    
public:
    virtual ~ParentDataProvider();
    bool init(void);
    
    std::string getAvailableChildrenValue(std::string keyName);
    int getAmountOfAvailableChildren();
    std::string getValueFromOneAvailableChild(int childNumber, std::string keyName);
    
    std::string getLoggedInParentActorStatus();
    std::string getLoggedInParentId();
    std::string getLoggedInParentApiKey();
};
