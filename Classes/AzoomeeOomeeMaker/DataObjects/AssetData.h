//
//  AssetData.h
//  AzoomeeOomeeMaker
//
//  Created by Macauley on 15/03/2019.
//

#ifndef AssetData_h
#define AssetData_h

#include "../AzoomeeOomeeMaker.h"
#include <TinizineCommon/Data/Json.h>
#include <string>

USING_NS_TZ

NS_AZ_OM_BEGIN

class AssetData;
typedef std::vector<AssetData> AssetSet;

class AssetData
{
private:
	
	std::string _tag;
	std::string _location;
	int _zOrder;
	
public:
	static AssetSet createAssetSet(const rapidjson::Value& assetSetData);
	
	AssetData(const std::string& tag = "", const std::string& location = "", int zOrder = 0);
	AssetData(const rapidjson::Value& assetData);
	
	std::string getTag() const;
	std::string getLocation() const;
	int getZOrder() const;
};



NS_AZ_OM_END

#endif /* AssetData_h */
