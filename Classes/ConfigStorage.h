#include "cocos2d.h"

class ConfigStorage : public cocos2d::Ref
{
    
public:
    /** Returns the shared instance of the Game Manager */
    static ConfigStorage* getInstance(void);
    
public:
    virtual ~ConfigStorage();
    bool init(void);
    
};
