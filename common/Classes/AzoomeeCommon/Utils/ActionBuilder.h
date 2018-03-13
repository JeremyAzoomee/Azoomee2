//
//  ActionBuilder.h
//  AzoomeeCommon
//
//  Created by Macauley on 13/02/2018.
//

#ifndef ActionBuilder_h
#define ActionBuilder_h

#include "../Azoomee.h"
#include <cocos/cocos2d.h>

NS_AZOOMEE_BEGIN

cocos2d::Sequence* createFadeInAndOutEffect(float totalDuration, int fadeInOpacity, int fadeOutOpacity);
cocos2d::Sequence* createFlickerEffect(int flickerCount, float delayBetweenFlickers);

NS_AZOOMEE_END

#endif /* ActionBuilder_h */
