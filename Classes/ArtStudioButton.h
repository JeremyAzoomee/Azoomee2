//
//  ArtStudioButton.h
//  Azoomee
//
//  Created by Macauley.Scoffins on 11/10/2019.
//

#ifndef ArtStudioButton_h
#define ArtStudioButton_h

#include <AzoomeeCommon/Azoomee.h>
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include <AzoomeeCommon/UI/RoundedRectSprite.h>

NS_AZOOMEE_BEGIN

class ArtStudioButton : public cocos2d::ui::Button
{
    typedef cocos2d::ui::Button Super;
private:
    static const cocos2d::Size kOverflowPadding;
    static const cocos2d::Size kFramePadding;
    
    RoundedRectSprite* _frame = nullptr;
    RoundedRectSprite* _background = nullptr;
    cocos2d::Sprite* _tools = nullptr;
    cocos2d::Sprite* _logo = nullptr;
    RoundedRectSprite* _paint = nullptr;
    
public:
    
    bool init() override;
    void onSizeChanged() override;
    
    CREATE_FUNC(ArtStudioButton);
};

NS_AZOOMEE_END

#endif /* ArtStudioButton_h */
