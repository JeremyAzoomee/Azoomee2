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
#include "AssetData.h"
#include "OomeeColour.h"

USING_NS_TZ

NS_AZ_OM_BEGIN

class Oomee;
typedef std::shared_ptr<Oomee> OomeeRef;

class Oomee
{
private:
    std::string _id;
    std::map<std::string, cocos2d::Vec2> _anchorPoints;
    std::vector<std::string> _lockedAnchors;
    std::vector<std::string> _defaultAccessories;
    std::vector<std::string> _incompatibleAccessories;
    cocos2d::Vec2 _position;
    float _scale;
    float _sizeMultiplier;
    AssetSet _assetSet;
	OomeeColourRef _colour = nullptr;
    
public:
    static OomeeRef create();
    static OomeeRef createWithData(const rapidjson::Value& oomeeConfig);
    Oomee();
    
    void initWithData(const rapidjson::Value& oomeeConfig);
    
    void setId(const std::string& id);
    std::string getId() const;
    void setAnchorPoints(const std::map<std::string, cocos2d::Vec2>& anchorPoints);
    std::map<std::string, cocos2d::Vec2> getAnchorPoints() const;
    void setLockedAnchors(const std::vector<std::string>& lockedAnchors);
    std::vector<std::string> getLockedAnchors() const;
    void setDefaultAccessories(const std::vector<std::string>& defaultAccessories);
    std::vector<std::string> getDefaultAccessories() const;
    void setIncompatableAccessories(const std::vector<std::string>& incompatableAccessories);
    std::vector<std::string> getIncompatableAccessories() const;
    void setPosition(const cocos2d::Vec2& position);
    cocos2d::Vec2 getPosition() const;
    void setScale(float scale);
    float getScale() const;
    void setSizeMultiplier(float sizeMultiplier);
    float getSizeMultiplier() const;
    void setAssetSet(const AssetSet& assetSet);
    AssetSet getAssetSet() const;
	OomeeColourRef getColour() const;
	
};

NS_AZ_OM_END

#endif /* Oomee_h */
