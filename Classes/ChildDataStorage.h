#include "cocos2d.h"
#include "external/json/document.h"

class ChildDataStorage : public cocos2d::Ref
{
    
public:
    static ChildDataStorage* getInstance(void);
    
public:
    virtual ~ChildDataStorage();
    bool init(void);
    
    rapidjson::Document childLoginData;
    bool childLoggedIn;
    std::string loggedInChildName;
    int loggedInChildNumber;
};
