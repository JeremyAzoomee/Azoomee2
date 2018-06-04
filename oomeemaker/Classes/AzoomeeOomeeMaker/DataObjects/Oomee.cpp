//
//  Oomee.cpp
//  AzoomeeOomeeMaker-mobile
//
//  Created by Macauley on 15/02/2018.
//

#include "Oomee.h"

NS_AZOOMEE_OM_BEGIN

OomeeRef Oomee::create()
{
    return std::make_shared<Oomee>();
}

OomeeRef Oomee::createWithData(const rapidjson::Document& oomeeConfig)
{
    OomeeRef oomee = std::make_shared<Oomee>();
    oomee->initWithData(oomeeConfig);
    return oomee;
}

Oomee::Oomee()
{
    
}

void Oomee::initWithData(const rapidjson::Document& oomeeConfig)
{
    setId(getStringFromJson("id", oomeeConfig));
    setAnchorPoints(getVec2MapFromJson(oomeeConfig["anchorPoints"]));
    setLockedAnchors(getStringArrayFromJson(oomeeConfig["lockedAnchors"]));
    setPosition(getVec2FromJson("position", oomeeConfig));
    setScale(getFloatFromJson("scale", oomeeConfig));
    setSizeMultiplier(getFloatFromJson("sizeMultiplier", oomeeConfig));
    if(oomeeConfig.HasMember("assetSet"))
    {
        setAssetSet(getStringMapFromJson(oomeeConfig["assetSet"]));
    }
}

// GETTERS AND SETTERS

void Oomee::setId(const std::string& id)
{
    _id = id;
}
std::string Oomee::getId() const
{
    return _id;
}

void Oomee::setAnchorPoints(const std::map<std::string, cocos2d::Vec2>& anchorPoints)
{
    _anchorPoints = anchorPoints;
}
std::map<std::string, cocos2d::Vec2> Oomee::getAnchorPoints() const
{
    return _anchorPoints;
}

void Oomee::setLockedAnchors(const std::vector<std::string> lockedAnchors)
{
    _lockedAnchors = lockedAnchors;
}
std::vector<std::string> Oomee::getLockedAnchors() const
{
    return _lockedAnchors;
}

void Oomee::setPosition(const cocos2d::Vec2& position)
{
    _position = position;
}
cocos2d::Vec2 Oomee::getPosition() const
{
    return _position;
}

void Oomee::setScale(float scale)
{
    _scale = scale;
}
float Oomee::getScale() const
{
    return _scale;
}

void Oomee::setSizeMultiplier(float sizeMultiplier)
{
    _sizeMultiplier = sizeMultiplier;
}
float Oomee::getSizeMultiplier() const
{
    return _sizeMultiplier;
}

void Oomee::setAssetSet(const std::map<std::string, std::string> assetSet)
{
    _assetSet = assetSet;
}

std::map<std::string, std::string> Oomee::getAssetSet() const
{
    return _assetSet;
}

NS_AZOOMEE_OM_END
