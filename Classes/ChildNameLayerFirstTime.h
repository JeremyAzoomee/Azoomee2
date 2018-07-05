//
//  ChildNameLayerFirstTime.h
//  azoomee2
//
//  Created by Macauley on 03/07/2018.
//

#ifndef ChildNameLayerFirstTime_h
#define ChildNameLayerFirstTime_h

#include <AzoomeeCommon/Azoomee.h>
#include <cocos/cocos2d.h>
#include <AzoomeeCommon/Input/TextInputLayer.h>
#include "AddChildLayer.h"

NS_AZOOMEE_BEGIN

class ChildNameLayerFirstTime : public AddChildLayer
{
    typedef cocos2d::Layer Super;
private:
    TextInputLayer* _textInput = nullptr;
    
public:
    
    virtual bool init() override;
    virtual void onEnter() override;
    
    CREATE_FUNC(ChildNameLayerFirstTime);
};

NS_AZOOMEE_END

#endif /* ChildNameLayerFirstTime_h */
