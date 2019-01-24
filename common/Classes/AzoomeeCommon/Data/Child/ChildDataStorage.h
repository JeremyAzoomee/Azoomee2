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
	
	void setChildLoggedIn(bool loggedIn);
	bool isChildLoggedIn() const;
	
	void setLoggedInChild(const ChildRef& child);
	ChildRef getLoggedInChild() const;
	
private:
    bool _childLoggedIn = false;
	ChildRef _loggedInChild = nullptr;
};

}

#endif
