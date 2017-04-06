#include "HQDataParser.h"
#include "HQDataStorage.h"
#include "HQDataProvider.h"

#include "external/json/document.h"
#include "external/json/writer.h"
#include "external/json/stringbuffer.h"
#include "external/json/prettywriter.h"

#include "HQScene.h"
#include "BackEndCaller.h"
#include "HttpRequestCreator.h"
#include <AzoomeeCommon/Data/ConfigStorage.h>
#include "ChildDataProvider.h"
#include "ChildDataParser.h"
#include "BaseScene.h"

using namespace cocos2d;
using namespace Azoomee;


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

bool HQDataParser::parseHQData(std::string responseString, const char *category)
{
    rapidjson::Document contentData;
    contentData.Parse(responseString.c_str());
    std::vector<std::map<std::string, std::string>> HQElements;
    std::map <std::string, Vec2> elementHighlightWithTitle;
    
    rapidjson::Value::MemberIterator M;
    const char *key;
    
    if (contentData.HasParseError()) return false; //JSON HAS ERRORS IN IT
    
    for (M=contentData["items"].MemberBegin(); M!=contentData["items"].MemberEnd(); M++)
    {
        key   = M->name.GetString();
        std::map<std::string, std::string> elementProperty;
        
        if (key!=NULL)
        {
            elementProperty["id"] = key;  //first we set up the id for the content, then we go through the properties. Reason for this: the id is the key for the contents itself, not included within the record.
            std::vector<std::string> itemNames = {"title", "description", "type", "uri"}; //we need values for these keys from the record.
            
            for(int i = 0; i < itemNames.size(); i++)
            {
                if(!contentData["items"][key].HasMember(itemNames.at(i).c_str()))
                {
                    elementProperty[itemNames.at(i).c_str()] = "";

                }
                else if(!contentData["items"][key][itemNames.at(i).c_str()].IsNull())
                {
                    elementProperty[itemNames.at(i).c_str()] = contentData["items"][key][itemNames.at(i).c_str()].GetString();
                }
                else
                {
                    elementProperty[itemNames.at(i).c_str()] = "";
                }
            }
            
            if(!contentData["items"][key]["entitled"].IsNull())
            {
                if(contentData["items"][key]["entitled"].GetBool()) elementProperty["entitled"] = "true";
                else elementProperty["entitled"] = "false";
            }
            
            HQElements.push_back(elementProperty);
        }
    }
    
    HQDataStorage::getInstance()->HQData[StringUtils::format("%s", category)].clear();
    HQDataStorage::getInstance()->HQData[StringUtils::format("%s", category)] = HQElements;
    
    return true;
}

bool HQDataParser::parseHQStructure(std::string responseString, const char *category)
{
    rapidjson::Document contentData;
    contentData.Parse(responseString.c_str());
    std::vector<std::map<std::string, std::string>> HQElements;
    std::vector<std::string> actualListTitles;
    std::vector<std::vector<std::string>> actualListElements;
    std::vector<std::vector<Vec2>> actualListHighlights;
    
    if (contentData.HasParseError()) return false; //JSON HAS ERRORS IN IT
    
    CCLOG("Category : %s, size: %d", category, contentData["rows"].Size());
    
    for (int i = 0; i < contentData["rows"].Size(); i++)
    {
        if(contentData["rows"][i]["title"].IsNull())
        {
            actualListTitles.push_back("");
        }
        else actualListTitles.push_back(contentData["rows"][i]["title"].GetString());
        
        std::vector<std::string> contentIds;
        std::vector<Vec2> listHighlights;
        
        if(contentData["rows"][i]["contentIds"].Size() != 0)
        {
            for(int j = 0; j < contentData["rows"][i]["contentIds"].Size(); j++)
            {
                contentIds.push_back(contentData["rows"][i]["contentIds"][j].GetString());
                listHighlights.push_back(Vec2(contentData["rows"][i]["shapes"][j][0].GetInt(), contentData["rows"][i]["shapes"][j][1].GetInt()));
            }
        }
        actualListElements.push_back(contentIds);
        actualListHighlights.push_back(listHighlights);
    }
    
    HQDataStorage::getInstance()->HQListTitles[category] = actualListTitles;
    HQDataStorage::getInstance()->HQListElements[category] = actualListElements;
    HQDataStorage::getInstance()->HQElementHighlights[category] = actualListHighlights;
    
    return true;

}

