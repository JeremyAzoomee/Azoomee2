#ifndef AzoomeeChat_OomeeWidget_h
#define AzoomeeChat_OomeeWidget_h

#include "../AzoomeeChat.h"
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>


NS_AZOOMEE_CHAT_BEGIN
    
class OomeeWidget : public cocos2d::ui::Layout
{
    typedef cocos2d::ui::Layout Super;
private:
    
    
    
    
public:
    
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;

    CREATE_FUNC(OomeeWidget);
};

NS_AZOOMEE_CHAT_END

#endif
