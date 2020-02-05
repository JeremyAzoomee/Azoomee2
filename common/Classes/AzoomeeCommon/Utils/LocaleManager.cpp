#include "LocaleManager.h"
#include "../Analytics/AnalyticsSingleton.h"
#include "StringFunctions.h"
#include "DirUtil.h"
#include "../Data/Json.h"
#include "../Device.h"
#include <cocos/cocos2d.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN
	
LanguageParams::LanguageParams(const std::string& identifier, const std::string& name, const std::string& text)
{
	_identifier = identifier;
	_name = name;
	_text = text;
}
	
const std::vector<LanguageParams> LocaleManager::kLanguageParams = {
	LanguageParams("en-GB", "English", "Hello!"),
	LanguageParams("afr", "Afrikaans", "Hallo!"),
	LanguageParams("fre-FR", "Français", "Bonjour!"),
	LanguageParams("spa-ES", "Español", "¡Hola!"),
	LanguageParams("ger-DE", "Deutsch", "Hallo!"),
	LanguageParams("por-PT", "Português", "Olá!"),
	LanguageParams("ita-IT", "Italiano", "Ciao!"),
	LanguageParams("gre", "Ελληνικά", "Γειά σου!"),
	LanguageParams("tur", "Türk", "Merhaba!")
};

const std::map<std::string, std::string> LocaleManager::kDeviceLangConvMap = {
	{"en", "en-GB"},
	{"af", "afr"},
	{"fr", "fre-FR"},
	{"es", "spa-ES"},
	{"de", "ger-DE"},
	{"pt", "por-PT"},
	{"it", "ita-IT"},
	{"el", "gre"},
	{"tr", "tur"},
};

const std::string LocaleManager::kDefaultLanguageIdentifier = kLanguageParams.at(0)._identifier;
    
const std::string LocaleManager::kLanguagesDir = "languages/";

	
static LocaleManager *_sharedLocaleManager = NULL;

LocaleManager* LocaleManager::getInstance()
{
    if (! _sharedLocaleManager)
    {
        _sharedLocaleManager = new LocaleManager();
        _sharedLocaleManager->init();
    }
    return _sharedLocaleManager;
}

LocaleManager::~LocaleManager(void)
{
}

bool LocaleManager::init(void)
{
    setLanguageIdentifier();
    
    _stringsDocument = parseFile(_languageID, "strings");
	const std::string& fileContent = FileUtils::getInstance()->getStringFromFile("res/languages/errormessages.json");
	_errorMessagesDocument.Parse(fileContent.c_str());
	
	const std::string& localDir = DirUtil::getCachesPath() + kLanguagesDir;
	if(!FileUtils::getInstance()->isDirectoryExist(localDir))
	{
		FileUtils::getInstance()->createDirectory(localDir);
	}
	
	_langsZipDownloader = FileDownloader::create();
	_langsZipDownloader->setDelegate(this);
	
    return true;
}

void LocaleManager::downloadRemoteStringsFiles(const std::string& url)
{
    _langsZipUrl = url;
    _langsZipDownloader->setEtag(getLocalEtag());
    _langsZipDownloader->downloadFileFromServer(_langsZipUrl);
}

void LocaleManager::changeLanguage(const std::string &languageID)
{
	AnalyticsSingleton::getInstance()->registerLanguageCode(languageID);
	AnalyticsSingleton::getInstance()->languageChangedEvent(languageID);
	_languageID = languageID;
	_stringsDocument = parseFile(languageID, "strings");
	UserDefault::getInstance()->setStringForKey("language", languageID);
}
	
std::string LocaleManager::getLanguageID() const
{
	return _languageID;
}

//--------------- Get Strings Functions ------------------

std::string LocaleManager::getStringForKey(const std::string& key)
{
    return getStringFromJson(key, _stringsDocument, key);
}

