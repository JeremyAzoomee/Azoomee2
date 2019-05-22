#include "StringMgr.h"
#include "../Analytics/AnalyticsSingleton.h"
#include "StringFunctions.h"
#include "DirUtil.h"
#include "../Data/Json.h"
#include <cocos/cocos2d.h>

using namespace cocos2d;

namespace Azoomee
{
	
LanguageParams::LanguageParams(const std::string& identifier, const std::string& name, const std::string& text)
{
	_identifier = identifier;
	_name = name;
	_text = text;
}
	
const std::vector<LanguageParams> StringMgr::kLanguageParams = {
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

const std::map<std::string, std::string> StringMgr::kDeviceLangConvMap = {
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
	
const std::string StringMgr::kLanguagesDir = "languages/";
#ifdef USINGCI
	const std::string StringMgr::kLangsZipUrl = "https://media.azoomee.ninja/static/popups/languages/languages.zip";
#else
	const std::string StringMgr::kLangsZipUrl = "https://media.azoomee.com/static/popups/languages/languages.zip";
#endif
	
static StringMgr *_sharedStringMgr = NULL;

StringMgr* StringMgr::getInstance()
{
    if (! _sharedStringMgr)
    {
        _sharedStringMgr = new StringMgr();
        _sharedStringMgr->init();
    }
    
    return _sharedStringMgr;
}

StringMgr::~StringMgr(void)
{
}

bool StringMgr::init(void)
{
    setLanguageIdentifier();
    
    stringsDocument = parseFile(languageID, "strings");
	const std::string& fileContent = FileUtils::getInstance()->getStringFromFile("res/languages/errormessages.json");
	errorMessagesDocument.Parse(fileContent.c_str());
    //errorMessagesDocument = parseFile(languageID, "errormessages");
	
	const std::string& localDir = DirUtil::getCachesPath() + kLanguagesDir;
	if(!FileUtils::getInstance()->isDirectoryExist(localDir))
	{
		FileUtils::getInstance()->createDirectory(localDir);
	}
	
	_langsZipDownloader = FileDownloader::create();
	_langsZipDownloader->setDelegate(this);
	_langsZipDownloader->setEtag(getLocalEtag());
	_langsZipDownloader->downloadFileFromServer(kLangsZipUrl);
	
    return true;
}
	
void StringMgr::changeLanguage(const std::string &languageID)
{
	AnalyticsSingleton::getInstance()->registerLanguageCode(languageID);
	AnalyticsSingleton::getInstance()->languageChangedEvent(languageID);
	this->languageID = languageID;
	stringsDocument = parseFile(languageID, "strings");
	//errorMessagesDocument = parseFile(languageID, "errormessages");
	UserDefault::getInstance()->setStringForKey("language", languageID);
}
	
std::string StringMgr::getLanguageID() const
{
	return languageID;
}

//--------------- Get Strings Functions ------------------

std::string StringMgr::getStringForKey(std::string key)
{
    return getStringFromJson(key, stringsDocument, key);
}

std::map<std::string, std::string> StringMgr::getErrorMessageWithCode(long errorCode)
{
    std::string errorCodeString = StringUtils::format("%ld",errorCode);

    if(!errorMessagesDocument.HasMember(errorCodeString.c_str()))
        errorCodeString = "default";
    
    std::map<std::string, std::string> errorMap;
	
	const std::string& titleKey  = getNestedStringFromJson(std::vector<std::string>{errorCodeString,ERROR_TITLE}, errorMessagesDocument);
	const std::string& bodyKey = getNestedStringFromJson(std::vector<std::string>{errorCodeString,ERROR_BODY}, errorMessagesDocument);
	const std::string& buttonKey = getNestedStringFromJson(std::vector<std::string>{errorCodeString,ERROR_BUTTON}, errorMessagesDocument);
	const std::string& buttonRefKey = getNestedStringFromJson(std::vector<std::string>{errorCodeString,ERROR_BUTTON_REFERENCE}, errorMessagesDocument);
	
    errorMap[ERROR_TITLE] = getStringFromJson(titleKey, stringsDocument);
    errorMap[ERROR_BODY] = getStringFromJson(bodyKey, stringsDocument);
    errorMap[ERROR_BUTTON] = getStringFromJson(buttonKey, stringsDocument);
	errorMap[ERROR_BUTTON_REFERENCE] = buttonRefKey;

    
    return errorMap;
}

//------------- PRIVATE FUNCTIONS---------------

void StringMgr::setLanguageIdentifier()
{
	const std::string storedLang = UserDefault::getInstance()->getStringForKey("language", "");
	if(storedLang == "")
	{
		const std::string& deviceLang = ConfigStorage::getInstance()->getDeviceLanguage().substr(0,2);
		if(kDeviceLangConvMap.find(deviceLang) != kDeviceLangConvMap.end())
		{
			const auto& target = std::find_if(kLanguageParams.begin(), kLanguageParams.end(), [&](const LanguageParams& langParam){
				return langParam._identifier == kDeviceLangConvMap.at(deviceLang);
			});
			languageID = target != kLanguageParams.end() ? target->_identifier : kLanguageParams.at(0)._identifier;
		}
		else
		{
			languageID = kLanguageParams.at(0)._identifier;
		}
		UserDefault::getInstance()->setStringForKey("language",languageID);
		UserDefault::getInstance()->flush();
	}
	else
	{
		languageID = UserDefault::getInstance()->getStringForKey("language", kLanguageParams.at(0)._identifier);
	}
	
	AnalyticsSingleton::getInstance()->registerLanguageCode(languageID);
}

Document StringMgr::parseFile(std::string languageID, std::string stringFile)
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

std::string StringMgr::getNestedStringFromJson(std::vector<std::string> jsonKeys, rapidjson::Value& sceneJsonDictionary)
{
    std::string stringError = "Text not found.";
    
    if(jsonKeys.size() == 0 || !sceneJsonDictionary.IsObject())
    {
        AnalyticsSingleton::getInstance()->localisedStringErrorEvent("Error With Strings File",languageID);
        return stringError;
    }
    
    std::string currentKey = jsonKeys.at(0);
    
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
        return "";
    
    AnalyticsSingleton::getInstance()->localisedStringErrorEvent(currentKey,languageID);
    return stringError;
}

std::string StringMgr::getLocalEtag() const
{
	const std::string& etagFilePath = DirUtil::getCachesPath() + kLanguagesDir + "etag.txt";
	if(cocos2d::FileUtils::getInstance()->isFileExist(etagFilePath))
	{
		return cocos2d::FileUtils::getInstance()->getStringFromFile(etagFilePath);
	}
	return "";
}
void StringMgr::setLocalEtag(const std::string& etag)
{
	const std::string& etagFilePath = DirUtil::getCachesPath() + kLanguagesDir + "etag.txt";
	cocos2d::FileUtils::getInstance()->writeStringToFile(etag, etagFilePath);
}

void StringMgr::removeLocalLanguagesFiles()
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
	
void StringMgr::onAsyncUnzipComplete(bool success, const std::string& zipPath, const std::string& dirpath)
{
	FileUtils::getInstance()->removeFile(zipPath);
	if(success)
	{
		_remoteDataInitialised = true;
		parseFile(languageID, "strings");
		const std::string& fileContent = FileUtils::getInstance()->getStringFromFile(dirpath + "errormessages.json");
		errorMessagesDocument.Parse(fileContent.c_str());
	}
}

void StringMgr::onFileDownloadComplete(const std::string& fileString, const std::string& tag, long responseCode)
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
			stringsDocument = parseFile(languageID, "strings");
			const std::string& fileContent = FileUtils::getInstance()->getStringFromFile(baseLocation + "errormessages.json");
			errorMessagesDocument.Parse(fileContent.c_str());
		}
	}
}
  
}
