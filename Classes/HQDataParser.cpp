#include "HQDataParser.h"
#include "HQDataProvider.h"

#include <external/json/document.h>
#include <external/json/writer.h>
#include <external/json/stringbuffer.h>
#include <external/json/prettywriter.h>

#include <AzoomeeCommon/Data/Json.h>

#include "BackEndCaller.h"
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include <AzoomeeCommon/Data/Child/ChildDataProvider.h>
#include <AzoomeeCommon/Data/Child/ChildDataParser.h>

#include <AzoomeeCommon/Data/HQDataObject/HQDataObjectStorage.h>
#include <AzoomeeCommon/Data/HQDataObject/HQDataObject.h>
#include <AzoomeeCommon/Data/HQDataObject/HQCarouselObject.h>
#include <AzoomeeCommon/Data/HQDataObject/HQContentItemObject.h>
#include <AzoomeeCommon/Data/HQDataObject/ContentItemPool.h>

#include <AzoomeeCommon/UI/ModalMessages.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

static HQDataParser *_sharedHQDataParser = NULL;

HQDataParser* HQDataParser::getInstance()
{
    if (! _sharedHQDataParser)
    {
        _sharedHQDataParser = new HQDataParser();
        _sharedHQDataParser->init();
    }
    
    return _sharedHQDataParser;
}

HQDataParser::~HQDataParser(void)
{
}

bool HQDataParser::init(void)
{
    return true;
}

bool HQDataParser::parseHQData(const std::string &responseString, const char *hqName)
{
    rapidjson::Document contentData;
    contentData.Parse(responseString.c_str());
    std::vector<std::map<std::string, std::string>> HQElements;
    
    if ((contentData.HasParseError())||(!contentData.HasMember("items")))
    {
        return false;
    }
    
    HQDataObjectStorage::getInstance()->getHQDataObjectForKey(hqName)->clearData();
    
    rapidjson::Value::MemberIterator M;
    
    for (M=contentData["items"].MemberBegin(); M!=contentData["items"].MemberEnd(); M++)
    {
        const char *key = M->name.GetString();
        
        if (key!=NULL)
        {
            HQContentItemObjectRef contentObject = HQContentItemObject::create();

            const rapidjson::Value &itemData = contentData["items"][key];
            
            contentObject->setContentItemId(key);
            contentObject->setTitle(getStringFromJson("title", itemData));
            contentObject->setDescription(getStringFromJson("description", itemData));
            contentObject->setType(getStringFromJson("type", itemData));
            contentObject->setUri(getStringFromJson("uri", itemData));
            contentObject->setEntitled(getBoolFromJson("entitled", itemData));
            
            if(itemData.HasMember("tags"))
            {
                contentObject->setTags(getStringArrayFromJson(itemData["tags"]));
            }
            
            if(itemData.HasMember("images"))
            {
                contentObject->setImages(getStringMapFromJson(itemData["images"]));
            }
            
            ContentItemPool::getInstance()->addContentItemToPool(contentObject);
        }
    }
    
    return true;
}

bool HQDataParser::parseHQStructure(const std::string &responseString, const char *hqName)
{
    rapidjson::Document contentData;
    contentData.Parse(responseString.c_str());
    
    if (contentData.HasParseError() || !contentData.HasMember("rows") || !contentData["rows"].IsArray())
    {
        return false; //JSON HAS ERRORS IN IT
    }
    
    for (int rowNumber = 0; rowNumber < contentData["rows"].Size(); rowNumber++)
    {
        const rapidjson::Value& rowData = contentData["rows"][rowNumber];
        HQCarouselObjectRef carouselObject = HQCarouselObject::create();
        
        carouselObject->setTitle(getStringFromJson("title", rowData));
        
        if(rowData.HasMember("images"))
        {
            carouselObject->setIcon(getStringFromJson("icon", rowData["images"])); //parsing carousel main icon if present
        }
        
        if(rowData.HasMember("contentIds") && rowData["contentIds"].IsArray() && rowData["contentIds"].Size() != 0)
        {
            const rapidjson::Value& contentIds = rowData["contentIds"];
            for(int elementIndex = 0; elementIndex < contentIds.Size(); elementIndex++)
            {
                if(!contentIds[elementIndex].IsString())
                {
                    continue;
                }
                const std::string &contentId = contentIds[elementIndex].GetString();
                
                const HQContentItemObjectRef &pointerToContentItem = ContentItemPool::getInstance()->getContentItemForId(contentId);
                if(pointerToContentItem == nullptr)
                {
                    continue;
                }
                
                Vec2 contentItemHighlight = Vec2(1,1);
                if(rowData.HasMember("shapes") && rowData["shapes"].IsArray() && rowData["shapes"].Size() > elementIndex)
                {
                    const rapidjson::Value& elementShapeData = rowData["shapes"][elementIndex];
                    if(elementShapeData.IsArray() && elementShapeData.Size() == 2 && elementShapeData[0].IsInt() && elementShapeData[1].IsInt())
                    {
                       contentItemHighlight = Vec2(elementShapeData[0].GetInt(), elementShapeData[1].GetInt());
                    }
                }
                
                carouselObject->addContentItemToCarousel(pointerToContentItem);
                carouselObject->addContentItemHighlight(contentItemHighlight);
            }
        }
        
        HQDataObjectStorage::getInstance()->getHQDataObjectForKey(hqName)->addCarouselToHq(carouselObject);
        HQDataObjectStorage::getInstance()->getHQDataObjectForKey(hqName)->setHqType(hqName);
    }
    
    if(contentData.HasMember("images"))
    {
        HQDataObjectStorage::getInstance()->getHQDataObjectForKey(hqName)->setImages(getStringMapFromJson(contentData["images"]));
    }
    
    return true;
}

bool HQDataParser::parseHQGetContentUrls(const std::string &responseString)
{
    rapidjson::Document contentData;
    contentData.Parse(responseString.c_str());
    
    if( contentData.HasParseError() || !contentData.HasMember("hqs") )
    {
        return false;
    }
    
    rapidjson::Value::MemberIterator M;
    
    for (M=contentData["hqs"].MemberBegin(); M!=contentData["hqs"].MemberEnd(); M++)
    {
        const char *key = M->name.GetString();
        const std::string &replacedKey = ConfigStorage::getInstance()->getHQSceneNameReplacementForPermissionFeed(key);
        
        const rapidjson::Value &currentItem = contentData["hqs"][key];
        
        HQDataObjectRef dataObject = HQDataObjectStorage::getInstance()->getHQDataObjectForKey(replacedKey);
        dataObject->setHqEntitlement(getBoolFromJson("available", currentItem));
        dataObject->setHqUrl(getStringFromJson("uri", currentItem));
    }
    
    return true;
}

//GETTING CONTENT

void HQDataParser::onGetContentAnswerReceived(const std::string &responseString, const std::string &hqName)
{
    if(parseHQData(responseString, hqName.c_str()))       //Parsing method returns true if there are no errors in the json string.
    {
        ModalMessages::getInstance()->stopLoading();
        parseHQStructure(responseString, hqName.c_str());
        
        HQDataProvider::getInstance()->startBuildingHQ(hqName);
        
    }
}

std::string HQDataParser::getExtensionFromUri(const std::string &uri) const
{
    int startPoint = (int)uri.find_last_of(".") + 1;
    std::string extension = uri.substr(startPoint);
    
    return extension;
}

NS_AZOOMEE_END
