//
//  OomeeItem.h
//  AzoomeeOomeeMaker
//
//  Created by Macauley on 14/02/2018.
//

#ifndef OomeeItem_h
#define OomeeItem_h

#include "../AzoomeeOomeeMaker.h"
#include <cocos/cocos2d.h>
#include <AzoomeeCommon/Data/Json.h>

NS_AZOOMEE_OM_BEGIN

class OomeeItem;
typedef std::shared_ptr<OomeeItem> OomeeItemRef;

class OomeeItem
{
private:
    std::string _id;
    std::string _targetAnchor;
    cocos2d::Vec2 _offset;
    cocos2d::Vec2 _snapRange;
    float _menuScale;
    float _dragScale;
    float _targetScale;
    int _zOrder;
    std::string _categoryId;
    std::string _assetFilename;
    
    
public:
    static OomeeItemRef create();
    static OomeeItemRef createWithData(const rapidjson::Value& itemConfig);
    OomeeItem();
    
    void initWithData(const rapidjson::Value& itemConfig);
    
    void setId(const std::string& id);
    std::string getId() const;
    void setTargetAnchor(const std::string& targetAnchor);
    std::string getTargetAnchor() const;
    void setOffset(const cocos2d::Vec2& offset);
    cocos2d::Vec2 getOffset() const;
    void setSnapRange(const cocos2d::Vec2& snapRange);
    cocos2d::Vec2 getSnapRange() const;
    void setMenuScale(float menuScale);
    float getMenuScale() const;
    void setDragScale(float dragScale);
    float getDragScale() const;
    void setTargetScale(float targetScale);
    float getTargetScale() const;
    void setZOrder(int zOrder);
    int getZOrder() const;
    void setCategoryId(const std::string& categoryId);
    std::string getCategoryId() const;
    void setAssetFilename(const std::string& assetName);
    std::string getAssetName() const;
    
};

NS_AZOOMEE_OM_END

#endif /* OomeeItem_h */
