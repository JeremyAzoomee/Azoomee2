#ifndef Azoomee_HQContentItemObject_h
#define Azoomee_HQContentItemObject_h

#include "../../Azoomee.h"
#include <cocos/cocos2d.h>

NS_AZOOMEE_BEGIN

class HQContentItemObject
{
private:
    std::string contentItemId = "";
    std::string title = "";
    std::string description = "";
    std::string type = "";
    std::string uri = "";
    bool entitled = true;
    bool newFlag = false;
    
    //variables that are just added when read (not storing)
    std::string imagePath = "";
    int elementNumber = 0;
    cocos2d::Vec2 elementShape = cocos2d::Vec2(0,0);
    
public:
    HQContentItemObject();
    static HQContentItemObject create();
    
    void setContentItemId(const std::string &inputId);
    void setTitle(const std::string &inputTitle);
    void setDescription(const std::string &inputDescription);
    void setType(const std::string &inputType);
    void setUri(const std::string &inputUri);
    void setEntitled(const bool inputEntitled);
    void setNewFlag(const bool inputNewFlag);
    
    std::string getContentItemId();
    std::string getTitle();
    std::string getDescription();
    std::string getType();
    std::string getUri();
    bool getEntitled();
    bool getNewFlag();
    
    //getters and setters for variables only handled upon read (not storing)
    void setImagePath(const std::string &inputImagePath);
    void setElementNumber(int inputElementNumber);
    void setElementShape(const cocos2d::Vec2 &inputElementShape);
    
    std::string getImagePath();
    int getElementNumber();
    cocos2d::Vec2 getElementShape();
    
    //other helper methods
    std::string getJSONRepresentationOfStructure();
    static HQContentItemObject createFromMap(const std::map<std::string, std::string> &inputMap);
    
};

NS_AZOOMEE_END

#endif
