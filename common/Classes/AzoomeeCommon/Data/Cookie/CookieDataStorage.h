#ifndef AzoomeeCommon_CookieDataStorage_h
#define AzoomeeCommon_CookieDataStorage_h

#include <cocos/cocos2d.h>
#include "../Json.h"

namespace Azoomee
{

class CookieDataStorage : public cocos2d::Ref
{
    
public:
    static CookieDataStorage* getInstance(void);

    virtual ~CookieDataStorage();
    bool init(void);
    
    std::vector<std::string> dataDownloadCookiesVector;
    std::string pureCookieResponseString;
};
  
}

#endif
