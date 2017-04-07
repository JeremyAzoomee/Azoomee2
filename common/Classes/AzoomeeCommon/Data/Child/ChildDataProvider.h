#ifndef AzoomeeCommon_ChildDataProvider_h
#define AzoomeeCommon_ChildDataProvider_h

#include "cocos2d.h"
#include "external/json/document.h"

namespace Azoomee
{

class ChildDataProvider : public cocos2d::Ref
{
    
public:
    static ChildDataProvider* getInstance(void);
    
    virtual ~ChildDataProvider();
    bool init(void);
    
    std::string getLoggedInChildName();
    std::string getLoggedInChildId();
    std::string getLoggedInChildAvatarId();
    int getLoggedInChildNumber();
    
    std::string getParentOrChildId();
    std::string getParentOrChildCdnSessionId();
    std::string getParentOrChildApiSecret();
    std::string getParentOrChildApiKey();
    
    bool getIsChildLoggedIn();
};
  
}

#endif
