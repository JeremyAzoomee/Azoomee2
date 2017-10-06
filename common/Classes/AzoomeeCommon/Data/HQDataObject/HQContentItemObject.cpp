#include "HQContentItemObject.h"
#include "../../Utils/StringFunctions.h"

NS_AZOOMEE_BEGIN

HQContentItemObject::HQContentItemObject()
{
}

HQContentItemObjectRef HQContentItemObject::create()
{
    return std::make_shared<HQContentItemObject>();
}

void HQContentItemObject::setContentItemId(const std::string &inputId)
{
    _contentItemId = inputId;
}

std::string HQContentItemObject::getContentItemId() const
{
    return _contentItemId;
}

void HQContentItemObject::setTitle(const std::string &inputTitle)
{
    _title = inputTitle;
}

void HQContentItemObject::setDescription(const std::string &inputDescription)
{
    _description = inputDescription;
}

void HQContentItemObject::setType(const std::string &inputType)
{
    _type = inputType;
}

void HQContentItemObject::setUri(const std::string &inputUri)
{
    _uri = inputUri;
}

void HQContentItemObject::setEntitled(const bool inputEntitled)
{
    _entitled = inputEntitled;
}

void HQContentItemObject::setNewFlag(const bool inputNewFlag)
{
    _newFlag = inputNewFlag;
}

std::string HQContentItemObject::getTitle() const
{
    return _title;
}

std::string HQContentItemObject::getDescription() const
{
    return _description;
}

std::string HQContentItemObject::getType() const
{
    return _type;
}

std::string HQContentItemObject::getUri() const
{
    return _uri;
}

bool HQContentItemObject::getEntitled() const
{
    return _entitled;
}

bool HQContentItemObject::getNewFlag() const
{
    return _newFlag;
}

//All functions that are being used only upon reading out

void HQContentItemObject::setImagePath(const std::string &inputImagePath)
{
    _imagePath = inputImagePath;
}

void HQContentItemObject::setElementNumber(int inputElementNumber)
{
    _elementNumber = inputElementNumber;
}

void HQContentItemObject::setElementShape(const cocos2d::Vec2 &inputElementShape)
{
    _elementShape = inputElementShape;
}

std::string HQContentItemObject::getImagePath() const
{
    return _imagePath;
}

int HQContentItemObject::getElementNumber() const
{
    return _elementNumber;
}

cocos2d::Vec2 HQContentItemObject::getElementShape()
{
    return _elementShape;
}

//Other helper methods

std::string HQContentItemObject::getJSONRepresentationOfStructure() const
{
    std::map<std::string, std::string> objectMap;
    objectMap["id"] = _contentItemId;
    objectMap["title"] = _title;
    objectMap["description"] = _description;
    objectMap["type"] = _type;
    objectMap["uri"] = _uri;
    _entitled ? objectMap["entitled"] = "true" : objectMap["entitled"] = "false";
    _newFlag ? objectMap["newFlag"] = "true" : objectMap["newFlag"] = "false";
    
    return getJSONStringFromMap(objectMap);
}

HQContentItemObjectRef HQContentItemObject::createFromMap(const std::map<std::string, std::string> &inputMap)
{
    HQContentItemObjectRef returnObject = HQContentItemObject::create();
    if(inputMap.find("id") != inputMap.end())
    {
        returnObject->setContentItemId(inputMap.at("id"));
    }
    
    if(inputMap.find("title") != inputMap.end())
    {
        returnObject->setTitle(inputMap.at("title"));
    }
    
    if(inputMap.find("description") != inputMap.end())
    {
        returnObject->setDescription(inputMap.at("description"));
    }
    
    if(inputMap.find("type") != inputMap.end())
    {
        returnObject->setType(inputMap.at("type"));
    }
    
    if(inputMap.find("uri") != inputMap.end())
    {
        returnObject->setUri(inputMap.at("uri"));
    }
    
    if(inputMap.find("entitled") != inputMap.end())
    {
        inputMap.at("entitled") == "false" ? returnObject->setEntitled(false) : returnObject->setEntitled(true);
    }
    
    if(inputMap.find("newFlag") != inputMap.end())
    {
        inputMap.at("newFlag") == "true" ? returnObject->setNewFlag(true) : returnObject->setNewFlag(false);
    }
    
    return returnObject;
}

NS_AZOOMEE_END
