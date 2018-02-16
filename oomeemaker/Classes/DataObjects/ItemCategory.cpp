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

ItemCategoryRef ItemCategory::createWithData(const rapidjson::Document& categoryConfig)
{
    ItemCategoryRef category = std::make_shared<ItemCategory>();
    category->initWithData(categoryConfig);
    return category;
}

ItemCategory::ItemCategory()
{
    
}

void ItemCategory::initWithData(const rapidjson::Document& categoryConfig)
{
    setId(getStringFromJson("id", categoryConfig));
    setIconFilename(getStringFromJson("iconFilename", categoryConfig));
    const std::vector<std::string>& holidayStrings = getStringArrayFromJson(categoryConfig["holidaySeasons"]);
    std::vector<HolidayCalenderID> holidayCalenderIds;
    for(const std::string& holidayStr : holidayStrings)
    {
        holidayCalenderIds.push_back(getHolidayCalenderIDFromString(holidayStr));
    }
    setHolidaySeasons(holidayCalenderIds);
    setScale(getFloatFromJson("scale", categoryConfig));
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

void ItemCategory::setIconFilename(const std::string& iconFilename)
{
    _iconFilename = iconFilename;
}
std::string ItemCategory::getIconFilename() const
{
    return _iconFilename;
}

void ItemCategory::setHolidaySeasons(const std::vector<HolidayCalenderID>& holidaySeasons)
{
    _holidaySeasonList = holidaySeasons;
}
std::vector<HolidayCalenderID> ItemCategory::getHolidaySeasons() const
{
    return _holidaySeasonList;
}

void ItemCategory::setScale(float scale)
{
    _scale = scale;
}
float ItemCategory::getScale() const
{
    return _scale;
}

void ItemCategory::setHighlightColour(const cocos2d::Color4B& colour)
{
    _highlightColour = colour;
}
cocos2d::Color4B ItemCategory::getHightlightColour() const
{
    return _highlightColour;
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
