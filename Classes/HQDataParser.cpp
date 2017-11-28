#include "HQDataParser.h"
#include "HQDataProvider.h"

#include <external/json/document.h>
#include <external/json/writer.h>
#include <external/json/stringbuffer.h>
#include <external/json/prettywriter.h>

#include <AzoomeeCommon/Data/Json.h>

#include "HQScene.h"
#include "BackEndCaller.h"
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include <AzoomeeCommon/Data/Child/ChildDataProvider.h>
#include <AzoomeeCommon/Data/Child/ChildDataParser.h>
#include "BaseScene.h"
#include "MainHubScene.h"

#include <AzoomeeCommon/Data/HQDataObject/HQDataObjectStorage.h>
#include <AzoomeeCommon/Data/HQDataObject/HQDataObject.h>
#include <AzoomeeCommon/Data/HQDataObject/HQCarouselObject.h>
#include <AzoomeeCommon/Data/HQDataObject/HQContentItemObject.h>

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

bool HQDataParser::parseHQData(const std::string &responseString, const char *category)
{
    rapidjson::Document contentData;
    contentData.Parse(responseString.c_str());
    std::vector<std::map<std::string, std::string>> HQElements;
    
    if ((contentData.HasParseError())||(!contentData.HasMember("items")))
    {
        return false;
    }
    
    HQDataObjectStorage::getInstance()->getHQDataObjectForKey(category)->clearData();
    
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
            
            HQDataObjectStorage::getInstance()->getHQDataObjectForKey(category)->addContentItemToRawStorage(key, contentObject);
        }
    }
    
    return true;
}

bool HQDataParser::parseHQStructure(const std::string &responseString, const char *category)
{
    rapidjson::Document contentData;
    contentData.Parse(responseString.c_str());
    
    if (contentData.HasParseError()) return false; //JSON HAS ERRORS IN IT
    
    for (int i = 0; i < contentData["rows"].Size(); i++)
    {
        HQCarouselObjectRef carouselObject = HQCarouselObject::create();
        
        carouselObject->setTitle(getStringFromJson("title", contentData["rows"][i]));
        
        if(contentData["rows"][i].HasMember("images"))
        {
            carouselObject->setIcon(getStringFromJson("icon", contentData["rows"][i]["images"])); //parsing carousel main icon if present
        }
        
        if(contentData["rows"][i]["contentIds"].Size() != 0)
        {
            for(int j = 0; j < contentData["rows"][i]["contentIds"].Size(); j++)
            {
                const std::string &contentId = contentData["rows"][i]["contentIds"][j].GetString();
                
                const HQContentItemObjectRef &pointerToContentItem = HQDataObjectStorage::getInstance()->getHQDataObjectForKey(category)->getContentItemForId(contentId);
                Vec2 contentItemHighlight = Vec2(contentData["rows"][i]["shapes"][j][0].GetInt(), contentData["rows"][i]["shapes"][j][1].GetInt());
                
                carouselObject->addContentItemToCarousel(pointerToContentItem);
                carouselObject->addContentItemHighlight(contentItemHighlight);
            }
        }
        
        HQDataObjectStorage::getInstance()->getHQDataObjectForKey(category)->addCarusoelToHq(carouselObject);
        HQDataObjectStorage::getInstance()->getHQDataObjectForKey(category)->setHqType(category);
    }
    
    if(contentData.HasMember("images"))
    {
        HQDataObjectStorage::getInstance()->getHQDataObjectForKey(category)->setImages(getStringMapFromJson(contentData["images"]));
    }
    
    return true;
}

bool HQDataParser::parseHQGetContentUrls(const std::string &responseString)
{
    HQDataObjectStorage::getInstance()->clearAllHQData();
    
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

void HQDataParser::onGetContentAnswerReceived(const std::string &responseString, const std::string &category)
{
    if(parseHQData(responseString, category.c_str()))       //Parsing method returns true if there are no errors in the json string.
    {
        ModalMessages::getInstance()->stopLoading();
        parseHQStructure(responseString, category.c_str());
        
        if(category == "HOME")
        {
            ChildDataParser::getInstance()->parseOomeeData(responseString);
            
            Scene *runningScene = Director::getInstance()->getRunningScene();
            if(!runningScene->getChildByName("baseLayer")) return;
            
            Node *baseLayer = runningScene->getChildByName("baseLayer");
            Node *contentLayer = baseLayer->getChildByName("contentLayer");
            MainHubScene *homeLayer = (MainHubScene *)contentLayer->getChildByName("HOME");

            homeLayer->buildMainHubScene();
        }
        else
        {
            HQDataProvider::getInstance()->startBuildingHQ(category);
        }
    }
}

std::string HQDataParser::getExtensionFromUri(const std::string &uri) const
{
    int startPoint = (int)uri.find_last_of(".") + 1;
    std::string extension = uri.substr(startPoint);
    
    return extension;
}

NS_AZOOMEE_END
