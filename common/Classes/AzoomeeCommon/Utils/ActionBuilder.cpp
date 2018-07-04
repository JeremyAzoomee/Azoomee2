//
//  ActionBuilder.cpp
//  AzoomeeCommon
//
//  Created by Macauley on 13/02/2018.
//

#include "ActionBuilder.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

Sequence* createFadeInAndOutEffect(float totalDuration, int fadeInOpacity, int fadeOutOpacity)
{
    return Sequence::create( EaseOut::create( FadeTo::create(totalDuration / 2.0f, fadeOutOpacity), 1), EaseIn::create( FadeTo::create(totalDuration / 2.0f, fadeInOpacity), 1), nullptr);
}

Sequence* createFlickerEffect(int flickerCount, float delayBetweenFlickers)
{
    return Sequence::create( Repeat::create(Sequence::create(EaseElasticOut::create(FadeTo::create(0.3, 50)), EaseElasticOut::create(FadeTo::create(0.3, 255)), nullptr), flickerCount), DelayTime::create(delayBetweenFlickers), nullptr);
}

cocos2d::Sequence* createBlinkEffect(float startDelay, float blinkDelay)
{
    return Sequence::create(DelayTime::create(startDelay), FadeTo::create(0, 255), DelayTime::create(blinkDelay), FadeTo::create(0, 0), DelayTime::create(blinkDelay), FadeTo::create(0, 255), NULL);
}

NS_AZOOMEE_END
