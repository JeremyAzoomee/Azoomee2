#include "cocos2d.h"
#include "external/json/document.h"

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
};