std::map<std::string, std::string> LocaleManager::getErrorMessageWithCode(long errorCode)
{
    std::string errorCodeString = StringUtils::format("%ld",errorCode);

    if(!_errorMessagesDocument.HasMember(errorCodeString.c_str()))
    {
        errorCodeString = "default";
    }
    std::map<std::string, std::string> errorMap;
	
    const rapidjson::Value& errorStrings = _errorMessagesDocument[errorCodeString.c_str()];
    
    const std::string& titleKey  = getStringFromJson(ERROR_TITLE, errorStrings);//getNestedStringFromJson(std::vector<std::string>{errorCodeString,ERROR_TITLE}, _errorMessagesDocument);
	const std::string& bodyKey = getStringFromJson(ERROR_BODY, errorStrings);//getNestedStringFromJson(std::vector<std::string>{errorCodeString,ERROR_BODY}, _errorMessagesDocument);
	const std::string& buttonKey = getStringFromJson(ERROR_BUTTON, errorStrings);//getNestedStringFromJson(std::vector<std::string>{errorCodeString,ERROR_BUTTON}, _errorMessagesDocument);
	const std::string& buttonRefKey = getStringFromJson(ERROR_BUTTON_REFERENCE, errorStrings);//getNestedStringFromJson(std::vector<std::string>{errorCodeString,ERROR_BUTTON_REFERENCE}, _errorMessagesDocument);
	
    errorMap[ERROR_TITLE] = getStringFromJson(titleKey, _stringsDocument);
    errorMap[ERROR_BODY] = getStringFromJson(bodyKey, _stringsDocument);
    errorMap[ERROR_BUTTON] = getStringFromJson(buttonKey, _stringsDocument);
	errorMap[ERROR_BUTTON_REFERENCE] = buttonRefKey;

    
    return errorMap;
}

//------------- PRIVATE FUNCTIONS---------------

void LocaleManager::setLanguageIdentifier()
{
	const std::string storedLang = UserDefault::getInstance()->getStringForKey("language", "");
	if(storedLang == "")
	{
		const std::string& deviceLang = Device::getInstance()->getDeviceLanguage().substr(0,2);
		if(kDeviceLangConvMap.find(deviceLang) != kDeviceLangConvMap.end())
		{
			const auto& target = std::find_if(kLanguageParams.begin(), kLanguageParams.end(), [&](const LanguageParams& langParam){
				return langParam._identifier == kDeviceLangConvMap.at(deviceLang);
			});
			_languageID = target != kLanguageParams.end() ? target->_identifier : kDefaultLanguageIdentifier;
		}
		else
		{
			_languageID = kDefaultLanguageIdentifier;
		}
		UserDefault::getInstance()->setStringForKey("language",_languageID);
		UserDefault::getInstance()->flush();
	}
	else
	{
		_languageID = UserDefault::getInstance()->getStringForKey("language", kDefaultLanguageIdentifier);
	}
	
	AnalyticsSingleton::getInstance()->registerLanguageCode(_languageID);
}

Document LocaleManager::parseFile(const std::string& languageID, const std::string& stringFile)
{
	const std::string& baseDir = _remoteDataInitialised ? DirUtil::getCachesPath() + kLanguagesDir : "res/languages/";
	const std::string& filename = baseDir + languageID + "/" + stringFile + ".json";
    
    const std::string& fileContent = FileUtils::getInstance()->getStringFromFile(filename);
    Document document;
    document.Parse(fileContent.c_str());
	
    if(document.HasParseError())
    {
        cocos2d::log("Language file parsing error!");
    }
    
    return document;
}

