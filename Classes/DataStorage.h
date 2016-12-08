#include "cocos2d.h"
#include "external/json/document.h"

class DataStorage : public cocos2d::Object
{
    
public:
    /** Returns the shared instance of the Game Manager */
    static DataStorage* getInstance(void);
    
public:
    virtual ~DataStorage();
    bool init(void);
    
    //Parent area
    rapidjson::Document parentLoginData;
    bool parseParentLoginData(std::string responseData);
    std::string getParentLoginValue(std::string keyName);
    
    //Child area
    rapidjson::Document availableChildrenData;
    rapidjson::Document childLoginData;
    bool parseAvailableChildren(std::string responseData);
    bool parseChildLoginData(std::string responseData);
    std::string getChildLoginValue(std::string keyName);
    std::string getAvailableChildrenValue(std::string keyName);
    int getAmountOfAvailableChildren();
    std::string getValueFromOneAvailableChild(int childNumber, std::string keyName);
    
    //Quick switch to see if currently only parent or parent + kid logged in
    bool childProfile;
    std::string getParentOrChildLoginValue(std::string keyName);
    
    //Content area
    rapidjson::Document contentData;
    std::vector<std::string> contentDataJsonKeys;
    bool parseMainHubContentData(std::string responseString);
    std::vector<std::map<std::string, std::string>> getMainHubDataForGivenType(std::string type);
    
    bool parseDownloadCookies(std::string responseString);
    
    std::vector<std::string> dataDownloadCookiesVector;
    std::string pureCookieResponseString;
    //std::string dataDownloadCookiesWithCommas;
    std::string dataDownloadCookiesForCpp;
    
    std::string replaceAll(std::string& str, const std::string& from, const std::string& to);
    
    std::string getCookiesForRequest(std::string url);
    
    std::vector<std::string> splitStringToVector(std::string inputString, std::string separator);
    bool checkIfCookieIsForUrl(std::string cookieRecord, std::string url);
    std::string getCookieMainContent(std::string cookieRecord);
    
    std::vector<std::map<std::string, std::string>> mainHubElements;
    
};
