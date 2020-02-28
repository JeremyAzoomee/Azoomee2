#include "HQContentItemObject.h"
#include "../../Utils/StringFunctions.h"

NS_TZ_BEGIN

const std::string& HQContentItemObject::kTagNew = "NEW";
const char* const HQContentItemObject::kContentTypeVideo = "VIDEO";
const char* const HQContentItemObject::kContentTypeAudio = "AUDIO";
const char* const HQContentItemObject::kContentTypeGame = "GAME";
const char* const HQContentItemObject::kContentTypeGroup = "GROUP";
const char* const HQContentItemObject::kContentTypeAudioGroup = "AUDIOGROUP";
const char* const HQContentItemObject::kContentTypeManual = "MANUAL";
const char* const HQContentItemObject::kContentTypeInternal = "INTERNAL";

HQContentItemObject::HQContentItemObject()
{
}

MutableHQContentItemObjectRef MutableHQContentItemObject::create()
{
    return std::make_shared<MutableHQContentItemObject>();
}

void MutableHQContentItemObject::setContentItemId(const std::string &inputId)
{
    _contentItemId = inputId;
}

std::string HQContentItemObject::getContentItemId() const
{
    return _contentItemId;
}

void MutableHQContentItemObject::setTitle(const std::string &inputTitle)
{
    _title = inputTitle;
}

void MutableHQContentItemObject::setDescription(const std::string &inputDescription)
{
    _description = inputDescription;
}

void MutableHQContentItemObject::setType(const std::string &inputType)
{
    _type = inputType;
}

void MutableHQContentItemObject::setUri(const std::string &inputUri)
{
    _uri = inputUri;
}

void MutableHQContentItemObject::setEntitled(const bool inputEntitled)
{
    _entitled = inputEntitled;
}

void MutableHQContentItemObject::addTag(const std::string &tag)
{
    _tags.push_back(tag);
}

void MutableHQContentItemObject::setTags(const std::vector<std::string> &tags)
{
    _tags = tags;
}

void MutableHQContentItemObject::setImages(const std::map<std::string, std::string> &images)
{
    _images = images;
}

void MutableHQContentItemObject::setItems(const std::vector<std::string> &items)
{
    _items = items;
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

bool HQContentItemObject::isEntitled() const
{
    return _entitled;
}

bool HQContentItemObject::isNew() const
{
    for(const std::string &tag : _tags)
    {
        if(tag == kTagNew)
        {
            return true;
        }
    }
    return false;
}

std::vector<std::string> HQContentItemObject::getTags() const
{
    return _tags;
}

std::map<std::string, std::string> HQContentItemObject::getImages() const
{
    return _images;
}

std::string HQContentItemObject::getBaseImageThumbUrl() const
{
    if(_images.find("ONE_ONE") == _images.end())
    {
        return "";
    }
    
    return _images.at("ONE_ONE");
}

std::vector<std::string> HQContentItemObject::getItems() const
{
    return _items;
}

//All functions that are being used only upon reading out

void MutableHQContentItemObject::setImagePath(const std::string &inputImagePath)
{
    _imagePath = inputImagePath;
}

void MutableHQContentItemObject::setElementNumber(int inputElementNumber)
{
    _elementNumber = inputElementNumber;
}

void MutableHQContentItemObject::setElementShape(const cocos2d::Vec2 &inputElementShape)
{
    _elementShape = inputElementShape;
}

void HQContentItemObject::setCarouselColour(const cocos2d::Color4B &colour)
{
    _carouselColour = colour;
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

cocos2d::Color4B HQContentItemObject::getCarouselColour() const
{
    return _carouselColour;
}

//Other helper methods

std::string HQContentItemObject::getJSONRepresentationOfStructure() const
{
    std::map<std::string, std::string> objectMap {
        {"id", _contentItemId},
        {"title", _title},
        {"description", _description},
        {"type", _type},
        {"uri", _uri},
        {"thumbUrl", getBaseImageThumbUrl()}
    };
    
    _entitled ? objectMap["entitled"] = "true" : objectMap["entitled"] = "false";
    isNew() ? objectMap["newFlag"] = "true" : objectMap["newFlag"] = "false";
    
    return StringFunctions::getJSONStringFromMap(objectMap);
}

MutableHQContentItemObjectRef MutableHQContentItemObject::createFromMap(const std::map<std::string, std::string> &inputMap)
{
    MutableHQContentItemObjectRef returnObject = MutableHQContentItemObject::create();
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
        returnObject->setEntitled(inputMap.at("entitled") == "true");
    }
    
    if(inputMap.find("newFlag") != inputMap.end())
    {
        returnObject->addTag(kTagNew);
    }
    
    if(inputMap.find("thumbUrl") != inputMap.end())
    {
        std::map<std::string, std::string> imageMap;
        imageMap["ONE_ONE"] = inputMap.at("thumbUrl");
        returnObject->setImages(imageMap);
    }
    
    return returnObject;
}

NS_TZ_END
