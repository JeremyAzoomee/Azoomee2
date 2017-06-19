#ifndef AzoomeeCommon_StringMgr_h
#define AzoomeeCommon_StringMgr_h

#include <string>

//#include <cocos/cocos2d.h>
//#include "StringHashDefines.h"
//#include "ErrorCodeHashDefines.h"

//USING_NS_CC;

#include "cocos-ext.h"
using namespace rapidjson;

#define ERROR_TITLE "title"
#define ERROR_BODY "body"
#define ERROR_BUTTON "button"
#define ERROR_BUTTON_REFERENCE "optionalButtonRefNames"


namespace Azoomee
{

class StringMgr
{
public:
    /** Returns the shared instance of the Game Manager */
    static StringMgr* getInstance(void);
    virtual ~StringMgr();
    
    std::string getStringForKey(std::string key);
    std::map<std::string, std::string> getErrorMessageWithCode(long errorCode);
    
private:
    bool init(void);
    
    Document stringsDocument;
    Document errorMessagesDocument;
    
    std::string languageID;
    void setLanguageIdentifier();
    Document parseFile(std::string languageID, std::string stringFile);

    bool keysExistInJson(std::string sceneID, std::string stringKey, Document inDocument);
    
    std::string getStringFromJson(std::vector<std::string> jsonKeys, rapidjson::Value& sceneJsonDictionary);
    
};
  
}

#endif
