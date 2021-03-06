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
    std::string _iconFilenameSelected;
    std::string _iconFilenameUnselected;
    std::vector<HolidayCalenderID> _holidaySeasonList;
    float _scaleSelected;
    float _scaleUnselected;
    cocos2d::Color4B _highlightColour;
	std::string _name;
    
public:
    static ItemCategoryRef create();
    static ItemCategoryRef createWithData(const rapidjson::Value& categoryConfig);
    ItemCategory();
    
    void initWithData(const rapidjson::Value& categoryConfig);
    
    void setId(const std::string& id);
    std::string getId() const;
    void setIconFilenameSelected(const std::string& iconFilename);
    std::string getIconFilenameSelected() const;
    void setIconFilenameUnselected(const std::string& iconFilename);
    std::string getIconFilenameUnselected() const;
    void setHolidaySeasons(const std::vector<HolidayCalenderID>& holidaySeasons);
    std::vector<HolidayCalenderID> getHolidaySeasons() const;
    void setScaleSelected(float scale);
    float getScaleSelected() const;
    void setScaleUnselected(float scale);
    float getScaleUnselected() const;
    void setHighlightColour(const cocos2d::Color4B& colour);
    cocos2d::Color4B getHightlightColour() const;
	void setName(const std::string& name);
	std::string getName() const;
    
};

NS_AZOOMEE_OM_END

#endif /* ItemCategory_h */
