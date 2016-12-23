#include "HQDataProvider.h"

USING_NS_CC;

#include "external/json/document.h"
#include "external/json/writer.h"
#include "external/json/stringbuffer.h"
#include "external/json/prettywriter.h"

#include "HQScene.h"
#include "BackEndCaller.h"
#include "DataStorage.h"
#include "HttpRequestCreator.h"
#include "ConfigStorage.h"

using namespace cocos2d;

static HQDataProvider *_sharedHQDataProvider = NULL;

HQDataProvider* HQDataProvider::getInstance()
{
    if (! _sharedHQDataProvider)
    {
        _sharedHQDataProvider = new HQDataProvider();
        _sharedHQDataProvider->init();
    }
    
    return _sharedHQDataProvider;
}

HQDataProvider::~HQDataProvider(void)
{
}

bool HQDataProvider::init(void)
{
    return true;
}

std::string HQDataProvider::getImageUrlForItem(std::string itemId)
{
    return StringUtils::format("https://media.azoomee.ninja/static/images/%s/toyBoxImage.jpg", itemId.c_str());
}

void HQDataProvider::startBuildingHQ(std::string category)
{
    if(category != "HOME")
    {
        Scene *runningScene = Director::getInstance()->getRunningScene();
        Node *baseLayer = runningScene->getChildByName("baseLayer");
        Node *contentLayer = baseLayer->getChildByName("contentLayer");
        HQScene *hqLayer = (HQScene *)contentLayer->getChildByName(category.c_str());
        
        hqLayer->startBuildingScrollViewBasedOnName();
    }
}

void HQDataProvider::getDataForHQ(std::string category)
{
    if(ConfigStorage::getInstance()->forcereload)
    {
        HQData.erase(category.c_str());
    }
    
    if(HQData.find(category.c_str()) != HQData.end())
    {
        startBuildingHQ(category);
    }
    else
    {
        if(HQGetContentUrls.find(category.c_str()) != HQGetContentUrls.end()) //We don't have the data locally. Now we have to check if the data has to be downloaded (we have an uri for it)
        {
            getContent(HQGetContentUrls[category.c_str()], category.c_str());
        }
    }
}

bool HQDataProvider::parseHQData(std::string responseString, const char *category)
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
                if(!contentData["items"][key][itemNames.at(i).c_str()].IsNull())
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
            
            //setting up highlight for element
            if(!contentData["items"][key]["shape"].IsNull())
            {
                int highlightx = contentData["items"][key]["shape"][0].GetInt();
                int highlighty = contentData["items"][key]["shape"][1].GetInt();
                Vec2 elementHighlight = Vec2(highlightx, highlighty);
                elementHighlightWithTitle[key] = elementHighlight;
            }
        }
    }
    
    HQData[StringUtils::format("%s", category)] = HQElements;
    HQElementHighlights[category] = elementHighlightWithTitle;
    
    return true;
}

bool HQDataProvider::parseHQStructure(std::string responseString, const char *category)
{
    rapidjson::Document contentData;
    contentData.Parse(responseString.c_str());
    std::vector<std::map<std::string, std::string>> HQElements;
    std::vector<std::string> actualListTitles;
    std::vector<std::vector<std::string>> actualListElements;
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
        if(contentData["rows"][i]["contentIds"].Size() != 0)
        {
            for(int j = 0; j < contentData["rows"][i]["contentIds"].Size(); j++)
            {
                contentIds.push_back(contentData["rows"][i]["contentIds"][j].GetString());
            }
        }
        actualListElements.push_back(contentIds);
    }
    
    HQListTitles[StringUtils::format("%s", category)] = actualListTitles;
    HQListElements[StringUtils::format("%s", category)] = actualListElements;
    
    return true;

}

bool HQDataProvider::parseHQGetContentUrls(std::string responseString)
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
                HQGetContentUrls[M->name.GetString()] = contentData["categories"][M->name.GetString()]["uri"].GetString();
            }
        }
    }
    
    HQGetContentUrls.erase("HOME"); //On front-end home is being handled separately from all other HQ-s.
    
    return true;
}

int HQDataProvider::getNumberOfRowsForHQ(std::string category)
{
    return (int)HQListTitles[category].size();
}

int HQDataProvider::getNumberOfElementsForRow(std::string category, int index)
{
    return (int)HQListElements[category][index].size();
}

std::vector<std::string> HQDataProvider::getElementsForRow(std::string category, int index)
{
    return HQListElements[category][index];
}

std::string HQDataProvider::getTitleForRow(std::string category, int index)
{
    return HQListTitles[category].at(index);
}

std::map<std::string, std::string> HQDataProvider::getItemDataForSpecificItem(std::string category, std::string itemid)
{
    std::map<std::string, std::string> result;
    
    std::vector<std::map<std::string, std::string>> toBeChecked = HQData[category];
    for(int i = 0; i < toBeChecked.size(); i++)
    {
        if(toBeChecked.at(i)["id"] == itemid) result = toBeChecked.at(i);
    }
    
    return result;
}

Vec2 HQDataProvider::getHighlightDataForSpecificItem(std::string category, std::string itemid)
{
    std::map<std::string, Vec2> mapForCategory = HQElementHighlights[category];
    
    if(mapForCategory.find(itemid) != mapForCategory.end())
    {
        return HQElementHighlights[category][itemid];
    }
    else return Vec2(1,1);
}

//GETTING CONTENT

void HQDataProvider::getContent(std::string url, std::string category)
{
    HttpRequestCreator* httpRequestCreator = new HttpRequestCreator();
    httpRequestCreator->url = url;
    httpRequestCreator->requestBody = "";
    httpRequestCreator->requestTag = category;
    httpRequestCreator->createEncryptedGetHttpRequest();
}

void HQDataProvider::onGetContentAnswerReceived(std::string responseString, std::string category)
{
    if(parseHQData(responseString, category.c_str()))       //Parsing method returns true if there are no errors in the json string.
    {
        parseHQStructure(responseString, category.c_str());
        startBuildingHQ(category.c_str());
            
        if(category == "HOME")    //If we have a home HQ set up, we have to get urls too.
        {
            parseHQGetContentUrls(responseString);      //Parsing method returns true if there are no errors in the json string.
            BackEndCaller::getInstance()->getGordon();                                                            //If both parsings went well, we move on to getting the cookies
        }
    }
}

std::string HQDataProvider::getExtensionFromUri(std::string uri)
{
    int startPoint = (int)uri.find_last_of(".") + 1;
    std::string extension = uri.substr(startPoint);
    
    return extension;
}
