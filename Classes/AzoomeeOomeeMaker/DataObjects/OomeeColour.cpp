//
//  OomeeColour.cpp
//  AzoomeeOomeeMaker
//
//  Created by Macauley on 04/06/2018.
//

#include "OomeeColour.h"

USING_NS_TZ

NS_AZ_OM_BEGIN

OomeeColourRef OomeeColour::create()
{
    return std::make_shared<OomeeColour>();
}

OomeeColourRef OomeeColour::createWithData(const rapidjson::Value& colourConfig)
{
    OomeeColourRef item = std::make_shared<OomeeColour>();
    item->initWithData(colourConfig);
    return item;
}

OomeeColour::OomeeColour()
{
    
}

void OomeeColour::initWithData(const rapidjson::Value& colourConfig)
{
    setId(getStringFromJson("id", colourConfig));
    if(colourConfig.HasMember("colours"))
    {
        setColours(getColourMapFromJson(colourConfig["colours"]));
    }
}

// GETTERS AND SETTERS

void OomeeColour::setId(const std::string& id)
{
    _id = id;
}
std::string OomeeColour::getId() const
{
    return _id;
}

void OomeeColour::setColours(const std::map<std::string, cocos2d::Color4B> &colourSet)
{
    _colourMap = colourSet;
}

std::map<std::string, cocos2d::Color4B> OomeeColour::getColours() const
{
    return _colourMap;
}

NS_AZ_OM_END
