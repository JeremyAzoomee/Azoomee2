#ifndef TinizineCommon_HMACSHA256_h
#define TinizineCommon_HMACSHA256_h

#include <cocos/cocos2d.h>
#include "../../Tinizine.h"

NS_TZ_BEGIN

class HMACSHA256 : public cocos2d::Ref
{
    
public:
    /** Returns the shared instance of the Game Manager */
    static HMACSHA256* getInstance(void);
    
    virtual ~HMACSHA256();
    bool init(void);
    
    std::string getHMACSHA256Hash(const std::string& message, const std::string& secret);
    
};
  
NS_TZ_END

#endif
