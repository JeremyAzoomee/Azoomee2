#include "cocos2d.h"

class HMACSHA256_ios : public cocos2d::Object
{
    
public:
    /** Returns the shared instance of the Game Manager */
    static HMACSHA256_ios* getInstance(void);
    
public:
    virtual ~HMACSHA256_ios();
    bool init(void);
    
    std::string getHMACSHA256Hash(std::string message, std::string secret);
    
};
