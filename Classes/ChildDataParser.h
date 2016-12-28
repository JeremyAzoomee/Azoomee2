#include "cocos2d.h"
#include "external/json/document.h"

class ChildDataParser : public cocos2d::Ref
{
    
public:
    static ChildDataParser* getInstance(void);
    
public:
    virtual ~ChildDataParser();
    bool init(void);
    
    rapidjson::Document childLoginData;
    bool parseChildLoginData(std::string responseData);
    
    bool childLoggedIn;
};
