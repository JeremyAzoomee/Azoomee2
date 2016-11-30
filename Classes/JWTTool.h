#include "cocos2d.h"
#include "external/json/document.h"

class JWTTool : public cocos2d::Object
{
    
public:
    /** Returns the shared instance of the Game Manager */
    static JWTTool* getInstance(void);
    
public:
    virtual ~JWTTool();
    bool init(void);
    
    std::string buildJWTString(std::string method, std::string path, std::string host, std::string queryParams, std::string requestBody);
    std::string getBase64Encoded(std::string input);
    std::string stringToLower(std::string input);
    
    std::string url_encode(const std::string &value);
    
    std::string getHeaderString(std::string kid);
    std::string getBodySignature(std::string method, std::string path, std::string host, std::string queryParams, std::string requestBody);
    std::string getBodyString(std::string method, std::string path, std::string host, std::string queryParams, std::string requestBody);
    std::string getJWTSignature(std::string sHeader, std::string sBody);
};
