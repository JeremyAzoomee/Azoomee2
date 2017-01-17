#include "cocos2d.h"

class HMACSHA256_ios : public cocos2d::Ref
{
    
public:
    /** Returns the shared instance of the Game Manager */
    static HMACSHA256_ios* getInstance(void);

    virtual ~HMACSHA256_ios();
    bool init(void);
    
    std::string getHMACSHA256Hash(std::string message, std::string secret);
    
};
