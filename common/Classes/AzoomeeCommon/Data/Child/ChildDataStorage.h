#ifndef AzoomeeCommon_ChildDataStorage_h
#define AzoomeeCommon_ChildDataStorage_h

#include "cocos2d.h"
#include "external/json/document.h"


namespace Azoomee
{

class ChildDataStorage : public cocos2d::Ref
{
    
public:
    static ChildDataStorage* getInstance(void);

    virtual ~ChildDataStorage();
    bool init(void);
    
    rapidjson::Document childLoginData;
    bool childLoggedIn;
    
    std::string loggedInChildName;
    std::string loggedInChildAvatarId;
    int loggedInChildNumber;
    
    std::string loggedInChildId;
    std::string loggedInChildCdnSessionId;
    std::string loggedInChildApiSecret;
    std::string loggedInChildApiKey;
};

}

#endif
