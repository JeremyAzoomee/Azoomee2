//
//  ActionBuilder.h
//  AzoomeeCommon
//
//  Created by Macauley on 13/02/2018.
//

#ifndef ActionBuilder_h
#define ActionBuilder_h

#include <AzoomeeCommon/Azoomee.h>
#include <cocos/cocos2d.h>

NS_AZOOMEE_BEGIN
namespace ActionBuilder
{
    cocos2d::Sequence* createFadeInAndOutEffect(float totalDuration, int fadeInOpacity, int fadeOutOpacity);
    cocos2d::Sequence* createFlickerEffect(int flickerCount, float delayBetweenFlickers);
    cocos2d::Sequence* createBlinkEffect(float startDelay, float blinkDelay);
}
NS_AZOOMEE_END

#endif /* ActionBuilder_h */
