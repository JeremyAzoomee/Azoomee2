//
//  AssetData.cpp
//  AzoomeeOomeeMaker
//
//  Created by Macauley on 15/03/2019.
//

#include "AssetData.h"

USING_NS_TZ

NS_AZ_OM_BEGIN

AssetSet AssetData::createAssetSet(const rapidjson::Value &assetSetData)
{
	AssetSet assets;
	if(assetSetData.IsArray())
	{
		for(int i = 0; i < assetSetData.Size(); i++)
		{
			assets.push_back(AssetData(assetSetData[i]));
		}
	}
	else
	{
		const auto& map = getAssetMapFromJson(assetSetData);
		for(const auto& asset : map)
		{
			assets.push_back(AssetData(asset.first, asset.second.first, asset.second.second));
		}
	}
	return assets;
}

AssetData::AssetData(const std::string& tag, const std::string& location, int zOrder)
{
	_tag = tag;
	_location = location;
	_zOrder = zOrder;
}

AssetData::AssetData(const rapidjson::Value& assetData)
{
	_tag = getStringFromJson("tag", assetData);
	_location = getStringFromJson("location", assetData);
	_zOrder = getIntFromJson("index", assetData, 0);
}

std::string AssetData::getTag() const
{
	return _tag;
}
std::string AssetData::getLocation() const
{
	return _location;
}
int AssetData::getZOrder() const
{
	return _zOrder;
}

NS_AZ_OM_END
