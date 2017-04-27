#include <cocos/cocos2d.h>

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
    std::string getExtensionFromUri(std::string uri);
    
    void clearAllHQData();
    
    //Communication methods
    void onGetContentAnswerReceived(std::string responseString, std::string category);
    void onGetPreviewContentAnswerReceived(std::string responseString);
};
