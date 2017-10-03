#include "HQContentItemObject.h"
#include "../../Utils/StringFunctions.h"

NS_AZOOMEE_BEGIN

HQContentItemObject::HQContentItemObject()
{
}

HQContentItemObject HQContentItemObject::create()
{
    return HQContentItemObject();
}

void HQContentItemObject::setContentItemId(const std::string &inputId)
{
    contentItemId = inputId;
}

std::string HQContentItemObject::getContentItemId()
{
    return contentItemId;
}

void HQContentItemObject::setTitle(const std::string &inputTitle)
{
    title = inputTitle;
}

void HQContentItemObject::setDescription(const std::string &inputDescription)
{
    description = inputDescription;
}

void HQContentItemObject::setType(const std::string &inputType)
{
    type = inputType;
}

void HQContentItemObject::setUri(const std::string &inputUri)
{
    uri = inputUri;
}

void HQContentItemObject::setEntitled(const bool inputEntitled)
{
    entitled = inputEntitled;
}

void HQContentItemObject::setNewFlag(const bool inputNewFlag)
{
    newFlag = inputNewFlag;
}

std::string HQContentItemObject::getTitle()
{
    return title;
}

std::string HQContentItemObject::getDescription()
{
    return description;
}

std::string HQContentItemObject::getType()
{
    return type;
}

std::string HQContentItemObject::getUri()
{
    return uri;
}

bool HQContentItemObject::getEntitled()
{
    return entitled;
}

bool HQContentItemObject::getNewFlag()
{
    return newFlag;
}

//All functions that are being used only upon reading out

void HQContentItemObject::setImagePath(const std::string &inputImagePath)
{
    imagePath = inputImagePath;
}

void HQContentItemObject::setElementNumber(int inputElementNumber)
{
    elementNumber = inputElementNumber;
}

void HQContentItemObject::setElementShape(const cocos2d::Vec2 &inputElementShape)
{
    elementShape = inputElementShape;
}

std::string HQContentItemObject::getImagePath()
{
    return imagePath;
}

int HQContentItemObject::getElementNumber()
{
    return elementNumber;
}

cocos2d::Vec2 HQContentItemObject::getElementShape()
{
    return elementShape;
}

//Other helper methods

std::string HQContentItemObject::getJSONRepresentationOfStructure()
{
    std::map<std::string, std::string> objectMap;
    objectMap["id"] = contentItemId;
    objectMap["title"] = title;
    objectMap["description"] = description;
    objectMap["type"] = type;
    objectMap["uri"] = uri;
    entitled ? objectMap["entitled"] = true : objectMap["entitled"] = false;
    newFlag ? objectMap["newFlag"] = true : objectMap["newFlag"] = false;
    
    return getJSONStringFromMap(objectMap);
}

HQContentItemObject HQContentItemObject::createFromMap(const std::map<std::string, std::string> &inputMap)
{
    HQContentItemObject returnObject = HQContentItemObject::create();
    if(inputMap.find("id") != inputMap.end()) returnObject.setContentItemId(inputMap.at("id"));
    if(inputMap.find("title") != inputMap.end()) returnObject.setTitle(inputMap.at("title"));
    if(inputMap.find("description") != inputMap.end()) returnObject.setDescription(inputMap.at("description"));
    if(inputMap.find("type") != inputMap.end()) returnObject.setType(inputMap.at("type"));
    if(inputMap.find("uri") != inputMap.end()) returnObject.setUri(inputMap.at("uri"));
    
    if(inputMap.find("entitled") != inputMap.end())
    {
        inputMap.at("entitled") == "false" ? returnObject.setEntitled(false) : returnObject.setEntitled(true);
    }
    
    if(inputMap.find("newFlag") != inputMap.end())
    {
        inputMap.at("newFlag") == "true" ? returnObject.setNewFlag(true) : returnObject.setNewFlag(false);

    }
    
    return returnObject;
}

NS_AZOOMEE_END
