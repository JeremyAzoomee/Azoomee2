#ifndef AzoomeeCommon_ParentDataStorage_h
#define AzoomeeCommon_ParentDataStorage_h

#include <cocos/cocos2d.h>
#include "../Json.h"
#include "BillingData.h"
#include "../Child/Child.h"
#include "Parent.h"
#include "FriendRequest.h"
#include <map>


namespace Azoomee
{
  
class ParentDataStorage : public cocos2d::Ref
{
    
public:
    static ParentDataStorage* getInstance(void);

    virtual ~ParentDataStorage();
    bool init(void);
	
	void setBillingData(const BillingDataRef& billingData);
	BillingDataRef getBillingData() const;
	void setBillingDataAvailable(bool available);
	bool isBillingDataAvailable() const;
	
	void setParent(const ParentRef& parent);
	ParentRef getParent() const;
	
	void clearAvailableChildren();
	void addChild(const ChildRef& child);
	std::vector<ChildRef> getAvailableChildren() const;
	std::map<std::string, ChildRef> getAvailableChildrenById() const;
	
	void setPendingFriendRequests(const std::vector<FriendRequestRef>& pendingRequests);
	std::vector<FriendRequestRef> getPendingFriendRequests() const;
	
private:
	BillingDataRef _billingData = nullptr;
	ParentRef _parent = nullptr;
	
    bool _isBillingDataAvailable = false;
	
	std::vector<ChildRef> _availableChildren;
	std::map<std::string, ChildRef> _availableChildrenById;
	
    std::vector<FriendRequestRef> _pendingFriendRequests;
};

}

#endif
