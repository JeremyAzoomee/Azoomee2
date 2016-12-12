#include "HQDataProvider.h"

#include "network/HttpClient.h"

#include "external/json/document.h"
#include "external/json/writer.h"
#include "external/json/stringbuffer.h"

#include "JWTTool.h"

USING_NS_CC;

#define CI_HOST "api.elb.ci.azoomee.ninja"
#define CI_URL "http://" CI_HOST
#define LOGIN_URL CI_URL"/api/auth/login"

#include "external/json/document.h"
#include "external/json/writer.h"
#include "external/json/stringbuffer.h"
#include "external/json/prettywriter.h"

#include "HQScene.h"

#include "BackEndCaller.h"
#include "DataStorage.h"


using namespace network;
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
        }
    }
    
    HQData[StringUtils::format("%s", category)] = HQElements;
    
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

std::vector<std::map<std::string, std::string>> HQDataProvider::getMainHubDataForGivenType(std::string type)
{
    std::vector<std::map<std::string, std::string>> result;
    
    for(int i = 0; i < HQData["HOME"].size(); i++)
    {
        if(HQData["HOME"].at(i)["type"] == type.c_str())
        {
            result.push_back(HQData["HOME"].at(i));
        }
    }
    
    return result;
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

//GETTING CONTENT

void HQDataProvider::getContent(std::string url, std::string category)
{
    HttpRequest *request = new HttpRequest();
    request->setRequestType(HttpRequest::Type::GET);
    request->setUrl(url.c_str());
    
    auto myJWTTool = JWTTool::getInstance();
    
    std::string myRequestString = myJWTTool->buildJWTString("GET", BackEndCaller::getInstance()->getPathFromUrl(url), BackEndCaller::getInstance()->getHostFromUrl(url), "", "");
    
    std::vector<std::string> headers;
    headers.push_back(StringUtils::format("x-az-req-datetime: %s", BackEndCaller::getInstance()->getDateFormatString().c_str()));
    headers.push_back(StringUtils::format("x-az-auth-token: %s", myRequestString.c_str()));
    
    request->setHeaders(headers);
    
    request->setResponseCallback(CC_CALLBACK_2(HQDataProvider::onGetContentAnswerReceived, this));
    request->setTag(category);
    HttpClient::getInstance()->send(request);
    
}

void HQDataProvider::onGetContentAnswerReceived(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response)
{
    std::string responseString = "";
    
    if (response && response->getResponseData())
    {
        std::vector<char> myResponse = *response->getResponseData();
        responseString = std::string(myResponse.begin(), myResponse.end());
        CCLOG("get content data: %s", responseString.c_str());
    }
    
    if(response->getResponseCode() == 200)          //Get content success
    {
        if(parseHQData(responseString, response->getHttpRequest()->getTag()))       //Parsing method returns true if there are no errors in the json string.
        {
            parseHQStructure(responseString, response->getHttpRequest()->getTag());
            startBuildingHQ(response->getHttpRequest()->getTag());
            
            if(strncmp(response->getHttpRequest()->getTag(), "HOME", strlen(response->getHttpRequest()->getTag())) == 0)    //If we have a home HQ set up, we have to get urls too.
            {
                parseHQGetContentUrls(responseString);      //Parsing method returns true if there are no errors in the json string.
                BackEndCaller::getInstance()->getGordon();                                                            //If both parsings went well, we move on to getting the cookies
            }
        }
    }
    else
    {
        CCLOG("GET CONTENT FAIL Response code: %ld", response->getResponseCode());
        CCLOG("GET CONTENT FAIL Response: %s", responseString.c_str());
    }
}
