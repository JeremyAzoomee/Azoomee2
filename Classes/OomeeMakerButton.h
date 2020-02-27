//
//  OomeeMakerButton.h
//  Azoomee
//
//  Created by Macauley.Scoffins on 11/10/2019.
//

#ifndef OomeeMakerButton_h
#define OomeeMakerButton_h

#include <TinizineCommon/Tinizine.h>
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include <TinizineCommon/UI/RoundedRectSprite.h>
#include "Azoomee.h"

USING_NS_TZ

NS_AZ_BEGIN

class OomeeMakerButton : public cocos2d::ui::Button
{
    typedef cocos2d::ui::Button Super;
private:
    static const cocos2d::Size kOverflowPadding;
    static const cocos2d::Size kFramePadding;
    
    RoundedRectSprite* _frame = nullptr;
    RoundedRectSprite* _background = nullptr;
    RoundedRectSprite* _oomees = nullptr;
    
public:
    
    bool init() override;
    void onSizeChanged() override;
    
    CREATE_FUNC(OomeeMakerButton);
};

NS_AZ_END
#endif /* OomeeMakerButton_h */
