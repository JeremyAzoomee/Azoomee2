#include "cocos2d.h"
#include "external/json/document.h"

class ChildDataStorage : public cocos2d::Ref
{
    
public:
    static ChildDataStorage* getInstance(void);
    
    virtual ~ChildDataStorage();
    bool init(void);
    
    rapidjson::Document childLoginData;
    bool childLoggedIn;
    std::string loggedInChildName;
    std::string loggedInChildId;
};
