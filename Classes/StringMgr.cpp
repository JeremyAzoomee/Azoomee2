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


string StringMgr::getStringForKey(string key)
{
    return stringsDocument[key.c_str()].GetString();
}

string StringMgr::getErrorMessageForKey(string key)
{
    return errorMessagesDocument[key.c_str()].GetString();
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

