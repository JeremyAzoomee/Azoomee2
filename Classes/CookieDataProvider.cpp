#include "CookieDataProvider.h"
#include "CookieDataParser.h"

using namespace cocos2d;

static CookieDataProvider *_sharedCookieDataProvider = NULL;

CookieDataProvider* CookieDataProvider::getInstance()
{
    if (! _sharedCookieDataProvider)
    {
        _sharedCookieDataProvider = new CookieDataProvider();
        _sharedCookieDataProvider->init();
    }
    
    return _sharedCookieDataProvider;
}

CookieDataProvider::~CookieDataProvider(void)
{
}

bool CookieDataProvider::init(void)
{
    return true;
}

std::string CookieDataProvider::getCookiesForRequest(std::string url)
{
    std::string cookieString = "";
    
    for(int i = 0; i < CookieDataParser::getInstance()->dataDownloadCookiesVector.size(); i++)
    {
        if(checkIfCookieIsForUrl(CookieDataParser::getInstance()->dataDownloadCookiesVector.at(i), url)) cookieString = StringUtils::format("%s%s; ", cookieString.c_str(), getCookieMainContent(CookieDataParser::getInstance()->dataDownloadCookiesVector.at(i)).c_str());
    }
    
    return cookieString;
}

std::vector<std::string> CookieDataProvider::splitStringToVector(std::string inputString, std::string separator)
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

bool CookieDataProvider::checkIfCookieIsForUrl(std::string cookieRecord, std::string url)
{
    bool domainFound = false;
    bool pathFound = false;
    
    std::vector<std::string> cookieParts = splitStringToVector(cookieRecord, "; ");
    for(int i = 0; i < cookieParts.size(); i++)
    {
        std::vector<std::string> partKeyAndValue = splitStringToVector(cookieParts.at(i), "=");
        if(partKeyAndValue.at(0) == "Domain")
        {
            if(url.find(partKeyAndValue.at(1)) != std::string::npos)
            {
                domainFound = true;
            }
        }
        else if(partKeyAndValue.at(0) == "Path")
        {
            if(url.find(partKeyAndValue.at(1)) != std::string::npos)
            {
                pathFound = true;
            }
        }
    }
    
    if(domainFound&&pathFound) return true;
    return false;
}

std::string CookieDataProvider::getCookieMainContent(std::string cookieRecord)
{
    std::string cookieMainContent = splitStringToVector(cookieRecord, "; ").at(0);
    
    return cookieMainContent;
}
