#ifndef AzoomeeCommon_ChildDataParser_h
#define AzoomeeCommon_ChildDataParser_h

#include <cocos/cocos2d.h>
#include "external/json/document.h"


namespace Azoomee
{

class ChildDataParser : public cocos2d::Ref
{
    
public:
    static ChildDataParser* getInstance(void);

    virtual ~ChildDataParser();
    bool init(void);
    
    bool parseChildLoginData(std::string responseData);
    void parseOomeeData(std::string responseData);
    
    void setLoggedInChildName(std::string childName);
    void setLoggedInChildId(std::string id);
    void setLoggedInChildNumber(int childNumber);
    void setLoggedInChildAvatarId(std::string avatarId);
    void setChildLoggedIn(bool loggedIn);
};

}

#endif
