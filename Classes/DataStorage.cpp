#include "DataStorage.h"

using namespace cocos2d;

static DataStorage *_sharedDataStorage = NULL;

DataStorage* DataStorage::getInstance()
{
    if (! _sharedDataStorage)
    {
        _sharedDataStorage = new DataStorage();
        _sharedDataStorage->init();
    }
    
    return _sharedDataStorage;
}

DataStorage::~DataStorage(void)
{
}

bool DataStorage::init(void)
{
    childProfile = false;
    return true;
}

bool DataStorage::parseParentLoginData(std::string responseData)
{
    childProfile = false;
    parentLoginData.Parse(responseData.c_str());
    
    if(parentLoginData.HasMember("code"))
    {
        if(parentLoginData["code"] != "INVALID_CREDENTIALS")
        {
            return true;
        }
    }
    
    return false;
}

std::string DataStorage::getParentLoginValue(std::string keyName)
{
    return parentLoginData[keyName.c_str()].GetString();
}

std::string DataStorage::getParentOrChildLoginValue(std::string keyName) //This method selects a given key from the parent or kid login data weather there is kid login data or parent only.
{
    if(childProfile)
    {
        return childLoginData[keyName.c_str()].GetString();
    }
    else
    {
        return parentLoginData[keyName.c_str()].GetString();
    }
}

std::string DataStorage::getChildLoginValue(std::string keyName)
{
    return childLoginData[keyName.c_str()].GetString();
}

std::string DataStorage::getAvailableChildrenValue(std::string keyName)
{
    return availableChildrenData[keyName.c_str()].GetString();
}

int DataStorage::getAmountOfAvailableChildren()
{
    return (int)availableChildrenData.Size();
}

std::string DataStorage::getValueFromOneAvailableChild(int childNumber, std::string keyName)
{
    return availableChildrenData[childNumber][keyName.c_str()].GetString();
}

bool DataStorage::parseAvailableChildren(std::string responseData)
{
    availableChildrenData.Parse(responseData.c_str());
    return true;
}

bool DataStorage::parseChildLoginData(std::string responseData)
{
    childProfile = true;
    childLoginData.Parse(responseData.c_str());
    return true;
}

bool DataStorage::parseContentData(std::string responseString)
{
    contentData.Parse(responseString.c_str());
    
    rapidjson::Value::MemberIterator M;
    const char *key;//,*value;
    
    if (contentData.HasParseError())
    {
        CCLOG("Json has errors!!!");
        return false;
    }
    
    for (M=contentData["contentItems"].MemberBegin(); M!=contentData["contentItems"].MemberEnd(); M++)
    {
        key   = M->name.GetString();
        
        if (key!=NULL)
        {
            contentDataJsonKeys.push_back(key);
        }
    }

    
    return true;
}

bool DataStorage::parseDownloadCookies(std::string responseString)
{
    size_t beginpos = responseString.find("Set-Cookie:");
    responseString = responseString.substr(beginpos);
    responseString = responseString.substr(12);
    size_t endpos = responseString.find("\n");
    responseString = responseString.substr(0, endpos);
    responseString = replaceAll(responseString, ", ", "\n");
    
    dataDownloadCookies = responseString;

    
    return true;
}

std::string DataStorage::replaceAll(std::string& str, const std::string& from, const std::string& to) {
    if(from.empty())
        return "";
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
    
    return str;
}
