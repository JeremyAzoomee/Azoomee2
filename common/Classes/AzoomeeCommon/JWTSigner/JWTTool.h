#ifndef AzoomeeCommon_JWTTool_h
#define AzoomeeCommon_JWTTool_h

#include <cocos/cocos2d.h>
#include "../Data/Json.h"


namespace Azoomee
{

class JWTTool : public cocos2d::Ref
{
    
public:
    /** Returns the shared instance of the Game Manager */
    static JWTTool* getInstance(void);
    virtual ~JWTTool();
    bool init(void);
    std::string buildJWTString(std::string method, std::string path, std::string host, std::string queryParams, std::string requestBody);
    
private:
    std::string getDateFormatString();
    std::string addLeadingZeroToDateElement(int input);
    std::string getBase64Encoded(std::string input);
    std::string stringToLower(std::string input);
    
    std::string url_encode(const std::string &value);
    std::string getUrlParamsInAlphabeticalOrder(const std::string &originalParams);
    
    std::string getHeaderString(std::string kid);
    std::string getBodySignature(std::string method, std::string path, std::string host, std::string queryParams, std::string requestBody);
    std::string getBodyString(std::string method, std::string path, std::string host, std::string queryParams, std::string requestBody);
    std::string getJWTSignature(std::string sHeader, std::string sBody);
};
  
}

#endif
