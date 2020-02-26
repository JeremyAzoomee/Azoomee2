//
//  SettingsMessageBox.h
//  azoomee2
//
//  Created by Macauley on 05/09/2018.
//

#ifndef SettingsMessageBox_h
#define SettingsMessageBox_h

#include <AzoomeeCommon/Tinizine.h>
#include <cocos/cocos2d.h>
#include <ui/CocosGUI.h>
#include "Azoomee.h"

USING_NS_TZ

NS_AZ_BEGIN

class SettingsMessageBox;

enum class SettingsMessageBoxButtonType {CANCEL = 0, CLOSE, DELETE, REJECT, SEND, TRY_AGAIN};

class SettingsMessageBoxButton : public cocos2d::ui::Button
{
private:
    SettingsMessageBoxButtonType _type;
public:
    void setMessageBoxButtonType(SettingsMessageBoxButtonType type){ _type = type; }
    SettingsMessageBoxButtonType getMessageBoxButtonType() const { return _type; }
    
    CREATE_FUNC(SettingsMessageBoxButton);
};

class SettingsMessageBoxDelegate
{
public:
   virtual void onButtonPressed(SettingsMessageBox* pSender, SettingsMessageBoxButtonType type) = 0;
};

class SettingsMessageBox :public cocos2d::Node
{
    typedef cocos2d::Node Super;
private:
    cocos2d::LayerColor* _overlay = nullptr;
    
protected:
    cocos2d::ui::Layout* _messageBox = nullptr;
    cocos2d::Size _visibleSize;
    
    SettingsMessageBoxDelegate* _delegate = nullptr;
    
public:
    
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;
    
    void setDelegate(SettingsMessageBoxDelegate* delegate);
    
    void onButtonPressed(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType eType);
    
    CREATE_FUNC(SettingsMessageBox);
};

NS_AZ_END

#endif /* SettingsMessageBox_h */
