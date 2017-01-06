#include "cocos2d.h"
#include "external/json/document.h"

class ChildDataParser : public cocos2d::Ref
{
    
public:
    static ChildDataParser* getInstance(void);
    
public:
    virtual ~ChildDataParser();
    bool init(void);
    
    bool parseChildLoginData(std::string responseData);
    
    void setLoggedInChildName(std::string childName);
    void setLoggedInChildNumber(int childNumber);
};
