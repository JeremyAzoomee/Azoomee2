//
//  ItemCategory.h
//  AzoomeeOomeeMaker
//
//  Created by Macauley on 14/02/2018.
//

#ifndef ItemCategory_h
#define ItemCategory_h

#include "../AzoomeeOomeeMaker.h"
#include <cocos/cocos2d.h>
#include <AzoomeeCommon/Data/Json.h>

NS_AZOOMEE_OM_BEGIN

class ItemCategory;
typedef std::shared_ptr<ItemCategory> ItemCategoryRef;

enum HolidayCalenderID{NONE, XMAS, EASTER, HALLOWEEN};

HolidayCalenderID getHolidayCalenderIDFromString(const std::string& holidayStr);

class ItemCategory
{
private:
    std::string _id;
    std::string _iconFilename;
    std::vector<HolidayCalenderID> _holidaySeasonList;
    float _scale;
    cocos2d::Color4B _highlightColour;
    
public:
    ItemCategoryRef create();
    ItemCategoryRef createWithData(const rapidjson::Document& categoryConfig);
    ItemCategory();
    
    void initWithData(const rapidjson::Document& categoryConfig);
    
    void setId(const std::string& id);
    std::string getId() const;
    void setIconFilename(const std::string& iconFilename);
    std::string getIconFilename() const;
    void setHolidaySeasons(const std::vector<HolidayCalenderID>& holidaySeasons);
    std::vector<HolidayCalenderID> getHolidaySeasons() const;
    void setScale(float scale);
    float getScale() const;
    void setHighlightColour(const cocos2d::Color4B& colour);
    cocos2d::Color4B getHightlightColour() const;
    
};

NS_AZOOMEE_OM_END

#endif /* ItemCategory_h */
