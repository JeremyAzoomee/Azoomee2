#include "cocos2d.h"
#include "external/json/document.h"

class CookieDataStorage : public cocos2d::Ref
{
    
public:
    static CookieDataStorage* getInstance(void);
    
public:
    virtual ~CookieDataStorage();
    bool init(void);
    
    std::vector<std::string> dataDownloadCookiesVector;
    std::string pureCookieResponseString;
    std::string dataDownloadCookiesForCpp;
};
