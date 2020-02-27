#ifndef TinizineCommon_LocaleManager_h
#define TinizineCommon_LocaleManager_h

#include <string>
#include "FileDownloader.h"
#include "FileZipUtil.h"
#include "StringFunctions.h"
#include "../Tinizine.h"

#include "cocos-ext.h"
using namespace rapidjson;

#define ERROR_TITLE "title"
#define ERROR_BODY "body"
#define ERROR_BUTTON "button"
#define ERROR_BUTTON_REFERENCE "optionalButtonRefNames"

#define _(string) LocaleManager::getInstance()->getStringForKey(string)

NS_TZ_BEGIN
	
class LanguageParams
{
public:
	LanguageParams(const std::string& identifier, const std::string& name, const std::string& text);
	std::string _identifier;
	std::string _name;
	std::string _text;
};
	
class LocaleManager : public FileDownloaderDelegate, FileZipDelegate
{
public:
	static const std::vector<LanguageParams> kLanguageParams;
	static const std::map<std::string, std::string> kDeviceLangConvMap;
    static const std::string kDefaultLanguageIdentifier;
    
    static LocaleManager* getInstance(void);
    virtual ~LocaleManager();
    
    std::string getStringForKey(const std::string& key);
    std::map<std::string, std::string> getErrorMessageWithCode(long errorCode);
	
	void changeLanguage(const std::string& languageID);
	std::string getLanguageID() const;
	
    void downloadRemoteStringsFiles(const std::string& url);
    
	// delegate functions
	void onAsyncUnzipComplete(bool success, const std::string& zipPath, const std::string& dirpath);
	void onFileDownloadComplete(const std::string& fileString, const std::string& tag, long responseCode);
	
private:
    bool init(void);
    
    Document _stringsDocument;
    Document _errorMessagesDocument;
	
	bool _remoteDataInitialised = false;
	FileDownloaderRef _langsZipDownloader = nullptr;
	
    std::string _languageID;
    void setLanguageIdentifier();
    Document parseFile(const std::string& languageID, const std::string& stringFile);

    bool keysExistInJson(const std::string& sceneID, const std::string& stringKey, const Document& inDocument);
    
    std::string getNestedStringFromJson(std::vector<std::string> jsonKeys, const rapidjson::Value& sceneJsonDictionary);
	
	std::string getLocalEtag() const;
	void setLocalEtag(const std::string& etag);
	
	void removeLocalLanguagesFiles();
	
    std::string _langsZipUrl;
	static const std::string kLanguagesDir;
	
};
  
NS_TZ_END

#endif
