#include "cocos2d.h"
#include "external/json/document.h"

class ChildDataProvider : public cocos2d::Ref
{
    
public:
    static ChildDataProvider* getInstance(void);
    
    virtual ~ChildDataProvider();
    bool init(void);
    
    std::string getParentOrChildLoginValue(std::string keyName);
    std::string getLoggedInChildName();
    std::string getLoggedInChildId();
};
