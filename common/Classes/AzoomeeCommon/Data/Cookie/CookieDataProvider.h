#ifndef AzoomeeCommon_CookieDataProvider_h
#define AzoomeeCommon_CookieDataProvider_h

#include <cocos/cocos2d.h>
#include "../Json.h"

namespace Azoomee
{

class CookieDataProvider : public cocos2d::Ref
{
    
public:
    static CookieDataProvider* getInstance(void);
    
public:
    virtual ~CookieDataProvider();
    bool init(void);
    
    std::string getCookiesForRequest(const std::string& url);
    std::string getCookieMainContent(const std::string& cookieRecord);
    std::string getAllCookiesInJson();
    
private:
    bool checkIfCookieIsForUrl(const std::string& cookieRecord, const std::string& url);
    std::string getUrlWithPathFromCookie(const std::string& cookieString);
    std::string getDomainFromCookie(const std::string& cookieString);
};

}

#endif
