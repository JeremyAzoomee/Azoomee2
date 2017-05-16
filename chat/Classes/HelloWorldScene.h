#ifndef AzoomeeChat_HelloWorldScene_h
#define AzoomeeChat_HelloWorldScene_h

#include "AzoomeeChat/AzoomeeChat.h"
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>


NS_AZOOMEE_CHAT_BEGIN
    
class HelloWorldScene : public cocos2d::Scene
{
    typedef cocos2d::Scene Super;
private:
    
    /// Root layout for all elements
    cocos2d::ui::Layout* _parentLayout = nullptr;
    
    /// Listener for window changes
    cocos2d::EventListenerCustom* _windowChangedEvent = nullptr;
    
    /// Called when projection/size changes
    void onWindowChanged(cocos2d::EventCustom* event);
    
public:
    
    virtual ~HelloWorldScene();
    
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void onExit() override;

    CREATE_FUNC(HelloWorldScene);
};

NS_AZOOMEE_CHAT_END

#endif
