#include "CookieDataParser.h"
#include "CookieDataStorage.h"
#include "../../Utils/StringFunctions.h"

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
    
    CookieDataStorage::getInstance()->dataDownloadCookiesVector.clear();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    CookieDataStorage::getInstance()->dataDownloadCookiesVector = splitStringToVector(CookieDataStorage::getInstance()->pureCookieResponseString, ", ");
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    CookieDataStorage::getInstance()->dataDownloadCookiesVector = splitStringToVector(CookieDataStorage::getInstance()->pureCookieResponseString, ",");
#endif
    
    return true;
}

}
