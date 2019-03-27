#include "ParentDataStorage.h"


namespace Azoomee
{

static ParentDataStorage *_sharedParentDataStorage = NULL;

ParentDataStorage* ParentDataStorage::getInstance()
{
    if (! _sharedParentDataStorage)
    {
        _sharedParentDataStorage = new ParentDataStorage();
        _sharedParentDataStorage->init();
    }
    
    return _sharedParentDataStorage;
}

ParentDataStorage::~ParentDataStorage(void)
{
}

bool ParentDataStorage::init(void)
{
    return true;
}

void ParentDataStorage::setBillingData(const BillingDataRef& billingData)
{
	_billingData = billingData;
}
BillingDataRef ParentDataStorage::getBillingData() const
{
	return _billingData;
}
void ParentDataStorage::setBillingDataAvailable(bool available)
{
	_isBillingDataAvailable = available;
}
bool ParentDataStorage::isBillingDataAvailable() const
{
	return _isBillingDataAvailable;
}
	
void ParentDataStorage::setParent(const ParentRef& parent)
{
	_parent = parent;
}
ParentRef ParentDataStorage::getParent() const
{
	return _parent;
}
	
void ParentDataStorage::clearAvailableChildren()
{
	_availableChildren.clear();
	_availableChildrenById.clear();
}
void ParentDataStorage::addChild(const ChildRef& child)
{
	_availableChildren.push_back(child);
	_availableChildrenById[child->getId()] = child;
}
	
std::vector<ChildRef> ParentDataStorage::getAvailableChildren() const
{
	return _availableChildren;
}
std::map<std::string, ChildRef> ParentDataStorage::getAvailableChildrenById() const
{
	return _availableChildrenById;
}
	
void ParentDataStorage::setPendingFriendRequests(const std::vector<FriendRequestRef>& pendingRequests)
{
	_pendingFriendRequests = pendingRequests;
}
std::vector<FriendRequestRef> ParentDataStorage::getPendingFriendRequests() const
{
	return _pendingFriendRequests;
}
	
}
