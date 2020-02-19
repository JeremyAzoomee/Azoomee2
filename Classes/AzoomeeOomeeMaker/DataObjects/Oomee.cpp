//
//  Oomee.cpp
//  AzoomeeOomeeMaker-mobile
//
//  Created by Macauley on 15/02/2018.
//

#include "Oomee.h"

USING_NS_TZ

NS_AZ_OM_BEGIN

OomeeRef Oomee::create()
{
    return std::make_shared<Oomee>();
}

OomeeRef Oomee::createWithData(const rapidjson::Value& oomeeConfig)
{
    OomeeRef oomee = std::make_shared<Oomee>();
    oomee->initWithData(oomeeConfig);
    return oomee;
}

Oomee::Oomee()
{
    
}

void Oomee::initWithData(const rapidjson::Value& oomeeConfig)
{
    setId(getStringFromJson("id", oomeeConfig));
    if(oomeeConfig.HasMember("anchorPoints"))
    {
        setAnchorPoints(getVec2MapFromJson(oomeeConfig["anchorPoints"]));
    }
    if(oomeeConfig.HasMember("lockedAnchors"))
    {
        setLockedAnchors(getStringArrayFromJson(oomeeConfig["lockedAnchors"]));
    }
    if(oomeeConfig.HasMember("defaultAccessories"))
    {
        setDefaultAccessories(getStringArrayFromJson(oomeeConfig["defaultAccessories"]));
    }
    if(oomeeConfig.HasMember("incompatibleAccessories"))
    {
        setIncompatableAccessories(getStringArrayFromJson(oomeeConfig["incompatibleAccessories"]));
    }
    setPosition(getVec2FromJson("position", oomeeConfig));
    setScale(getFloatFromJson("scale", oomeeConfig));
    setSizeMultiplier(getFloatFromJson("sizeMultiplier", oomeeConfig));
    if(oomeeConfig.HasMember("assetSet"))
    {
		setAssetSet(AssetData::createAssetSet(oomeeConfig["assetSet"]));
    }
	if(oomeeConfig.HasMember("colours"))
	{
		_colour = OomeeColour::createWithData(oomeeConfig);
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

void Oomee::setLockedAnchors(const std::vector<std::string>& lockedAnchors)
{
    _lockedAnchors = lockedAnchors;
}
std::vector<std::string> Oomee::getLockedAnchors() const
{
    return _lockedAnchors;
}

void Oomee::setDefaultAccessories(const std::vector<std::string>& defaultAccessories)
{
    _defaultAccessories = defaultAccessories;
}
std::vector<std::string> Oomee::getDefaultAccessories() const
{
    return _defaultAccessories;
}

void Oomee::setIncompatableAccessories(const std::vector<std::string>& incompatableAccessories)
{
    _incompatibleAccessories = incompatableAccessories;
}
std::vector<std::string> Oomee::getIncompatableAccessories() const
{
    return _incompatibleAccessories;
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

void Oomee::setAssetSet(const AssetSet& assetSet)
{
    _assetSet = assetSet;
}

AssetSet Oomee::getAssetSet() const
{
    return _assetSet;
}

OomeeColourRef Oomee::getColour() const
{
	return _colour;
}

NS_AZ_OM_END
