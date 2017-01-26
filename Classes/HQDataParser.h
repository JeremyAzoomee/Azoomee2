#include "cocos2d.h"
#include "network/HttpClient.h"
#include "external/json/document.h"

class HQDataParser : public cocos2d::Ref
{
    
public:
    static HQDataParser* getInstance(void);
    virtual ~HQDataParser();
    bool init(void);
    
    //Data Parsing methods
    bool parseHQData(std::string responseString, const char *category);
    bool parseHQStructure(std::string responseString, const char *category);
    bool parseHQGetContentUrls(std::string responseString);
    void getContent(std::string url, std::string category);
    std::string getExtensionFromUri(std::string uri);
    
    //Communication methods
    void onGetContentAnswerReceived(std::string responseString, std::string category);
};
