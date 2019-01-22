#ifndef AzoomeeCommon_ChildDataProvider_h
#define AzoomeeCommon_ChildDataProvider_h

#include <cocos/cocos2d.h>
#include "../Json.h"
#include "Child.h"

namespace Azoomee
{

class ChildDataProvider : public cocos2d::Ref
{
    
public:
    static ChildDataProvider* getInstance(void);
    
    virtual ~ChildDataProvider();
    bool init(void);
    
    //std::string getLoggedInChildName() const;
    //std::string getLoggedInChildId() const;
    //std::string getLoggedInChildAvatarId() const;
    
    std::string getParentOrChildId() const;
    std::string getParentOrChildCdnSessionId() const;
    std::string getParentOrChildApiSecret() const;
    std::string getParentOrChildApiKey() const;
    std::string getParentOrChildAvatarId() const;
	std::string getParentOrChildName() const;
	
	ChildRef getLoggedInChild() const;
	
    bool getIsChildLoggedIn() const;
};
  
}

#endif
