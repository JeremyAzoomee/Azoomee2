#ifndef Azoomee_HQDataParser_h
#define Azoomee_HQDataParser_h

#include <cocos/cocos2d.h>
#include <AzoomeeCommon/Azoomee.h>

NS_AZOOMEE_BEGIN

class HQDataParser : public cocos2d::Ref
{
    
public:
    static HQDataParser* getInstance(void);
    virtual ~HQDataParser();
    bool init(void);
    
    //Data Parsing methods
    bool parseHQData(const std::string &responseString, const char *category);
    bool parseHQStructure(const std::string &responseString, const char *category);
    bool parseHQGetContentUrls(const std::string &responseString);
    std::string getExtensionFromUri(const std::string &uri);
    
    //Communication methods
    void onGetContentAnswerReceived(const std::string &responseString, const std::string &category);
    void onGetPreviewContentAnswerReceived(const std::string &responseString);
};

NS_AZOOMEE_END

#endif
