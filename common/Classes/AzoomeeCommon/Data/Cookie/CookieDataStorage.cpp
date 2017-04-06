#include "CookieDataStorage.h"

using namespace cocos2d;

namespace Azoomee
{

static CookieDataStorage *_sharedCookieDataStorage = NULL;

CookieDataStorage* CookieDataStorage::getInstance()
{
    if (! _sharedCookieDataStorage)
    {
        _sharedCookieDataStorage = new CookieDataStorage();
        _sharedCookieDataStorage->init();
    }
    
    return _sharedCookieDataStorage;
}

CookieDataStorage::~CookieDataStorage(void)
{
}

bool CookieDataStorage::init(void)
{
    return true;
}
  
}
