//
//  CookieManager.cpp
//  AzoomeeCommon
//
//  Created by Macauley on 05/06/2019.
//

#include "CookieManager.h"
#include "../../Utils/StringFunctions.h"
#include "../ConfigStorage.h"

NS_AZOOMEE_BEGIN

static std::auto_ptr<CookieManager> sCookieManagerSharedInstance;

CookieManager* CookieManager::getInstance()
{
	if(!sCookieManagerSharedInstance.get())
	{
		sCookieManagerSharedInstance.reset(new CookieManager());
	}
	return sCookieManagerSharedInstance.get();
}

CookieManager::~CookieManager()
{
	
}

std::string CookieManager::getCookiesForRequest(const std::string& url)
{
	std::string cookieString = "";
	
	for(int i = 0; i < _dataDownloadCookiesVector.size(); i++)
	{
		if(checkIfCookieIsForUrl(_dataDownloadCookiesVector.at(i), url))
		{
			cookieString += getCookieMainContent(_dataDownloadCookiesVector.at(i)) + "; ";
		}
	}
	
	return cookieString;
}

bool CookieManager::checkIfCookieIsForUrl(const std::string& cookieRecord, const std::string& url)
{
	bool domainFound = false;
	bool pathFound = false;
	
	std::vector<std::string> cookieParts = StringFunctions::splitStringToVector(cookieRecord, "; ");
	for(int i = 0; i < cookieParts.size(); i++)
	{
		std::vector<std::string> partKeyAndValue = StringFunctions::splitStringToVector(cookieParts.at(i), "=");
		if(partKeyAndValue.at(0) == "Domain")
		{
			if(partKeyAndValue.size() > 1 && url.find(partKeyAndValue.at(1)) != std::string::npos)
			{
				domainFound = true;
			}
		}
		else if(partKeyAndValue.at(0) == "Path")
		{
			if(partKeyAndValue.size() > 1 && url.find(partKeyAndValue.at(1)) != std::string::npos)
			{
				pathFound = true;
			}
		}
	}
	
	if(domainFound&&pathFound)
	{
		return true;
	}
	
	return false;
}

std::string CookieManager::getCookieMainContent(const std::string& cookieRecord)
{
	return StringFunctions::splitStringToVector(cookieRecord, "; ").at(0);
}

std::string CookieManager::getUrlWithPathFromCookie(const std::string& cookieString)
{
	std::string domain = "";
	std::string path = "";
	
	std::vector<std::string> cookieParts = StringFunctions::splitStringToVector(cookieString, "; ");
	for(int i = 0; i < cookieParts.size(); i++)
	{
		std::string currentPart = cookieParts.at(i);
		if(currentPart.find("Domain=") != std::string::npos)
		{
			domain = StringFunctions::splitStringToVector(currentPart, "=").at(1);
		}
		else if(currentPart.find("Path=") != std::string::npos)
		{
			path = StringFunctions::splitStringToVector(currentPart, "=").at(1);
		}
	}
	
	return ConfigStorage::getInstance()->getMediaPrefixForXwalkCookies() + domain + path;
}

std::string CookieManager::getDomainFromCookie(const std::string& cookieString)
{
	std::string domain = "";
	
	std::vector<std::string> cookieParts = StringFunctions::splitStringToVector(cookieString, "; ");
	for(int i = 0; i < cookieParts.size(); i++)
	{
		std::string currentPart = cookieParts.at(i);
		if(currentPart.find("Domain=") != std::string::npos)
		{
			domain = StringFunctions::splitStringToVector(currentPart, "=").at(1);
		}
	}
	
	return ConfigStorage::getInstance()->getMediaPrefixForXwalkCookies() + domain;
}

std::string CookieManager::getAllCookiesInJson()
{
	std::vector<std::map<std::string, std::string>> allCookies;
	
	for(int i = 0; i < _dataDownloadCookiesVector.size(); i++)
	{
		const std::string& cookieStr = _dataDownloadCookiesVector.at(i);
		std::map<std::string, std::string> cookieRecord;
		cookieRecord["url"] = getUrlWithPathFromCookie(cookieStr);
		cookieRecord["cookie"] = cookieStr;
		allCookies.push_back(cookieRecord);
	}
	
//	cocos2d::log("COOKIE JSON: %s", getJSONStringFromVectorOfMaps(allCookies).c_str());
	
	return StringFunctions::getJSONStringFromVectorOfMaps(allCookies);
}

bool CookieManager::parseDownloadCookies(const std::string& responseString)
{
//	cocos2d::log("Responsestring: %s", responseString.c_str());
	
	std::string cookieStr = responseString;
	
	size_t beginpos = cookieStr.find("Set-Cookie:");
	cookieStr = cookieStr.substr(beginpos);
	
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)         //response header on ios looks 1 char different than android header
	cookieStr = cookieStr.substr(12);
#endif
	
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	cookieStr = cookieStr.substr(11);
#endif
	
	size_t endpos = cookieStr.find("\n");
	cookieStr = cookieStr.substr(0, endpos);
	
	_pureCookieResponseString = cookieStr;
	
	//Now create the array of cookies based on ", " string
	
	_dataDownloadCookiesVector.clear();
	
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	_dataDownloadCookiesVector = StringFunctions::splitStringToVector(_pureCookieResponseString, ", ");
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	_dataDownloadCookiesVector = StringFunctions::splitStringToVector(_pureCookieResponseString, ",");
#endif
	
	return true;
}


NS_AZOOMEE_END
