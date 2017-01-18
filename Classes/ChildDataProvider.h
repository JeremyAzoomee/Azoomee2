#include "cocos2d.h"
#include "external/json/document.h"

class ChildDataProvider : public cocos2d::Ref
{
    
public:
    static ChildDataProvider* getInstance(void);
    
    virtual ~ChildDataProvider();
    bool init(void);
    
    std::string getLoggedInChildName();
    std::string getLoggedInChildId();
    int getLoggedInChildNumber();
    
    std::string getParentOrChildId();
    std::string getParentOrChildCdnSessionId();
    std::string getParentOrChildApiSecret();
    std::string getParentOrChildApiKey();
    
    bool getIsChildLoggedIn();
};
