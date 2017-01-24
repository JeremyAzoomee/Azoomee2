#include "cocos2d.h"
#include "external/json/document.h"

class ChildDataParser : public cocos2d::Ref
{
    
public:
    static ChildDataParser* getInstance(void);

    virtual ~ChildDataParser();
    bool init(void);
    
    bool parseChildLoginData(std::string responseData);
    
    void setLoggedInChildName(std::string childName);
    void setLoggedInChildId(std::string id);
    void setLoggedInChildNumber(int childNumber);
};
