#ifndef Azoomee_HQContentItemObject_h
#define Azoomee_HQContentItemObject_h

#include "../../Azoomee.h"
#include <cocos/cocos2d.h>

NS_AZOOMEE_BEGIN

class HQContentItemObject;
typedef std::shared_ptr<HQContentItemObject> HQContentItemObjectRef;

class HQContentItemObject
{
private:
    std::string _contentItemId = "";
    std::string _title = "";
    std::string _description = "";
    std::string _type = "";
    std::string _uri = "";
    bool _entitled = true;
    std::vector<std::string> _tags;
    std::map<std::string, std::string> _images;
    
    //variables that are just added when read (not storing)
    std::string _imagePath = "";
    int _elementNumber = 0;
    cocos2d::Vec2 _elementShape = cocos2d::Vec2(0,0);
    
public:
    HQContentItemObject();
    static HQContentItemObjectRef create();
    
    void setContentItemId(const std::string &inputId);
    void setTitle(const std::string &inputTitle);
    void setDescription(const std::string &inputDescription);
    void setType(const std::string &inputType);
    void setUri(const std::string &inputUri);
    void setEntitled(const bool inputEntitled);
    void addTag(const std::string &tag);
    void addImage(const std::string &key, const std::string &value);
    
    std::string getContentItemId() const;
    std::string getTitle() const;
    std::string getDescription() const;
    std::string getType() const;
    std::string getUri() const;
    bool isEntitled() const;
    bool isNew() const;
    std::vector<std::string> getTags() const;
    std::map<std::string, std::string> getImages() const;
    
    //getters and setters for variables only handled upon read (not storing)
    void setImagePath(const std::string &inputImagePath);
    void setElementNumber(int inputElementNumber);
    void setElementShape(const cocos2d::Vec2 &inputElementShape);
    
    std::string getImagePath() const;
    int getElementNumber() const;
    cocos2d::Vec2 getElementShape();
    
    //other helper methods
    std::string getJSONRepresentationOfStructure() const;
    static HQContentItemObjectRef createFromMap(const std::map<std::string, std::string> &inputMap);
    
};

NS_AZOOMEE_END

#endif
