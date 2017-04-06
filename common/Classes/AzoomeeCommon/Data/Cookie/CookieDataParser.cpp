#include "CookieDataParser.h"
#include "CookieDataStorage.h"

using namespace cocos2d;

namespace Azoomee
{

static CookieDataParser *_sharedCookieDataParser = NULL;

CookieDataParser* CookieDataParser::getInstance()
{
    if (! _sharedCookieDataParser)
    {
        _sharedCookieDataParser = new CookieDataParser();
        _sharedCookieDataParser->init();
    }
    
    return _sharedCookieDataParser;
}

CookieDataParser::~CookieDataParser(void)
{
}

bool CookieDataParser::init(void)
{
    return true;
}

bool CookieDataParser::parseDownloadCookies(std::string responseString)
{
    CCLOG("Responsestring: %s", responseString.c_str());
    
    size_t beginpos = responseString.find("Set-Cookie:");
    responseString = responseString.substr(beginpos);
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)         //response header on ios looks 1 char different than android header
    responseString = responseString.substr(12);
#endif
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    responseString = responseString.substr(11);
#endif
    
    size_t endpos = responseString.find("\n");
    responseString = responseString.substr(0, endpos);
    
    CookieDataStorage::getInstance()->pureCookieResponseString = responseString;
    
    //Now create the array of cookies based on ", " string
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    CookieDataStorage::getInstance()->dataDownloadCookiesVector = splitStringToVector(CookieDataStorage::getInstance()->pureCookieResponseString, ", ");
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    CookieDataStorage::getInstance()->dataDownloadCookiesVector = splitStringToVector(CookieDataStorage::getInstance()->pureCookieResponseString, ",");
#endif
    
    //dataDownloadCookiesWithCommas = responseString;
    responseString = replaceAll(responseString, ", ", "\n");
    CookieDataStorage::getInstance()->dataDownloadCookiesForCpp = responseString;
    
    
    return true;
}

std::vector<std::string> CookieDataParser::splitStringToVector(std::string inputString, std::string separator)
{
    std::vector<std::string> result;
    std::vector<std::string> tokens;
    size_t prev = 0, pos = 0;
    do
    {
        pos = inputString.find(separator, prev);
        if (pos == std::string::npos) pos = inputString.length();
        std::string token = inputString.substr(prev, pos - prev);
        if (!token.empty()) result.push_back(token);
        prev = pos + separator.length();
    }
    while (pos < inputString.length() && prev < inputString.length());
    
    return result;
}

std::string CookieDataParser::replaceAll(std::string& str, const std::string& from, const std::string& to) {
    if(from.empty())
        return "";
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
    
    return str;
}

}