std::string LocaleManager::getNestedStringFromJson(std::vector<std::string> jsonKeys, const rapidjson::Value& sceneJsonDictionary)
{
    const std::string& stringError = "Text not found.";
    
    if(jsonKeys.size() == 0 || !sceneJsonDictionary.IsObject())
    {
        AnalyticsSingleton::getInstance()->localisedStringErrorEvent("Error With Strings File",_languageID);
        return stringError;
    }
    
    const std::string& currentKey = jsonKeys.at(0);
    
    if(jsonKeys.size() == 1)
    {
        if(sceneJsonDictionary.HasMember(currentKey.c_str()))
        {
            if(sceneJsonDictionary[currentKey.c_str()].IsString() && !sceneJsonDictionary[currentKey.c_str()].IsNull())
                return sceneJsonDictionary[currentKey.c_str()].GetString();
        }
    }
    else if(sceneJsonDictionary.HasMember(currentKey.c_str()) && !sceneJsonDictionary[currentKey.c_str()].IsNull())
        {
            jsonKeys.erase(jsonKeys.begin() + 0);
            return getNestedStringFromJson(jsonKeys,sceneJsonDictionary[currentKey.c_str()]);
        }

    //Due to Error Button Reference being optional
    if(currentKey == ERROR_BUTTON_REFERENCE)
    {
        return "";
    }
    
    AnalyticsSingleton::getInstance()->localisedStringErrorEvent(currentKey,_languageID);
    return stringError;
}

std::string LocaleManager::getLocalEtag() const
{
	const std::string& etagFilePath = DirUtil::getCachesPath() + kLanguagesDir + "etag.txt";
	if(cocos2d::FileUtils::getInstance()->isFileExist(etagFilePath))
	{
		return cocos2d::FileUtils::getInstance()->getStringFromFile(etagFilePath);
	}
	return "";
}
void LocaleManager::setLocalEtag(const std::string& etag)
{
	const std::string& etagFilePath = DirUtil::getCachesPath() + kLanguagesDir + "etag.txt";
	cocos2d::FileUtils::getInstance()->writeStringToFile(etag, etagFilePath);
}

void LocaleManager::removeLocalLanguagesFiles()
{
	const std::string& baseLocation = DirUtil::getCachesPath() + kLanguagesDir;
	const std::vector<std::string>& langsFolders = DirUtil::getFoldersInDirectory(baseLocation);
	for(const std::string& folder : langsFolders)
	{
		if(folder.size() > 2)
		{
			FileUtils::getInstance()->removeDirectory(baseLocation + folder);
		}
	}
	FileUtils::getInstance()->removeFile(baseLocation + "errormessages.json");
	
}
	
// Delegate functions
	
void LocaleManager::onAsyncUnzipComplete(bool success, const std::string& zipPath, const std::string& dirpath)
{
	FileUtils::getInstance()->removeFile(zipPath);
	if(success)
	{
		_remoteDataInitialised = true;
		parseFile(_languageID, "strings");
		const std::string& fileContent = FileUtils::getInstance()->getStringFromFile(dirpath + "errormessages.json");
		_errorMessagesDocument.Parse(fileContent.c_str());
	}
}

void LocaleManager::onFileDownloadComplete(const std::string& fileString, const std::string& tag, long responseCode)
{
	if(responseCode == 200)
	{
		const std::string& baseLocation = DirUtil::getCachesPath() + kLanguagesDir;
		const std::string& zipLoc = baseLocation + "langs.zip";
		bool success = FileUtils::getInstance()->writeStringToFile(fileString, zipLoc);
		removeLocalLanguagesFiles();
		FileZipUtil::getInstance()->asyncUnzip(zipLoc, baseLocation, "", this);
		setLocalEtag(_langsZipDownloader->getEtag());
		_langsZipDownloader->setDelegate(nullptr);
		_langsZipDownloader = nullptr;
	}
	else if(responseCode == 304)
	{
		const std::string& baseLocation = DirUtil::getCachesPath() + kLanguagesDir;
		const std::vector<std::string>& langsFolders = DirUtil::getFoldersInDirectory(baseLocation);
		if(langsFolders.size() > 0)
		{
			_remoteDataInitialised = true;
			_stringsDocument = parseFile(_languageID, "strings");
			const std::string& fileContent = FileUtils::getInstance()->getStringFromFile(baseLocation + "errormessages.json");
			_errorMessagesDocument.Parse(fileContent.c_str());
		}
	}
}
  
NS_AZOOMEE_END
