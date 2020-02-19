//
//  ChildManager.h
//  AzoomeeCommon
//
//  Created by Macauley on 05/06/2019.
//

#ifndef ChildManager_h
#define ChildManager_h

#include <cocos/cocos2d.h>
#include "../Json.h"
#include "Child.h"

NS_TZ_BEGIN
	
class ChildManager
{
public:
    static const std::string kInventoryUpdatedEvent;
    
	static ChildManager* getInstance();
	virtual ~ChildManager();
	
	void setChildLoggedIn(bool loggedIn);
	bool isChildLoggedIn() const;
	
	void setLoggedInChild(const MutableChildRef& child);
	ChildRef getLoggedInChild() const;
	
	//std::string getParentOrChildId() const;
	//std::string getParentOrChildCdnSessionId() const;
	//std::string getParentOrChildApiSecret() const;
	//std::string getParentOrChildApiKey() const;
	//std::string getParentOrChildAvatarId() const;
	//std::string getParentOrChildName() const;
    
    /// Get the latest inventory from the server
    void updateInventory();
	
	void parseAvatarUpdate(const std::string& avatarData);
	void parseChildSessionUpdate(const std::string& sessionData);
	void loginChildOffline(const std::string& childId);
	
private:
    void parseChildInventory(const std::string& inventoryData);
    
	bool _childLoggedIn = false;
	MutableChildRef _loggedInChild = nullptr;
};

NS_TZ_END

#endif /* ChildManager_h */
