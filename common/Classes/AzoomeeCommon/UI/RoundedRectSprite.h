//
//  RoundedRectSprite.h
//  AzoomeeCommon
//
//  Created by Macauley.Scoffins on 27/09/2019.
//

#ifndef RoundedRectSprite_h
#define RoundedRectSprite_h

#include "../Azoomee.h"
#include <cocos/cocos2d.h>

NS_AZOOMEE_BEGIN

class RoundedRectSprite : public cocos2d::Sprite
{
    typedef cocos2d::Sprite Super;
public:
    
    bool init() override;
    void setContentSize(const cocos2d::Size& contentSize) override;
    
    void setCornerRadius(float radius);
    
    CREATE_FUNC(RoundedRectSprite);
};

NS_AZOOMEE_END

#endif /* RoundedRectSprite_h */
