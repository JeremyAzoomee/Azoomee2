#include "ChildDataStorage.h"

using namespace cocos2d;

namespace Azoomee
{

static ChildDataStorage *_sharedChildDataStorage = NULL;

ChildDataStorage* ChildDataStorage::getInstance()
{
    if (! _sharedChildDataStorage)
    {
        _sharedChildDataStorage = new ChildDataStorage();
        _sharedChildDataStorage->init();
    }
    
    return _sharedChildDataStorage;
}

ChildDataStorage::~ChildDataStorage(void)
{
}

bool ChildDataStorage::init(void)
{
    _childLoggedIn = false;
    
    return true;
}

void ChildDataStorage::setChildLoggedIn(bool loggedIn)
{
	_childLoggedIn = loggedIn;
}
bool ChildDataStorage::isChildLoggedIn() const
{
	return _childLoggedIn;
}
	
void ChildDataStorage::setLoggedInChild(const ChildRef& child)
{
	_loggedInChild = child;
}
ChildRef ChildDataStorage::getLoggedInChild() const
{
	return _loggedInChild;
}
}
