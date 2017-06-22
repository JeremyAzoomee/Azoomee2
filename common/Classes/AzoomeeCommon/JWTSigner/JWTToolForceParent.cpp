#include "JWTToolForceParent.h"
#include "../Data/Json.h"
#include "../Data/Parent/ParentDataProvider.h"
#include "../Data/Child/ChildDataProvider.h"
#include <iomanip>

#include "HMACSHA256/HMACSHA256.h"

using namespace cocos2d;
using namespace rapidjson;


namespace Azoomee
{

static JWTToolForceParent *_sharedJWTToolForceParent = NULL;

JWTToolForceParent* JWTToolForceParent::getInstance()
{
    if (! _sharedJWTToolForceParent)
    {
        _sharedJWTToolForceParent = new JWTToolForceParent();
        _sharedJWTToolForceParent->init();
    }
    
    return _sharedJWTToolForceParent;
}

JWTToolForceParent::~JWTToolForceParent(void)
{
}

bool JWTToolForceParent::init(void)
{
    return true;
}

std::string JWTToolForceParent::getDateFormatString()
{
    time_t rawtime;
    struct tm * ptm;
    time ( &rawtime );
    ptm = gmtime ( &rawtime );
    
    std::string myDateTime = StringUtils::format("%d-%s-%sT%s:%s:%sZ", ptm->tm_year + 1900, addLeadingZeroToDateElement(ptm->tm_mon + 1).c_str(), addLeadingZeroToDateElement(ptm->tm_mday).c_str(), addLeadingZeroToDateElement(ptm->tm_hour).c_str(), addLeadingZeroToDateElement(ptm->tm_min).c_str(), addLeadingZeroToDateElement(ptm->tm_sec).c_str());
    
    return myDateTime;
}

std::string JWTToolForceParent::addLeadingZeroToDateElement(int input)
{
    std::string dateFormat = StringUtils::format("%d", input);
    if(dateFormat.length() == 1)
    {
        dateFormat = StringUtils::format("0%s", dateFormat.c_str());
    }
    
    return dateFormat;
}

std::string JWTToolForceParent::stringToLower(std::string input)
{
    for(int i = 0; i < input.length(); i++)
    {
        input[i] = tolower(input[i]);
    }
    
    return input;
}

std::string JWTToolForceParent::url_encode(const std::string &value) {
    std::ostringstream escaped;
    escaped.fill('0');
    escaped << std::hex;
    
    for (std::string::const_iterator i = value.begin(), n = value.end(); i != n; ++i) {
        std::string::value_type c = (*i);
        
        // Keep alphanumeric and other accepted characters intact
        if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
            escaped << c;
            continue;
        }
        
        // Any other characters are percent-encoded
        escaped << std::uppercase;
        escaped << '%' << std::setw(2) << int((unsigned char) c);
        escaped << std::nouppercase;
    }
    
    return escaped.str();
}

std::string JWTToolForceParent::getBase64Encoded(std::string input) //This is now used only to convert the first and second part of the jwt to base64, the HMAC SHA256 is already base64 encoded.
{
    char *output = NULL;
    cocos2d::base64Encode((unsigned char *)input.c_str(), (unsigned int)input.length(), &output);
    
    std::string outputStr = StringUtils::format("%s", output);
    
    return outputStr;
}

std::string JWTToolForceParent::getHeaderString(std::string kid)
{
    rapidjson::StringBuffer s;
    rapidjson::Writer<rapidjson::StringBuffer> writer(s);
    
    writer.StartObject();
    
    writer.String("alg", (int)StringUtils::format("alg").length());
    writer.String("HS256", (int)StringUtils::format("HS256").length());
    
    writer.String("kid", (int)StringUtils::format("kid").length());
    writer.String(kid.c_str(), (int)kid.length());
    
    writer.EndObject();
    
    //getting the string
    
    const char *jsonString = s.GetString();
    
    CCLOG("\n\n\n Body string: %s\n\n\n", jsonString);
    
    std::string result = getBase64Encoded(StringUtils::format("%s", jsonString));
    
    CCLOG("\n\n\n base64 encoded body: %s\n\n\n", result.c_str());

    return result;
}

