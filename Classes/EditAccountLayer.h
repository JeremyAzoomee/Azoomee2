//
//  EditAccountLayer.h
//  azoomee2
//
//  Created by Macauley on 28/08/2018.
//

#ifndef EditAccountLayer_h
#define EditAccountLayer_h

#include <AzoomeeCommon/Azoomee.h>
#include <AzoomeeCommon/Input/TextInputLayer.h>
#include <cocos/cocos2d.h>
#include <ui/CocosGUI.h>

NS_AZOOMEE_BEGIN

class EditAccountLayer : public cocos2d::ui::Layout
{
    typedef cocos2d::ui::Layout Super;
private:
    cocos2d::ui::Layout* _accountTypeLayout = nullptr;
    cocos2d::ui::Text* _nameText = nullptr;
    cocos2d::ui::Button* _editNameButon = nullptr;
    cocos2d::ui::Text* _emailText = nullptr;
    TextInputLayer* _pinEditbox = nullptr;
    TextInputLayer* _passwordEditBox = nullptr;
    
    
public:
    
    virtual bool init() override;
    virtual void onEnter() override;
    
    CREATE_FUNC(EditAccountLayer);
};

NS_AZOOMEE_END

#endif /* EditAccountLayer_h */
