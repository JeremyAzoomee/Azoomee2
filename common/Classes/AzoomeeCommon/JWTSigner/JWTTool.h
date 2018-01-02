#ifndef AzoomeeCommon_JWTTool_h
#define AzoomeeCommon_JWTTool_h

#include <cocos/cocos2d.h>
#include "../Data/Json.h"


namespace Azoomee
{

class JWTTool : public cocos2d::Ref
{
    
public:
    void setMethod(const std::string &method);
    void setPath(const std::string &path);
    void setHost(const std::string &host);
    void setQueryParams(const std::string &queryParams);
    void setRequestBody(const std::string &requestBody);
    void setForceParent(bool forceParent);
    
    std::string buildJWTString();
    
private:
    std::string _method = "";
    std::string _path = "";
    std::string _host = "";
    std::string _queryParams = "";
    std::string _requestBody = "";
    bool _forceParent = false;
    
    std::string getAppropriateAPISecret();
    
    std::string getDateFormatString();
    std::string addLeadingZeroToDateElement(int input);
    std::string getBase64Encoded(std::string input);
    std::string stringToLower(std::string input);
    
    std::string url_encode(const std::string &value);
    
    std::string getHeaderString(std::string kid);
    std::string getBodySignature();
    std::string getBodyString();
    std::string getJWTSignature(std::string sHeader, std::string sBody);
};
  
}

#endif
