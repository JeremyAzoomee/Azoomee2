//
//  HQStructureParser.cpp
//  AzoomeeCommon
//
//  Created by Macauley on 10/05/2018.
//

#include "HQStructureParser.h"
#include "HQDataObject.h"
#include "ContentItemPool.h"
#include "HQDataObjectStorage.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

static std::auto_ptr<HQStructureParser> sHQStructureParserSharedInstance;

HQStructureParser* HQStructureParser::getInstance()
{
    if(!sHQStructureParserSharedInstance.get())
    {
        sHQStructureParserSharedInstance.reset(new HQStructureParser());
    }
    return sHQStructureParserSharedInstance.get();
}

HQStructureParser::~HQStructureParser(void)
{
    
}

void HQStructureParser::parseHQStructureData(const std::string& hqStuctureData, const std::string& hqName)
{
    // stucture
    /*
    {
     "items":{
         "1111111-11111-1111-111111":{"entitled":true},
         "1111111-22222-1111-111111":{"entitled":true}
         etc.
     },
     "rows": [
     {
        "contentIds": [
            "1111111-11111-1111-111111",
            etc.
        ],
        "title": "favs",
        "images": {
            "icon": "icon url"
        },
        "shapes": [
            [1,1],
            etc
        ]
     }
    }
    */
    
    rapidjson::Document hqData;
    hqData.Parse(hqStuctureData.c_str());
    
    if (hqData.HasParseError())
    {
        return; //JSON HAS ERRORS IN IT
    }
    
    if(hqData.HasMember("items"))
    {
        const rapidjson::Value& items = hqData["items"];
        for (auto M = items.MemberBegin(); M != items.MemberEnd(); M++)
        {
            auto item = ContentItemPool::getInstance()->getContentItemForId(M->name.GetString());
            if(item)
            {
                item->setUri(getStringFromJson("location", M->value, item->getUri()));
            }
        }
    }
    if(!hqData.HasMember("rows"))
    {
        return;
    }
    
    HQDataObjectRef hq = HQDataObjectStorage::getInstance()->getHQDataObjectForKey(hqName);
    
    hq->clearData();
    
    for (int rowNumber = 0; rowNumber < hqData["rows"].Size(); rowNumber++)
    {
        const rapidjson::Value& rowData = hqData["rows"][rowNumber];
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
        
        hq->addCarouselToHq(carouselObject);
        hq->setHqType(hqName);
    }
        
    if(hqData.HasMember("images"))
    {
        hq->setImages(getStringMapFromJson(hqData["images"]));
    }
    
}

void HQStructureParser::parseEntitlementData(const std::string &entitlementData)
{
    rapidjson::Document data;
    data.Parse(entitlementData.c_str());
    
    if (data.HasParseError())
    {
        return; //JSON HAS ERRORS IN IT
    }
    
    for (auto M = data.MemberBegin(); M != data.MemberEnd(); M++)
    {
        auto item = ContentItemPool::getInstance()->getContentItemForId(M->name.GetString());
        if(item)
        {
            item->setEntitled(M->value.GetBool());
        }
    }
}

NS_AZOOMEE_END
