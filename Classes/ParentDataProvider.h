#include "cocos2d.h"
#include "external/json/document.h"

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
    
    std::string getLoggedInParentActorStatus();
    std::string getLoggedInParentId();
    std::string getLoggedInParentApiKey();
};
