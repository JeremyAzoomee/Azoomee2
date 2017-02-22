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
    //auto test1 = splitStringToVector(keys, "/");
    
    //test1.size()
    
    
    
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
    //rapidjson::Value& V = stringsDocument["LoginScene"];
    //auto test = V[keys.c_str()].GetString();
    //std:string myvalue = stringsDocument["LoginScene"][key.c_str()].GetString();
    //GetString();
    
    return getStringFromJson(splitStringToVector(keys, "/"), stringsDocument);
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

/*bool StringMgr::keysExistInJson(string sceneID, string stringKey, Document inDocument)
{
    bool keyExists = true;
    
    if(inDocument.HasMember(sceneID.c_str()))
        {
            if (inDocument[sceneID.c_str()].IsNull())
                keyExists = false;
            
            rapidjson::Value& sceneJsonDictionary = stringsDocument[sceneID.c_str()];
            
            if(sceneJsonDictionary.HasMember(stringKey.c_str()))
            {
                if(!sceneJsonDictionary[stringKey.c_str()].IsString())
                    keyExists = false;
                if(!sceneJsonDictionary[stringKey.c_str()].IsNull())
                    keyExists = false;
            }
            else
                keyExists = false;
        }
    else
        keyExists = false;
    
    return keyExists;
}

string StringMgr::getStringFromJson(string keys, Document inDocument)
{
    std::vector<std::string> jsonKeys = splitStringToVector(keys, "/");
    
    rapidjson::Value& sceneJsonDictionary = inDocument;
    
    for(int i =0; i < jsonKeys.size();i++)
    {
        if(sceneJsonDictionary.HasMember(jsonKeys.at(i).c_str()))
           {
               if (inDocument[jsonKeys.at(i).c_str()].IsNull())
                   break;
               else
               {
                   
               }
           }
        
    }
    
    bool keyExists = true;
    
    if(inDocument.HasMember(sceneID.c_str()))
    {
        if (inDocument[sceneID.c_str()].IsNull())
            keyExists = false;
        
        rapidjson::Value& sceneJsonDictionary = stringsDocument[sceneID.c_str()];
        
        if(sceneJsonDictionary.HasMember(stringKey.c_str()))
        {
            if(!sceneJsonDictionary[stringKey.c_str()].IsString())
                keyExists = false;
            if(!sceneJsonDictionary[stringKey.c_str()].IsNull())
                keyExists = false;
        }
        else
            keyExists = false;
    }
    else
        keyExists = false;
    
    return keyExists;
}

*/

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
