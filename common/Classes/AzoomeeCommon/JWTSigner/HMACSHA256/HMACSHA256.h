#ifndef AzoomeeCommon_HMACSHA256_h
#define AzoomeeCommon_HMACSHA256_h

#include <cocos/cocos2d.h>


namespace Azoomee
{

class HMACSHA256 : public cocos2d::Ref
{
    
public:
    /** Returns the shared instance of the Game Manager */
    static HMACSHA256* getInstance(void);
    
    virtual ~HMACSHA256();
    bool init(void);
    
    std::string getHMACSHA256Hash(const std::string& message, const std::string& secret);
    
};
  
}

#endif
