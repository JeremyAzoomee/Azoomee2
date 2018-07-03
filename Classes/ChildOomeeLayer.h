//
//  ChildOomeeLayer.h
//  azoomee2
//
//  Created by Macauley on 03/07/2018.
//

#ifndef ChildOomeeLayer_h
#define ChildOomeeLayer_h

#include <AzoomeeCommon/Azoomee.h>
#include <cocos/cocos2d.h>
#include <AzoomeeCommon/Input/TextInputLayer.h>
#include "AddChildLayer.h"

NS_AZOOMEE_BEGIN

class ChildOomeeLayer : public AddChildLayer
{
    typedef cocos2d::Layer Super;
private:
    
    
public:

    virtual bool init() override;
    virtual void onEnter() override;
    
    CREATE_FUNC(ChildOomeeLayer);
};

NS_AZOOMEE_END

#endif /* ChildOomeeLayer_h */
