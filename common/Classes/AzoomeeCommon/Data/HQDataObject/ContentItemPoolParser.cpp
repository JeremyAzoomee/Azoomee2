//
//  ContentItemPoolParser.cpp
//  AzoomeeCommon
//
//  Created by Macauley on 08/05/2018.
//

#include "ContentItemPoolParser.h"
#include "HQContentItemObject.h"
#include "ContentItemPool.h"

NS_AZOOMEE_BEGIN

static std::auto_ptr<ContentItemPoolParser> sContentItemPoolParserSharedInstance;

ContentItemPoolParser* ContentItemPoolParser::getInstance()
{
    if(!sContentItemPoolParserSharedInstance.get())
    {
        sContentItemPoolParserSharedInstance.reset(new ContentItemPoolParser());
    }
    return sContentItemPoolParserSharedInstance.get();
}

ContentItemPoolParser::~ContentItemPoolParser(void)
{

}

void ContentItemPoolParser::parseContentItemPool(const std::string contentItemPoolString)
{
    rapidjson::Document contentData;
    contentData.Parse(contentItemPoolString.c_str());
    if(contentData.HasParseError())
    {
        return;
    }
    
    if(contentData.IsObject())
    {
        if(contentData.HasMember("items"))
        {
            const rapidjson::Value& itemList = contentData["items"];
            if(itemList.IsArray())
            {
                for (auto M = itemList.MemberBegin(); M != itemList.MemberEnd(); M++)
                {
                    parseContentItem(M->name.GetString(), M->value);
                }
            }
        }
        else
        {
            for (auto M = contentData.MemberBegin(); M != contentData.MemberEnd(); M++)
            {
                parseContentItem(M->name.GetString(), M->value);
            }
        }
    }
    else
    {
        if(contentData.IsArray())
        {
            for (auto M = contentData.MemberBegin(); M != contentData.MemberEnd(); M++)
            {
                parseContentItem(M->name.GetString(), M->value);
            }
        }
    }
}

void ContentItemPoolParser::parseContentItem(const std::string &contentId, const rapidjson::Value &contentItemData)
{
    HQContentItemObjectRef contentObject = HQContentItemObject::create();
    
    contentObject->setContentItemId(contentId);
    contentObject->setTitle(getStringFromJson("title", contentItemData));
    contentObject->setDescription(getStringFromJson("description", contentItemData));
    contentObject->setType(getStringFromJson("type", contentItemData));
    contentObject->setUri(getStringFromJson("uri", contentItemData));
    contentObject->setEntitled(getBoolFromJson("entitled", contentItemData));
    
    if(contentItemData.HasMember("tags"))
    {
        contentObject->setTags(getStringArrayFromJson(contentItemData["tags"]));
    }
    
    if(contentItemData.HasMember("images"))
    {
        contentObject->setImages(getStringMapFromJson(contentItemData["images"]));
    }
    
    ContentItemPool::getInstance()->addContentItemToPool(contentObject);
}


NS_AZOOMEE_END
