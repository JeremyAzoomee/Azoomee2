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
  
}
