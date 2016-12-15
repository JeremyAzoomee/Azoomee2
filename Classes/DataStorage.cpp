#include "DataStorage.h"
#include "BackEndCaller.h"
#include "HQScene.h"

using namespace cocos2d;

static DataStorage *_sharedDataStorage = NULL;

DataStorage* DataStorage::getInstance()
{
    if (! _sharedDataStorage)
    {
        _sharedDataStorage = new DataStorage();
        _sharedDataStorage->init();
    }
    
    return _sharedDataStorage;
}

DataStorage::~DataStorage(void)
{
}

bool DataStorage::init(void)
{
    childProfile = false;
    return true;
}

bool DataStorage::parseParentLoginData(std::string responseData)
{
    childProfile = false;
    parentLoginData.Parse(responseData.c_str());
    
    if(parentLoginData.HasMember("code"))
    {
        if(parentLoginData["code"] != "INVALID_CREDENTIALS")
        {
            return true;
        }
    }
    
    return false;
}

std::vector<std::string> DataStorage::splitStringToVector(std::string inputString, std::string separator)
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

std::string DataStorage::getParentLoginValue(std::string keyName)
{
    return parentLoginData[keyName.c_str()].GetString();
}

std::string DataStorage::getParentOrChildLoginValue(std::string keyName) //This method selects a given key from the parent or kid login data weather there is kid login data or parent only.
{
    if(childProfile)
    {
        return childLoginData[keyName.c_str()].GetString();
    }
    else
    {
        return parentLoginData[keyName.c_str()].GetString();
    }
}

std::string DataStorage::getChildLoginValue(std::string keyName)
{
    return childLoginData[keyName.c_str()].GetString();
}

std::string DataStorage::getAvailableChildrenValue(std::string keyName)
{
    return availableChildrenData[keyName.c_str()].GetString();
}

int DataStorage::getAmountOfAvailableChildren()
{
    return (int)availableChildrenData.Size();
}

std::string DataStorage::getValueFromOneAvailableChild(int childNumber, std::string keyName)
{
    return availableChildrenData[childNumber][keyName.c_str()].GetString();
}

bool DataStorage::parseAvailableChildren(std::string responseData)
{
    availableChildrenData.Parse(responseData.c_str());
    return true;
}

bool DataStorage::parseChildLoginData(std::string responseData)
{
    childProfile = true;
    childLoginData.Parse(responseData.c_str());
    return true;
}

bool DataStorage::parseDownloadCookies(std::string responseString)
{
    //This is not a good aproach to parse strings by searching strings. We have to parse them by adding them to maps or vectors, otherwise we will have a browswer-dependent working method.
    //In this example as you can see, on android we have to substr one character earlier, as it gives back Set-Cookie:content instead of Set-Cookie: content ... Crazy...
    
    CCLOG("Responsestring: %s", responseString.c_str());
    
    size_t beginpos = responseString.find("Set-Cookie:");
    responseString = responseString.substr(beginpos);
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    responseString = responseString.substr(12);
#endif
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    responseString = responseString.substr(11);
#endif
    
    size_t endpos = responseString.find("\n");
    responseString = responseString.substr(0, endpos);
    
    pureCookieResponseString = responseString;
    
    //Now create the array of cookies based on ", " string
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    dataDownloadCookiesVector = splitStringToVector(pureCookieResponseString, ", ");
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    dataDownloadCookiesVector = splitStringToVector(pureCookieResponseString, ",");
#endif
    
    //dataDownloadCookiesWithCommas = responseString;
    responseString = replaceAll(responseString, ", ", "\n");
    dataDownloadCookiesForCpp = responseString;

    
    return true;
}

std::string DataStorage::replaceAll(std::string& str, const std::string& from, const std::string& to) {
    if(from.empty())
        return "";
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
    
    return str;
}

std::string DataStorage::getCookiesForRequest(std::string url)
{
    std::string cookieString = "";
    
    for(int i = 0; i < dataDownloadCookiesVector.size(); i++)
    {
        CCLOG("Cookie in array: %s", dataDownloadCookiesVector.at(i).c_str());
        if(checkIfCookieIsForUrl(dataDownloadCookiesVector.at(i), url)) cookieString = StringUtils::format("%s%s; ", cookieString.c_str(), getCookieMainContent(dataDownloadCookiesVector.at(i)).c_str());
    }
    
    CCLOG("\n\n\nCookieString:\n\n\n%s", cookieString.c_str());
    return cookieString;
}

bool DataStorage::checkIfCookieIsForUrl(std::string cookieRecord, std::string url)
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

std::string DataStorage::getCookieMainContent(std::string cookieRecord)
{
    std::string cookieMainContent = splitStringToVector(cookieRecord, "; ").at(0);
    
    return cookieMainContent;
}
