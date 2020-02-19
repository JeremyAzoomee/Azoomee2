//
//  OomeeAccessory.h
//  AzoomeeOomeeMaker
//
//  Created by Macauley on 04/06/2018.
//

#ifndef OomeeAccessory_h
#define OomeeAccessory_h

#include "../AzoomeeOomeeMaker.h"
#include "../DataObjects/OomeeItem.h"
#include "../DataObjects/OomeeColour.h"
#include <cocos/cocos2d.h>

USING_NS_TZ

NS_AZ_OM_BEGIN

class OomeeAccessory : public cocos2d::Node
{
    typedef cocos2d::Node Super;
private:
    static const std::map<std::string,int> kLayerOrderMap;
    
    OomeeItemRef _itemData = nullptr;
    OomeeColourRef _colours = nullptr;
    std::map<std::string,cocos2d::Sprite*> _sprites;
    
public:
    
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;
    
    void setItemData(const OomeeItemRef& itemData);
    void setColourData(const OomeeColourRef& colourData);
    
    OomeeItemRef getItemData() const;
    std::string getItemId() const;
    
    cocos2d::Sprite* getBaseSprite() const;
    
    CREATE_FUNC(OomeeAccessory);
};

NS_AZ_OM_END


#endif /* OomeeAccessory_h */
