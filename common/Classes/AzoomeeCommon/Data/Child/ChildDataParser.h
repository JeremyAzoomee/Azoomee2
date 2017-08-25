#ifndef AzoomeeCommon_ChildDataParser_h
#define AzoomeeCommon_ChildDataParser_h

#include <cocos/cocos2d.h>
#include "../Json.h"


namespace Azoomee
{

class ChildDataParser : public cocos2d::Ref
{
    
public:
    static ChildDataParser* getInstance(void);

    virtual ~ChildDataParser();
    bool init(void);
    
    bool parseChildLoginData(const std::string &responseData);
    void parseOomeeData(const std::string &responseData);
    
    void setLoggedInChildName(const std::string &childName);
    void setLoggedInChildId(const std::string &id);
    void setLoggedInChildNumber(int childNumber);
    void setLoggedInChildAvatarId(const std::string &avatarId);
    void setChildLoggedIn(bool loggedIn);
};

}

#endif
