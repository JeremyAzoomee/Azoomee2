#ifndef AzoomeeCommon_ChildDataStorage_h
#define AzoomeeCommon_ChildDataStorage_h

#include <cocos/cocos2d.h>
#include "../Json.h"
#include "Child.h"


namespace Azoomee
{

class ChildDataStorage : public cocos2d::Ref
{
    
public:
    static ChildDataStorage* getInstance(void);

    virtual ~ChildDataStorage();
    bool init(void);
    
    bool childLoggedIn = false;
	ChildRef _loggedInChild = nullptr;
};

}

#endif
