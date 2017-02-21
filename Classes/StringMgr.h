#ifndef StringMgr_h
#define StringMgr_h

#include <string>
using std::string;

#include "cocos2d.h"
USING_NS_CC;

#include "cocos-ext.h"
using namespace rapidjson;

class StringMgr
{
public:
    /** Returns the shared instance of the Game Manager */
    static StringMgr* getInstance(void);
    virtual ~StringMgr();
    bool init(void);
    
    string getStringForKey(string key);
    string getErrorMessageForKey(string key);
    
private:
    Document stringsDocument;
    Document errorMessagesDocument;
    
    string getLanguageIdentifier();
    Document parseFile(string languageID, string stringFile);
};
#endif
