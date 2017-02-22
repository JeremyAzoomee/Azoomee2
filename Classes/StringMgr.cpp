#include "StringMgr.h"

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
    string languageID = getLanguageIdentifier();
    
    stringsDocument = parseFile(languageID, "strings");
    errorMessagesDocument = parseFile(languageID, "errormessages");

    return true;
}

//--------------- Get Strings Functions ------------------

string StringMgr::getStringForKey(string key)
{
    return getStringFromJson(splitStringToVector(key, "/"), stringsDocument);
}

std::map<std::string, std::string> StringMgr::getErrorMessageWithCode(long errorCode)
{
    string errorCodeString = StringUtils::format("%ld",errorCode);
    
    if(!errorMessagesDocument.HasMember(errorCodeString.c_str()))
        errorCodeString = "default";
    
    std::map<std::string, std::string> errorMap;
    
    errorMap[ERROR_TITLE] = getStringFromJson(std::vector<std::string>{errorCodeString,ERROR_TITLE}, errorMessagesDocument);
    errorMap[ERROR_BODY] = getStringFromJson(std::vector<std::string>{errorCodeString,ERROR_BODY}, errorMessagesDocument);
    errorMap[ERROR_BUTTON] = getStringFromJson(std::vector<std::string>{errorCodeString,ERROR_BUTTON}, errorMessagesDocument);
    
    return errorMap;
}

//------------- PRIVATE FUNCTIONS---------------

string StringMgr::getLanguageIdentifier()
{
    string languageID;
    
    switch(Application::getInstance()->getCurrentLanguage())
    {
        case cocos2d::LanguageType::ENGLISH:
            languageID = "en";
            break;
        default:
            languageID = "en";
            break;
    };
    
    return languageID;
}

Document StringMgr::parseFile(string languageID, string stringFile)
{
    string filename = StringUtils::format("res/languages/%s/%s.json",languageID.c_str(),stringFile.c_str());
    
    std::string fileContent = FileUtils::getInstance()->getStringFromFile(filename);
    Document document;
    document.Parse(fileContent.c_str());
    
    if(document.HasParseError())
    {
        CCLOG("Language file parsing error!");
    }
    
    return document;
}

std::vector<std::string> StringMgr::splitStringToVector(std::string inputString, std::string separator)
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

string StringMgr::getStringFromJson(std::vector<std::string> jsonKeys, rapidjson::Value& sceneJsonDictionary)
{
    string stringError = "";
    
    if(jsonKeys.size() == 0 || !sceneJsonDictionary.IsObject())
        return stringError;
    
    string currentKey = jsonKeys.at(0);
    
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
            return getStringFromJson(jsonKeys,sceneJsonDictionary[currentKey.c_str()]);
        }

    return stringError;
}
