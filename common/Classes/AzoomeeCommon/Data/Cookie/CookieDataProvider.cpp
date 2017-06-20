#include "CookieDataProvider.h"
#include "CookieDataStorage.h"
#include "../../Utils/StringFunctions.h"
#include "../ConfigStorage.h"

using namespace cocos2d;

namespace Azoomee
{

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
    
    for(int i = 0; i < CookieDataStorage::getInstance()->dataDownloadCookiesVector.size(); i++)
    {
        if(checkIfCookieIsForUrl(CookieDataStorage::getInstance()->dataDownloadCookiesVector.at(i), url)) cookieString += getCookieMainContent(CookieDataStorage::getInstance()->dataDownloadCookiesVector.at(i)) + "; ";
    }
    
    return cookieString;
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
    return splitStringToVector(cookieRecord, "; ").at(0);
}
    
std::string CookieDataProvider::getUrlWithPathFromCookie(std::string cookieString)
{
    std::string domain = "";
    std::string path = "";
    
    std::vector<std::string> cookieParts = splitStringToVector(cookieString, "; ");
    for(int i = 0; i < cookieParts.size(); i++)
    {
        std::string currentPart = cookieParts.at(i);
        if(currentPart.find("Domain=") != std::string::npos)
        {
            domain = splitStringToVector(currentPart, "=").at(1);
        }
        else if(currentPart.find("Path=") != std::string::npos)
        {
            path = splitStringToVector(currentPart, "=").at(1);
        }
    }
    
    return ConfigStorage::getInstance()->getMediaPrefixForXwalkCookies() + domain + path;
}
    
std::string CookieDataProvider::getDomainFromCookie(std::string cookieString)
{
    std::string domain = "";
    
    std::vector<std::string> cookieParts = splitStringToVector(cookieString, "; ");
    for(int i = 0; i < cookieParts.size(); i++)
    {
        std::string currentPart = cookieParts.at(i);
        if(currentPart.find("Domain=") != std::string::npos)
        {
            domain = splitStringToVector(currentPart, "=").at(1);
        }
    }
    
    return ConfigStorage::getInstance()->getMediaPrefixForXwalkCookies() + domain;
}
    
std::string CookieDataProvider::getAllCookiesInJson()
{
    std::vector<std::map<std::string, std::string>> allCookies;
    
    for(int i = 0; i < CookieDataStorage::getInstance()->dataDownloadCookiesVector.size(); i++)
    {
        std::map<std::string, std::string> cookieRecord;
        cookieRecord["url"] = getUrlWithPathFromCookie(CookieDataStorage::getInstance()->dataDownloadCookiesVector.at(i));
        cookieRecord["cookie"] = CookieDataStorage::getInstance()->dataDownloadCookiesVector.at(i);
        allCookies.push_back(cookieRecord);
    }
    
    return getJSONStringFromVectorOfMaps(allCookies);
}
  
}
