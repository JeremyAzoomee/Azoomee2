#include "JWTTool.h"
#include "../Data/Json.h"
#include "../Data/Parent/ParentManager.h"
#include "../Data/Child/ChildManager.h"
#include "../Net/Utils.h"
#include <iomanip>

#include "HMACSHA256/HMACSHA256.h"

using namespace cocos2d;
using namespace rapidjson;


namespace Azoomee
{
    
#pragma mark SETTERS

    void JWTTool::setMethod(const std::string &method)
    {
        _method = method;
    }
    
    void JWTTool::setPath(const std::string &path)
    {
        _path = path;
    }
    
    void JWTTool::setHost(const std::string &host)
    {
        _host = host;
    }
    
    void JWTTool::setQueryParams(const std::string &queryParams)
    {
        _queryParams = queryParams;
    }
    
    void JWTTool::setRequestBody(const std::string &requestBody)
    {
        _requestBody = requestBody;
    }
    
    void JWTTool::setForceParent(bool forceParent)
    {
        _forceParent = forceParent;
    }
    
#pragma mark MAIN METHOD
    
    std::string JWTTool::buildJWTString()
    {
        
        //HEADER STRING------------------------------------------------------------------------------
        
        std::string sHeader = getHeaderString(getAppropriateAPIKey());
        
        
        //PAYLOAD STRING------------------------------------------------------------------------------
        
        std::string sBody = getBodyString();
        
        //SIGNATURE STRING----------------------------------------------------------------------------
        
        std::string sSignature = getJWTSignature(sHeader, sBody);
        
        //DISPLAYING DEBUG INFO-----------------------------------------------------------------------
        
//        cocos2d::log("\n\n\n apiSecret: %s\n\n\n", getAppropriateAPISecret().c_str());
        
        
        //CREATE THE FINAL JWT STRING-----------------------------------------------------------------
        
        std::string finalJWT = StringUtils::format("%s.%s.%s", sHeader.c_str(), sBody.c_str(), sSignature.c_str());
        
//        cocos2d::log("\n\n\n FINAL JWT STRING: %s\n\n\n", finalJWT.c_str());
        
        
        return finalJWT;
    }

#pragma mark HELPER METHODS
    
std::string JWTTool::getAppropriateAPISecret()
{
    if(_forceParent)
    {
        return ParentManager::getInstance()->getLoggedInParentApiSecret();
    }
    else
    {
        return ChildManager::getInstance()->getParentOrChildApiSecret();
    }
}
    
std::string JWTTool::getAppropriateAPIKey()
{
    if(_forceParent)
    {
        return ParentManager::getInstance()->getLoggedInParentApiKey();
    }
    else
    {
        return ChildManager::getInstance()->getParentOrChildApiKey();
    }
}
    
std::string JWTTool::getAppropriateUserId()
{
    if(_forceParent)
    {
        return ParentManager::getInstance()->getLoggedInParentId();
    }
    else
    {
        return ChildManager::getInstance()->getParentOrChildId();
    }
}
    
std::string JWTTool::getDateFormatString()
{
    time_t rawtime;
    struct tm * ptm;
    time ( &rawtime );
    ptm = gmtime ( &rawtime );
    
    std::string myDateTime = StringUtils::format("%d-%s-%sT%s:%s:%sZ", ptm->tm_year + 1900, addLeadingZeroToDateElement(ptm->tm_mon + 1).c_str(), addLeadingZeroToDateElement(ptm->tm_mday).c_str(), addLeadingZeroToDateElement(ptm->tm_hour).c_str(), addLeadingZeroToDateElement(ptm->tm_min).c_str(), addLeadingZeroToDateElement(ptm->tm_sec).c_str());
    
    return myDateTime;
}

std::string JWTTool::addLeadingZeroToDateElement(int input)
{
    std::string dateFormat = StringUtils::format("%d", input);
    if(dateFormat.length() == 1)
    {
        dateFormat = StringUtils::format("0%s", dateFormat.c_str());
    }
    
    return dateFormat;
}

std::string JWTTool::stringToLower(std::string input)
{
    for(int i = 0; i < input.length(); i++)
    {
        input[i] = tolower(input[i]);
    }
    
    return input;
}

std::string JWTTool::url_encode(const std::string &value) {
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

std::string JWTTool::getBase64Encoded(std::string input) //This is now used only to convert the first and second part of the jwt to base64, the HMAC SHA256 is already base64 encoded.
{
    char *output = NULL;
    cocos2d::base64Encode((unsigned char *)input.c_str(), (unsigned int)input.length(), &output);
    
    return output;
}

std::string JWTTool::getHeaderString(std::string kid)
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
    
//    cocos2d::log("\n\n\n Body string: %s\n\n\n", jsonString);
    
    std::string result = getBase64Encoded(StringUtils::format("%s", jsonString));
    
//    cocos2d::log("\n\n\n base64 encoded body: %s\n\n\n", result.c_str());

    return result;
}

std::string JWTTool::getBodySignature()
{
    std::string stringContentType = "";
    if(_requestBody.length() != 0)
    {
        stringContentType = StringUtils::format("content-type=%s&", url_encode(stringToLower("application/json;charset=UTF-8")).c_str());
    }
    
    std::string stringMandatoryHeaders = StringUtils::format("%shost=%s&x-az-req-datetime=%s", stringContentType.c_str(), url_encode(stringToLower(_host)).c_str(), url_encode(stringToLower(getDateFormatString())).c_str());
    
    std::string stringToBeEncoded = StringUtils::format("%s\n%s\n%s\n%s\n", _method.c_str(), url_encode(_path).c_str(), Net::getUrlParamsInAlphabeticalOrder(stringToLower(_queryParams)).c_str(), stringMandatoryHeaders.c_str());
    stringToBeEncoded += getBase64Encoded(_requestBody);
    std::string bodySignature = HMACSHA256::getInstance()->getHMACSHA256Hash(stringToBeEncoded, getAppropriateAPISecret());
    
    return bodySignature;
}

std::string JWTTool::getBodyString()
{
    rapidjson::StringBuffer s;
    rapidjson::Writer<rapidjson::StringBuffer> writer(s);
    
    writer.StartObject();
    
    writer.String("iss", (int)StringUtils::format("iss").length());
    writer.String(getAppropriateUserId().c_str(), (int)(StringUtils::format("%s", getAppropriateUserId().c_str()).length()));
    
    writer.String("aud", (int)StringUtils::format("aud").length());
    writer.String("", 0);
    
    writer.String("applicationClaim", (int)StringUtils::format("applicationClaim").length());
    
    writer.StartObject();
    
    writer.String("signature", (int)StringUtils::format("signature").length());
    writer.String(getBodySignature().c_str(), (int)getBodySignature().length());
    
    writer.String("parentKey", (int)StringUtils::format("parentKey").length());
    writer.String(ParentManager::getInstance()->getLoggedInParentApiKey().c_str(), (int)ParentManager::getInstance()->getLoggedInParentApiKey().length());
    
    writer.EndObject();
    
    writer.EndObject();
    
    
    const char *jsonString = s.GetString();
    
//    cocos2d::log("\n\n\n Body string: %s\n\n\n", jsonString);
    
    std::string result = getBase64Encoded(StringUtils::format("%s", jsonString));
    
//    cocos2d::log("\n\n\n base64 encoded body: %s\n\n\n", result.c_str());
    
    return result;
}

std::string JWTTool::getJWTSignature(std::string sHeader, std::string sBody)
{
    std::string unEncodedSignature = sHeader + "." + sBody;
    std::string encodedSignature = HMACSHA256::getInstance()->getHMACSHA256Hash(unEncodedSignature, getAppropriateAPISecret());
    
    return encodedSignature;
}
  
}
