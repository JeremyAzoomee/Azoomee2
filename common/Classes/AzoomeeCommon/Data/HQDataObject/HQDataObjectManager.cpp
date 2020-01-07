#include "HQDataObjectManager.h"
#include "../Json.h"
#include "../ConfigStorage.h"
#include "ContentItemManager.h"
#include "../../UI/Style.h"


using namespace cocos2d;

NS_AZOOMEE_BEGIN

static std::auto_ptr<HQDataObjectManager> _sharedHQDataObjectManager;

HQDataObjectManager* HQDataObjectManager::getInstance()
{
    if (! _sharedHQDataObjectManager.get())
    {
        _sharedHQDataObjectManager.reset(new HQDataObjectManager);
    }
    
    return _sharedHQDataObjectManager.get();
}

HQDataObjectManager::~HQDataObjectManager()
{
}

HQDataObjectManager::HQDataObjectManager()
{
}

HQDataObjectRef HQDataObjectManager::getHQDataObjectForKey(const std::string &key)
{
    if(_hqDataObjects.find(key) != _hqDataObjects.end())
    {
		 return _hqDataObjects[key];
    }
	return nullptr;
	
}

void HQDataObjectManager::clearAllHQData()
{
    _hqDataObjects.clear();
}

bool HQDataObjectManager::isSameHQData(const std::string &etag) const
{
    return _currentHQDataEtag == etag;
}

void HQDataObjectManager::setHQDataEtag(const std::string &etag)
{
    _currentHQDataEtag = etag;
}

void HQDataObjectManager::parseNavigationData(const std::string &data)
{
	rapidjson::Document result;
	result.Parse(data.c_str());
	if(result.HasParseError() || !result.HasMember("navigation"))
	{
		return;
	}
	
	std::vector<std::string> hqNames;
	for(int i = 0; i < result["navigation"].Size(); i++)
	{
		const auto& value = result["navigation"][i];
		const std::string& hqName = getStringFromJson("name", value);
		hqNames.push_back(hqName);
		if(getBoolFromJson("default", value, false))
		{
			ConfigStorage::getInstance()->setDefaultHQ(hqNames.back());
		}
		if(value.HasMember("available"))
		{
			MutableHQDataObjectRef dataObject = nullptr;
			if(_hqDataObjects.find(hqName) == _hqDataObjects.end())
			{
				dataObject = MutableHQDataObject::create();
				_hqDataObjects[hqName] = dataObject;
			}
			else
			{
				dataObject = _hqDataObjects.at(hqName);
			}
			dataObject->setHqEntitlement(getBoolFromJson("available", value));
		}
		
	}
	ConfigStorage::getInstance()->setNavigationHQs(hqNames);
}

void HQDataObjectManager::parseHQStructureData(const std::string& hqStuctureData, const std::string& hqName)
{
    /* Example structure
	{
        "items": {
            "1111111-11111-1111-111111": {
                "entitled": true
            },
            "1111111-22222-1111-111111": {
                "entitled": true
            },
            etc
        },
        "rows": [
            {
                "contentIds": [
                    "1111111-11111-1111-111111",
                    etc
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
        ]
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
			ContentItemManager::getInstance()->parseContentItemURI(M->name.GetString(),getStringFromJson("location", M->value, ""));
		}
	}
	if(!hqData.HasMember("rows"))
	{
		return;
	}
	
	MutableHQDataObjectRef dataObject = nullptr;
	if(_hqDataObjects.find(hqName) == _hqDataObjects.end())
	{
		dataObject = MutableHQDataObject::create();
		_hqDataObjects[hqName] = dataObject;
		if(hqName == ConfigStorage::kGroupHQName)
		{
			dataObject->setHqEntitlement(true); // force set group hq to entitled as not set in entitlement feed
		}
	}
	else
	{
		dataObject = _hqDataObjects.at(hqName);
	}
	
	dataObject->clearData();
	
	for (int rowNumber = 0; rowNumber < hqData["rows"].Size(); rowNumber++)
	{
		const rapidjson::Value& rowData = hqData["rows"][rowNumber];
		MutableHQCarouselObjectRef carouselObject = MutableHQCarouselObject::create();
		
		carouselObject->setTitle(getStringFromJson("title", rowData));
        const Color4B& carouselColour = rowNumber == 0 ? Color4B(Style::Color::macaroniAndCheese) : getColor4BFromJson("rgbColour", rowData, Color4B(Style::Color::azure));
        carouselObject->setColour(carouselColour);
		if(rowData.HasMember("images"))
		{
			carouselObject->setIcon(getStringFromJson("carouselImage", rowData["images"])); //parsing carousel main icon if present
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
				
				const HQContentItemObjectRef &pointerToContentItem = ContentItemManager::getInstance()->getContentItemForId(contentId);
				if(pointerToContentItem == nullptr)
				{
					continue;
				}
                pointerToContentItem->setCarouselColour(carouselColour);
                
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
		
		dataObject->addCarouselToHq(carouselObject);
		dataObject->setHqType(hqName);
	}
	
	if(hqData.HasMember("images"))
	{
		dataObject->setImages(getStringMapFromJson(hqData["images"]));
	}
	
}

NS_AZOOMEE_END
