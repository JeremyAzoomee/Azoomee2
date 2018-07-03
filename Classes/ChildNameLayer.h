//
//  ChildNameLayer.h
//  azoomee2
//
//  Created by Macauley on 03/07/2018.
//

#ifndef ChildNameLayer_h
#define ChildNameLayer_h

#include <AzoomeeCommon/Azoomee.h>
#include <cocos/cocos2d.h>
#include <AzoomeeCommon/Input/TextInputLayer.h>
#include "AddChildLayer.h"

NS_AZOOMEE_BEGIN

class ChildNameLayer : public AddChildLayer
{
    typedef cocos2d::Layer Super;
private:
    
    
public:
    
    virtual bool init() override;
    virtual void onEnter() override;
    
    CREATE_FUNC(ChildNameLayer);
};

NS_AZOOMEE_END

#endif /* ChildNameLayer_h */
