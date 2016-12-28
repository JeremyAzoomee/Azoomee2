#include "cocos2d.h"
#include "external/json/document.h"

class ParentDataParser : public cocos2d::Ref
{
    
public:
    static ParentDataParser* getInstance(void);
    
public:
    virtual ~ParentDataParser();
    bool init(void);
    
    //Methods
    bool parseParentLoginData(std::string responseData);
    bool parseAvailableChildren(std::string responseData);
    
    //Variables
    rapidjson::Document parentLoginData;
    rapidjson::Document availableChildrenData;
};
