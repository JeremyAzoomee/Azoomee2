//
//  OomeeBody.h
//  AzoomeeOomeeMaker
//
//  Created by Macauley on 04/06/2018.
//

#ifndef OomeeBody_h
#define OomeeBody_h

#include "../AzoomeeOomeeMaker.h"
#include "../DataObjects/Oomee.h"
#include "../DataObjects/OomeeColour.h"
#include <cocos/cocos2d.h>

NS_AZOOMEE_OM_BEGIN

class OomeeBody : public cocos2d::Node
{
    typedef cocos2d::Node Super;
private:
    static const std::map<std::string,int> kLayerOrderMap;
    
    OomeeRef _oomeeData = nullptr;
    std::map<std::string,cocos2d::Sprite*> _sprites;
    
public:
    
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;
    
    void setOomeeData(const OomeeRef& oomeeData);
    
    int transformZOrder(int zOrder);
    
    CREATE_FUNC(OomeeBody);
};

NS_AZOOMEE_OM_END


#endif /* OomeeBody_h */
