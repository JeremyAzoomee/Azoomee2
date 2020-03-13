//
//  OomeeColour.h
//  AzoomeeOomeeMaker
//
//  Created by Macauley on 04/06/2018.
//

#ifndef OomeeColour_h
#define OomeeColour_h

#include "../AzoomeeOomeeMaker.h"
#include <cocos/cocos2d.h>
#include <TinizineCommon/Data/Json.h>

USING_NS_TZ

NS_AZ_OM_BEGIN

class OomeeColour;
typedef std::shared_ptr<OomeeColour> OomeeColourRef;

class OomeeColour
{
private:
    std::string _id;
    std::map<std::string,cocos2d::Color4B> _colourMap;
    
    
public:
    static OomeeColourRef create();
    static OomeeColourRef createWithData(const rapidjson::Value& colourConfig);
    OomeeColour();
    
    void initWithData(const rapidjson::Value& colourConfig);
    
    void setId(const std::string& id);
    std::string getId() const;
    
    void setColours(const std::map<std::string, cocos2d::Color4B>& colourSet);
    std::map<std::string, cocos2d::Color4B> getColours() const;
    
};

NS_AZ_OM_END


#endif /* OomeeColour_h */
