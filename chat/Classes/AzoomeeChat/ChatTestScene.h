#ifndef AzoomeeChat_ChatTestScene_h
#define AzoomeeChat_ChatTestScene_h

#include "AzoomeeChat.h"
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>


NS_AZOOMEE_CHAT_BEGIN
    
class ChatTestScene : public cocos2d::Scene
{
    typedef cocos2d::Scene Super;
private:
    
    /// Root layout for all elements
    cocos2d::ui::Layout* _parentLayout = nullptr;
    
public:
    
    virtual ~ChatTestScene();
    
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;

    CREATE_FUNC(ChatTestScene);
};

NS_AZOOMEE_CHAT_END

#endif
