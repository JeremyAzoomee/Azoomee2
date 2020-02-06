//
//  CookieManager.h
//  AzoomeeCommon
//
//  Created by Macauley on 05/06/2019.
//

#ifndef CookieManager_h
#define CookieManager_h

#include <cocos/cocos2d.h>
#include "../Json.h"
#include "../../Azoomee.h"

NS_AZOOMEE_BEGIN

class CookieManager
{
private:
    static const std::string kMediaCookiePrefix;
    
	std::vector<std::string> _dataDownloadCookiesVector;
	std::string _pureCookieResponseString;
	
	bool checkIfCookieIsForUrl(const std::string& cookieRecord, const std::string& url);
	std::string getUrlWithPathFromCookie(const std::string& cookieString);
	std::string getDomainFromCookie(const std::string& cookieString);
	
public:
	static CookieManager* getInstance();
	virtual ~CookieManager();
	
	std::string getCookiesForRequest(const std::string& url);
	std::string getCookieMainContent(const std::string& cookieRecord);
	std::string getAllCookiesInJson();
	
	bool parseDownloadCookies(const std::string& responseString);
	
};

NS_AZOOMEE_END

#endif /* CookieManager_h */
