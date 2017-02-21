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
    
    string getStringForKeys(string keys);
    string getErrorMessageWithCode(long errorCode);
    
private:
    Document stringsDocument;
    Document errorMessagesDocument;
    
    string getLanguageIdentifier();
    Document parseFile(string languageID, string stringFile);
    
    std::vector<std::string> splitStringToVector(std::string inputString, std::string separator);
    
};
#endif