bool HQDataParser::parseHQGetContentUrls(std::string responseString)
{
    rapidjson::Document contentData;
    contentData.Parse(responseString.c_str());
    
    rapidjson::Value::MemberIterator M;
    
    if (contentData.HasParseError()) return false; //JSON HAS ERRORS IN IT.
    
    for (M=contentData["categories"].MemberBegin(); M!=contentData["categories"].MemberEnd(); M++)
    {
        if(contentData["categories"][M->name.GetString()].HasMember("uri"))
        {
            if(!contentData["categories"][M->name.GetString()]["uri"].IsNull())
            {
                HQDataStorage::getInstance()->HQGetContentUrls[M->name.GetString()] = contentData["categories"][M->name.GetString()]["uri"].GetString();
            }
        }
    }
    
    HQDataStorage::getInstance()->HQGetContentUrls.erase("HOME"); //On front-end home is being handled separately from all other HQ-s.
    
    return true;
}

//GETTING CONTENT

void HQDataParser::getContent(std::string url, std::string category)
{
    HttpRequestCreator* httpRequestCreator = new HttpRequestCreator();
    httpRequestCreator->url = url;
    httpRequestCreator->requestBody = "";
    httpRequestCreator->requestTag = category;
    httpRequestCreator->createEncryptedGetHttpRequest();
}

void HQDataParser::onGetContentAnswerReceived(std::string responseString, std::string category)
{
    if(parseHQData(responseString, category.c_str()))       //Parsing method returns true if there are no errors in the json string.
    {
        parseHQStructure(responseString, category.c_str());
            
        if(category == "HOME")    //If we have a home HQ set up, we have to get urls too.
        {
            parseHQGetContentUrls(responseString);      //Parsing method returns true if there are no errors in the json string.
            ChildDataParser::getInstance()->parseOomeeData(responseString);
            BackEndCaller::getInstance()->getGordon();   //If both parsings went well, we move on to getting the cookies
        }
        else
        {
            HQDataProvider::getInstance()->startBuildingHQ(category);
        }
    }
}

void HQDataParser::getPreviewContent(std::string url, std::string category)
{
    CCLOG("Getting data from: %s", url.c_str());
    
    HttpRequestCreator* httpRequestCreator = new HttpRequestCreator();
    httpRequestCreator->url = url;
    httpRequestCreator->requestBody = "";
    httpRequestCreator->requestTag = "PreviewHOME";
    httpRequestCreator->createGetHttpRequest();
}

void HQDataParser::onGetPreviewContentAnswerReceived(std::string responseString)
{
    if(parseHQData(responseString, "HOME"))       //Parsing method returns true if there are no errors in the json string.
    {
        parseHQStructure(responseString, "HOME");
        parseHQGetContentUrls(responseString);
        ChildDataParser::getInstance()->parseOomeeData(responseString);
        
        BaseScene *baseScene = (BaseScene *)Director::getInstance()->getRunningScene()->getChildByName("baseLayer");
        baseScene->startBuildingHQs();
    }
}

std::string HQDataParser::getExtensionFromUri(std::string uri)
{
    int startPoint = (int)uri.find_last_of(".") + 1;
    std::string extension = uri.substr(startPoint);
    
    return extension;
}

void HQDataParser::clearAllHQData()
{
    HQDataStorage::getInstance()->HQData.clear();
    HQDataStorage::getInstance()->HQListTitles.clear();
    HQDataStorage::getInstance()->HQListElements.clear();
    HQDataStorage::getInstance()->HQElementHighlights.clear();
    HQDataStorage::getInstance()->HQGetContentUrls.clear();
}
