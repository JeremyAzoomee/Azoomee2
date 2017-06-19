#ifndef AzoomeeCommon_CookieDataParser_h
#define AzoomeeCommon_CookieDataParser_h

#include <cocos/cocos2d.h>
#include "../Json.h"


namespace Azoomee
{

class CookieDataParser : public cocos2d::Ref
{
    
public:
    static CookieDataParser* getInstance(void);
    virtual ~CookieDataParser();
    bool init(void);
    
    //Methods
    bool parseDownloadCookies(std::string responseString);
    
};
  
}

#endif
