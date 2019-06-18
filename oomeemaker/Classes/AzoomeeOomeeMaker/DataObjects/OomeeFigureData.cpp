//
//  OomeeFigureData.cpp
//  AzoomeeOomeeMaker
//
//  Created by Macauley on 18/06/2019.
//

#include "OomeeFigureData.h"

NS_AZOOMEE_OM_BEGIN

OomeeFigureDataRef OomeeFigureData::create()
{
	return std::make_shared<OomeeFigureData>();
}

OomeeFigureDataRef OomeeFigureData::createWithData(const rapidjson::Value& oomeeConfig)
{
	OomeeFigureDataRef oomee = std::make_shared<OomeeFigureData>();
	oomee->initWithData(oomeeConfig);
	return oomee;
}

OomeeFigureData::OomeeFigureData()
{
	
}

void OomeeFigureData::initWithData(const rapidjson::Value& oomeeConfig)
{
	setId(getStringFromJson("id", oomeeConfig));
	setOomeeId(getStringFromJson("oomeeId", oomeeConfig));
	if(oomeeConfig.HasMember("oomeeItems"))
	{
		setAccessoryIds(getStringArrayFromJson(oomeeConfig["oomeeItems"]));
	}
}

// GETTERS AND SETTERS

void OomeeFigureData::setId(const std::string& id)
{
	_id = id;
}
std::string OomeeFigureData::getId() const
{
	return _id;
}

void OomeeFigureData::setOomeeId(const std::string& oomeeId)
{
	_oomeeId = oomeeId;
}
std::string OomeeFigureData::getOomeeId() const
{
	return _oomeeId;
}

void OomeeFigureData::setAccessoryIds(const std::vector<std::string>& accessoryIds)
{
	_accessoryIds = accessoryIds;
}
std::vector<std::string> OomeeFigureData::getAccessoryIds() const
{
	return _accessoryIds;
}


NS_AZOOMEE_OM_END
