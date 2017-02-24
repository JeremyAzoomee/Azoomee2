#ifndef StringMgr_h
#define StringMgr_h

#include <string>
using std::string;

#include "cocos2d.h"
#include "StringHashDefines.h"
#include "ErrorCodeHashDefines.h"

USING_NS_CC;

#define ERROR_TITLE "title"
#define ERROR_BODY "body"
#define ERROR_BUTTON "button"

#include "cocos-ext.h"
using namespace rapidjson;

class StringMgr
{
public:
    /** Returns the shared instance of the Game Manager */
    static StringMgr* getInstance(void);
    virtual ~StringMgr();
    
    string getStringForKey(string key);
    std::map<std::string, std::string> getErrorMessageWithCode(long errorCode);
    
private:
    bool init(void);
    
    Document stringsDocument;
    Document errorMessagesDocument;
    
    string getLanguageIdentifier();
    Document parseFile(string languageID, string stringFile);

    bool keysExistInJson(string sceneID, string stringKey, Document inDocument);
    
    string getStringFromJson(std::vector<std::string> jsonKeys, rapidjson::Value& sceneJsonDictionary);
    
};
#endif
