//
//  OomeeFigureData.h
//  AzoomeeOomeeMaker
//
//  Created by Macauley on 18/06/2019.
//

#ifndef OomeeFigureData_h
#define OomeeFigureData_h

#include "../AzoomeeOomeeMaker.h"
#include <AzoomeeCommon/Data/Json.h>

NS_AZOOMEE_OM_BEGIN

class OomeeFigureData;
typedef std::shared_ptr<OomeeFigureData> OomeeFigureDataRef;

class OomeeFigureData
{
private:
	std::string _id = "";
	std::string _oomeeId;
	std::vector<std::string> _accessoryIds;
    bool _selected = false;

public:
	
	static OomeeFigureDataRef create();
	static OomeeFigureDataRef createWithData(const rapidjson::Value& oomeeConfig);
	OomeeFigureData();
	
	void initWithData(const rapidjson::Value& oomeeConfig);
	
	void setId(const std::string& id);
	std::string getId() const;
	void setOomeeId(const std::string& oomeeId);
	std::string getOomeeId() const;
	void setAccessoryIds(const std::vector<std::string>& accessoryIds);
	std::vector<std::string> getAccessoryIds() const;
    void setSelected(bool selected);
    bool isSelected() const;
    
	bool isEqual(const OomeeFigureDataRef& comparable);
};

NS_AZOOMEE_OM_END

#endif /* OomeeFigureData_h */
