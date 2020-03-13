//
//  ActionBuilder.h
//  TinizineCommon
//
//  Created by Macauley on 13/02/2018.
//

#ifndef ActionBuilder_h
#define ActionBuilder_h

#include <TinizineCommon/Tinizine.h>
#include <cocos/cocos2d.h>
#include "Azoomee.h"

USING_NS_TZ

NS_AZ_BEGIN
namespace ActionBuilder
{
    cocos2d::Sequence* createFadeInAndOutEffect(float totalDuration, int fadeInOpacity, int fadeOutOpacity);
    cocos2d::Sequence* createFlickerEffect(int flickerCount, float delayBetweenFlickers);
    cocos2d::Sequence* createBlinkEffect(float startDelay, float blinkDelay);
}
NS_AZ_END

#endif /* ActionBuilder_h */
