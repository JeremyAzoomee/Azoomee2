//
//  OomeeFigure.h
//  AzoomeeOomeeMaker
//
//  Created by Macauley on 16/02/2018.
//

#ifndef OomeeFigure_h
#define OomeeFigure_h

#include "../AzoomeeOomeeMaker.h"
#include "../DataObjects/Oomee.h"
#include "../DataObjects/OomeeItem.h"
#include <cocos/cocos2d.h>

NS_AZOOMEE_OM_BEGIN

class OomeeFigure : public cocos2d::Node
{
    typedef cocos2d::Node Super;
private:
    OomeeRef _oomeeData = nullptr;
    cocos2d::Sprite* _baseSprite = nullptr;
    std::map<std::string, cocos2d::Sprite*> _accessorySprites;
    std::map<std::string, OomeeItemRef> _accessoryData;
    
public:
    virtual bool init() override;
    virtual void onEnter() override;
    
    void setOomeeData(const OomeeRef& oomeeData);
    void addAccessory(const OomeeItemRef& oomeeItem);
    void removeAccessory(const std::string anchorPoint);
    
    CREATE_FUNC(OomeeFigure);
};

NS_AZOOMEE_OM_END

#endif /* OomeeFigure_h */
