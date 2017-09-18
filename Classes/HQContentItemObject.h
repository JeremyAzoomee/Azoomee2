#ifndef Azoomee_HQContentItemObject_h
#define Azoomee_HQContentItemObject_h

#include <AzoomeeCommon/Azoomee.h>
#include <cocos/cocos2d.h>

NS_AZOOMEE_BEGIN

class HQContentItemObject
{
private:
    
    std::string title;
    std::string description;
    std::string type;
    std::string uri;
    bool entitled;
    bool newFlag;
    HQContentItemObject();
    
public:
    
    static HQContentItemObject create();
    
    void setTitle(const std::string &inputTitle);
    void setDescription(const std::string &inputDescription);
    void setType(const std::string &inputType);
    void setUri(const std::string &inputUri);
    void setEntitled(const bool inputEntitled);
    void setNewFlag(const bool inputNewFlag);
    
    std::string getTitle();
    std::string getDescription();
    std::string getType();
    std::string getUri();
    bool getEntitled();
    bool getNewFlag();
    
};

NS_AZOOMEE_END

#endif
