#include "cocos2d.h"
#include "external/json/document.h"

class ChildDataProvider : public cocos2d::Ref
{
    
public:
    static ChildDataProvider* getInstance(void);
    
public:
    virtual ~ChildDataProvider();
    bool init(void);
    
    std::string getChildLoginValue(std::string keyName);
    std::string getParentOrChildLoginValue(std::string keyName);
};
