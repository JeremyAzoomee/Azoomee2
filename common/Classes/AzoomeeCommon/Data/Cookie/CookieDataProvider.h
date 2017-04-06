#include <cocos/cocos2d.h>
#include "external/json/document.h"

namespace Azoomee
{

class CookieDataProvider : public cocos2d::Ref
{
    
public:
    static CookieDataProvider* getInstance(void);
    
public:
    virtual ~CookieDataProvider();
    bool init(void);
    
    std::string getCookiesForRequest(std::string url);
    std::string getCookieMainContent(std::string cookieRecord);
    
private:
    std::vector<std::string> splitStringToVector(std::string inputString, std::string separator);
    bool checkIfCookieIsForUrl(std::string cookieRecord, std::string url);
};

}
