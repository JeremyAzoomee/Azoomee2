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

NS_AZOOMEE_BEGIN
	
class ChildManager
{
		
public:
	static ChildManager* getInstance();
	
	virtual ~ChildManager();
	
	void setChildLoggedIn(bool loggedIn);
	bool isChildLoggedIn() const;
	
	void setLoggedInChild(const MutableChildRef& child);
	ChildRef getLoggedInChild() const;
	
	std::string getParentOrChildId() const;
	std::string getParentOrChildCdnSessionId() const;
	std::string getParentOrChildApiSecret() const;
	std::string getParentOrChildApiKey() const;
	std::string getParentOrChildAvatarId() const;
	std::string getParentOrChildName() const;
	
	void parseChildInventory(const std::string& inventoryData);
	void parseAvatarUpdate(const std::string& avatarData);
	void parseChildSessionUpdate(const std::string& sessionData);
	void loginChildOffline(const std::string& childId);
	
private:
	bool _childLoggedIn = false;
	MutableChildRef _loggedInChild = nullptr;
};

NS_AZOOMEE_END

#endif /* ChildManager_h */
