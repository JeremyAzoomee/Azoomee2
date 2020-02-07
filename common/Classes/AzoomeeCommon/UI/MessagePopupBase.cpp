//
//  MessagePopupBase.cpp
//  AzoomeeCommon
//
//  Created by Macauley.Scoffins on 06/02/2020.
//

#include "MessagePopupBase.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

bool MessagePopupBase::init()
{
    if(!Super::init())
    {
        return false;
    }
    
    setSizeType(SizeType::PERCENT);
    setSizePercent(Vec2(1.0, 1.0));
    setBackGroundColor(Color3B::BLACK);
    setBackGroundColorType(BackGroundColorType::SOLID);
    setBackGroundColorOpacity(150);
    setSwallowTouches(true);
    setTouchEnabled(true);
    
    return true;
}

NS_AZOOMEE_END
