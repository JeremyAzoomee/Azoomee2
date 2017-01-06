#include "cocos2d.h"

class HMACSHA256 : public cocos2d::Ref
{
    
public:
    /** Returns the shared instance of the Game Manager */
    static HMACSHA256* getInstance(void);
    
public:
    virtual ~HMACSHA256();
    bool init(void);
    
    std::string getHMACSHA256Hash(std::string message, std::string secret);
    
};
