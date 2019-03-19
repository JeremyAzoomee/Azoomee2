//
//  ItemCategory.cpp
//  AzoomeeOomeeMaker-mobile
//
//  Created by Macauley on 15/02/2018.
//

#include "ItemCategory.h"

NS_AZOOMEE_OM_BEGIN

ItemCategoryRef ItemCategory::create()
{
    return std::make_shared<ItemCategory>();
}

ItemCategoryRef ItemCategory::createWithData(const rapidjson::Value& categoryConfig)
{
    ItemCategoryRef category = std::make_shared<ItemCategory>();
    category->initWithData(categoryConfig);
    return category;
}

ItemCategory::ItemCategory()
{
    
}

void ItemCategory::initWithData(const rapidjson::Value& categoryConfig)
{
    setId(getStringFromJson("id", categoryConfig));
    setIconFilenameSelected(getStringFromJson("iconFilenameSelected", categoryConfig));
    setIconFilenameUnselected(getStringFromJson("iconFilenameUnselected", categoryConfig));
    const std::vector<std::string>& holidayStrings = getStringArrayFromJson(categoryConfig["holidaySeasons"]);
    std::vector<HolidayCalenderID> holidayCalenderIds;
    for(const std::string& holidayStr : holidayStrings)
    {
        holidayCalenderIds.push_back(getHolidayCalenderIDFromString(holidayStr));
    }
    setHolidaySeasons(holidayCalenderIds);
    setScaleSelected(getFloatFromJson("scaleSelected", categoryConfig));
    setScaleUnselected(getFloatFromJson("scaleUnselected", categoryConfig));
    setHighlightColour(getColor4BFromJson("highlightColour", categoryConfig));
}

// GETTERS AND SETTERS

void ItemCategory::setId(const std::string& id)
{
    _id = id;
}
std::string ItemCategory::getId() const
{
    return _id;
}

void ItemCategory::setIconFilenameSelected(const std::string& iconFilename)
{
    _iconFilenameSelected = iconFilename;
}
std::string ItemCategory::getIconFilenameSelected() const
{
    return _iconFilenameSelected;
}

void ItemCategory::setIconFilenameUnselected(const std::string& iconFilename)
{
    _iconFilenameUnselected = iconFilename;
}
std::string ItemCategory::getIconFilenameUnselected() const
{
    return _iconFilenameUnselected;
}

void ItemCategory::setHolidaySeasons(const std::vector<HolidayCalenderID>& holidaySeasons)
{
    _holidaySeasonList = holidaySeasons;
}
std::vector<HolidayCalenderID> ItemCategory::getHolidaySeasons() const
{
    return _holidaySeasonList;
}

void ItemCategory::setScaleSelected(float scale)
{
    _scaleSelected = scale;
}
float ItemCategory::getScaleSelected() const
{
    return _scaleSelected;
}

void ItemCategory::setScaleUnselected(float scale)
{
    _scaleUnselected = scale;
}
float ItemCategory::getScaleUnselected() const
{
    return _scaleUnselected;
}

void ItemCategory::setHighlightColour(const cocos2d::Color4B& colour)
{
    _highlightColour = colour;
}
cocos2d::Color4B ItemCategory::getHightlightColour() const
{
    return _highlightColour;
}

void ItemCategory::setName(const std::string& name)
{
	_name = name;
}
std::string ItemCategory::getName() const
{
	return _name;
}

HolidayCalenderID getHolidayCalenderIDFromString(const std::string& holidayStr)
{
    if(holidayStr == "xmas")
    {
        return XMAS;
    }
    else if(holidayStr == "easter")
    {
        return EASTER;
    }
    else if(holidayStr == "halloween")
    {
        return HALLOWEEN;
    }
    else
    {
        return NONE;
    }
}

NS_AZOOMEE_OM_END
