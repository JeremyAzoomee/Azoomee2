#include "HMACSHA256.h"

using namespace cocos2d;


namespace Azoomee
{

static HMACSHA256 *_sharedHMACSHA256 = NULL;

HMACSHA256* HMACSHA256::getInstance()
{
  if (! _sharedHMACSHA256)
  {
    _sharedHMACSHA256 = new HMACSHA256();
    _sharedHMACSHA256->init();
  }
  
  return _sharedHMACSHA256;
}

HMACSHA256::~HMACSHA256(void)
{
}

bool HMACSHA256::init(void)
{
  return true;
}
  
}
