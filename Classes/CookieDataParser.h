#include "cocos2d.h"
#include "external/json/document.h"

class CookieDataParser : public cocos2d::Ref
{
    
public:
    static CookieDataParser* getInstance(void);
    virtual ~CookieDataParser();
    bool init(void);
    
    //Methods
    bool parseDownloadCookies(std::string responseString);
    
private:
    std::string replaceAll(std::string& str, const std::string& from, const std::string& to);
    std::vector<std::string> splitStringToVector(std::string inputString, std::string separator);
};
