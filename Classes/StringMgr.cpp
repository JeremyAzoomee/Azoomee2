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


string StringMgr::getStringForKeys(string keys)
{
    auto test1 = splitStringToVector(keys, "/");
    
    
    
    /*int jsonDataLength = stringsDocument.Size();
    int loginSceneLength = stringsDocument["LoginScene"].Size();
    
    rapidjson::Value::MemberIterator M;
    const char *key1;
    
    for (M=stringsDocument["LoginScene"].MemberBegin(); M!=stringsDocument["LoginScene"].MemberEnd(); M++)
    {
        key1   = M->name.GetString();
        CCLOG("actual key: %s", key1);
    }
     
     HasMember("code"))
     
     IsNull();
     .IsString())
     
     
    */
    rapidjson::Value& V = stringsDocument["LoginScene"];
    auto test = V[keys.c_str()].GetString();
    //std:string myvalue = stringsDocument["LoginScene"][key.c_str()].GetString();
    //GetString();
    
    return stringsDocument[keys.c_str()].GetString();
}

string StringMgr::getErrorMessageWithCode(long errorCode)
{
    return "";
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
