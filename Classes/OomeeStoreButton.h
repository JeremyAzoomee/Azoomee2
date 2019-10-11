//
//  OomeeStoreButton.h
//  Azoomee
//
//  Created by Macauley.Scoffins on 11/10/2019.
//

#ifndef OomeeStoreButton_h
#define OomeeStoreButton_h

#include <AzoomeeCommon/Azoomee.h>
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>
#include <AzoomeeCommon/UI/RoundedRectSprite.h>

NS_AZOOMEE_BEGIN

class OomeeStoreButton : public cocos2d::ui::Button
{
    typedef cocos2d::ui::Button Super;
private:
    
    RoundedRectSprite* _frame = nullptr;
    RoundedRectSprite* _background = nullptr;
    cocos2d::Sprite* _oomees = nullptr;
    cocos2d::Sprite* _logo = nullptr;
    
public:
    
    bool init() override;
    void onSizeChanged() override;
    
    CREATE_FUNC(OomeeStoreButton);
};

NS_AZOOMEE_END

#endif /* OomeeStoreButton_h */