std::string JWTToolForceParent::getBodySignature(std::string method, std::string path, std::string host, std::string queryParams, std::string requestBody)
{
    std::string stringContentType = "";
    if(requestBody.length() != 0)
    {
        stringContentType = StringUtils::format("content-type=%s&", url_encode(stringToLower("application/json;charset=UTF-8")).c_str());
    }
    
    std::string stringMandatoryHeaders = StringUtils::format("%shost=%s&x-az-req-datetime=%s", stringContentType.c_str(), url_encode(stringToLower(host)).c_str(), url_encode(stringToLower(getDateFormatString())).c_str());
    
    std::string stringToBeEncoded = StringUtils::format("%s\n%s\n%s\n%s\n%s", method.c_str(), url_encode(path).c_str(), stringToLower(queryParams).c_str(), stringMandatoryHeaders.c_str(), getBase64Encoded(requestBody).c_str());
    std::string bodySignature = HMACSHA256::getInstance()->getHMACSHA256Hash(stringToBeEncoded, ParentDataProvider::getInstance()->getLoggedInParentApiSecret());
    
    cocos2d::log("Payload signature:\n\n%s\nend\n\n", stringToBeEncoded.c_str());
    
    
    return bodySignature;
}

std::string JWTToolForceParent::getBodyString(std::string method, std::string path, std::string host, std::string queryParams, std::string requestBody)
{
    rapidjson::StringBuffer s;
    rapidjson::Writer<rapidjson::StringBuffer> writer(s);
    
    writer.StartObject();
    
    writer.String("iss", (int)StringUtils::format("iss").length());
    writer.String(ParentDataProvider::getInstance()->getLoggedInParentId().c_str(), (int)(StringUtils::format("%s", ParentDataProvider::getInstance()->getLoggedInParentId().c_str()).length()));
    
    writer.String("aud", (int)StringUtils::format("aud").length());
    writer.String("", 0);
    
    writer.String("applicationClaim", (int)StringUtils::format("applicationClaim").length());
    
    writer.StartObject();
    
    writer.String("signature", (int)StringUtils::format("signature").length());
    writer.String(getBodySignature(method, path, host, queryParams, requestBody).c_str(), (int)getBodySignature(method, path, host, queryParams, requestBody).length());
    
    writer.String("parentKey", (int)StringUtils::format("parentKey").length());
    writer.String(ParentDataProvider::getInstance()->getLoggedInParentApiKey().c_str(), (int)ParentDataProvider::getInstance()->getLoggedInParentApiKey().length());
    
    writer.EndObject();
    
    writer.EndObject();
    
    
    const char *jsonString = s.GetString();
    
    cocos2d::log("\n\n\n Body string: %s\n\n\n", jsonString);
    
    std::string result = getBase64Encoded(StringUtils::format("%s", jsonString));
    
    cocos2d::log("\n\n\n base64 encoded body: %s\n\n\n", result.c_str());
    
    return result;
}

std::string JWTToolForceParent::getJWTSignature(std::string sHeader, std::string sBody)
{
    std::string unEncodedSignature = StringUtils::format("%s.%s", sHeader.c_str(), sBody.c_str());
    std::string encodedSignature = HMACSHA256::getInstance()->getHMACSHA256Hash(unEncodedSignature, ParentDataProvider::getInstance()->getLoggedInParentApiSecret());
    
    return encodedSignature;
}

std::string JWTToolForceParent::buildJWTString(std::string method, std::string path, std::string host, std::string queryParams, std::string requestBody)
{
    
    //HEADER STRING------------------------------------------------------------------------------
    
    std::string sHeader = getHeaderString(ParentDataProvider::getInstance()->getLoggedInParentApiKey());
    
    
    //PAYLOAD STRING------------------------------------------------------------------------------
    
    std::string sBody = getBodyString(method, path, host, queryParams, requestBody);
    
    //SIGNATURE STRING----------------------------------------------------------------------------
    
    std::string sSignature = getJWTSignature(sHeader, sBody);
    
    //DISPLAYING DEBUG INFO-----------------------------------------------------------------------
    
    cocos2d::log("\n\n\n apiSecret: %s\n\n\n", ParentDataProvider::getInstance()->getLoggedInParentApiSecret().c_str());
    
    
    //CREATE THE FINAL JWT STRING-----------------------------------------------------------------
    
    std::string finalJWT = StringUtils::format("%s.%s.%s", sHeader.c_str(), sBody.c_str(), sSignature.c_str());
    
    cocos2d::log("\n\n\n FINAL JWT STRING: %s\n\n\n", finalJWT.c_str());
    
    
    return finalJWT;
}
  
}
