//
//  Oomee.h
//  AzoomeeOomeeMaker
//
//  Created by Macauley on 14/02/2018.
//

#ifndef Oomee_h
#define Oomee_h

#include "../AzoomeeOomeeMaker.h"
#include <cocos/cocos2d.h>
#include <AzoomeeCommon/Data/Json.h>

NS_AZOOMEE_OM_BEGIN

class Oomee;
typedef std::shared_ptr<Oomee> OomeeRef;

class Oomee
{
private:
    std::string _id;
    std::map<std::string, cocos2d::Vec2> _anchorPoints;
    cocos2d::Vec2 _position;
    float _scale;
    float _sizeMultiplier;
    
public:
    OomeeRef create();
    OomeeRef createWithData(const rapidjson::Document& oomeeConfig);
    Oomee();
    
    void initWithData(const rapidjson::Document& oomeeConfig);
    
    void setId(const std::string& id);
    std::string getId() const;
    void setAnchorPoints(const std::map<std::string, cocos2d::Vec2>& anchorPoints);
    std::map<std::string, cocos2d::Vec2> getAnchorPoints() const;
    void setPosition(const cocos2d::Vec2& position);
    cocos2d::Vec2 getPosition() const;
    void setScale(float scale);
    float getScale() const;
    void setSizeMultiplier(float sizeMultiplier);
    float getSizeMultiplier() const;
    
    
};

NS_AZOOMEE_OM_END

#endif /* Oomee_h */
