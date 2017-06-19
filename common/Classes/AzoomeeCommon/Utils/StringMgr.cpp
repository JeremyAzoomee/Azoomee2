#include "StringMgr.h"
#include "../Analytics/AnalyticsSingleton.h"
#include "StringFunctions.h"
#include <cocos/cocos2d.h>

using namespace cocos2d;

namespace Azoomee
{

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
    errorMessagesDocument = parseFile(languageID, "errormessages");

    return true;
}

//--------------- Get Strings Functions ------------------

std::string StringMgr::getStringForKey(std::string key)
{
    return getStringFromJson(splitStringToVector(key, "/"), stringsDocument);
}

std::map<std::string, std::string> StringMgr::getErrorMessageWithCode(long errorCode)
{
    std::string errorCodeString = StringUtils::format("%ld",errorCode);
    
    if(!errorMessagesDocument.HasMember(errorCodeString.c_str()))
        errorCodeString = "default";
    
    std::map<std::string, std::string> errorMap;
    
    errorMap[ERROR_TITLE] = getStringFromJson(std::vector<std::string>{errorCodeString,ERROR_TITLE}, errorMessagesDocument);
    errorMap[ERROR_BODY] = getStringFromJson(std::vector<std::string>{errorCodeString,ERROR_BODY}, errorMessagesDocument);
    errorMap[ERROR_BUTTON] = getStringFromJson(std::vector<std::string>{errorCodeString,ERROR_BUTTON}, errorMessagesDocument);
    errorMap[ERROR_BUTTON_REFERENCE] = getStringFromJson(std::vector<std::string>{errorCodeString,ERROR_BUTTON_REFERENCE}, errorMessagesDocument);

    
    return errorMap;
}

//------------- PRIVATE FUNCTIONS---------------

void StringMgr::setLanguageIdentifier()
{
    switch(Application::getInstance()->getCurrentLanguage())
    {
        case cocos2d::LanguageType::ENGLISH:
            languageID = "en-GB";
            break;
        default:
            languageID = "en-GB";
            break;
    };
}

Document StringMgr::parseFile(std::string languageID, std::string stringFile)
{
    std::string filename = StringUtils::format("res/languages/%s/%s.json",languageID.c_str(),stringFile.c_str());
    
    std::string fileContent = FileUtils::getInstance()->getStringFromFile(filename);
    Document document;
    document.Parse(fileContent.c_str());
    
    if(document.HasParseError())
    {
        CCLOG("Language file parsing error!");
    }
    
    return document;
}

std::string StringMgr::getStringFromJson(std::vector<std::string> jsonKeys, rapidjson::Value& sceneJsonDictionary)
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
            return getStringFromJson(jsonKeys,sceneJsonDictionary[currentKey.c_str()]);
        }

    //Due to Error Button Reference being optional
    if(currentKey == ERROR_BUTTON_REFERENCE)
        return "";
    
    AnalyticsSingleton::getInstance()->localisedStringErrorEvent(currentKey,languageID);
    return stringError;
}
  
}
