//
//  ButtonActionData.cpp
//  azoomee2
//
//  Created by Macauley on 25/09/2017.
//
//

#include "ButtonActionData.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

ButtonActionDataRef ButtonActionData::createWithJson(const rapidjson::Value &actionData)
{
    if(!actionData.IsObject() || !actionData.HasMember("type") || !actionData["type"].IsString())
    {
        return ButtonActionDataRef();
    }
    
    const std::string& type = actionData["type"].GetString();
    
    if(!actionData.HasMember("params") || !actionData["params"].IsObject())
    {
        return ButtonActionDataRef();
    }
    
    std::map<std::string, std::string> params;
    
    for (auto M = actionData["params"].MemberBegin(); M!=actionData["params"].MemberEnd(); M++)
    {
        const char *key = M->name.GetString();
        
        if (key!=NULL)
        {
            const rapidjson::Value &itemData = actionData["params"][key];
            if(itemData.IsString())
            {
                params[key] = itemData.GetString();
            }
        }
    }
    
    ButtonActionDataRef buttonActionData(new ButtonActionData);
    buttonActionData->_type = type;
    buttonActionData->_params = params;
    
    return buttonActionData;
}

ButtonActionData::ButtonActionData()
{
    _type = "";
    _params = std::map<std::string,std::string>();
}

std::string ButtonActionData::getType()
{
    return _type;
}

std::map<std::string, std::string> ButtonActionData::getParams()
{
    return _params;
}

void ButtonActionData::setParams(const std::map<std::string,std::string>& updatedParams)
{
    _params = updatedParams;
}

std::string ButtonActionData::getParamForKey(const std::string& key)
{
    try{
        std::string value = _params.at(key);
        return value;
    }
    catch(std::out_of_range){
        return "";
    }
}

NS_AZOOMEE_END
