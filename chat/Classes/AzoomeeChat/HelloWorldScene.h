#ifndef __AzoomeeChat_HelloWorldScene__
#define __AzoomeeChat_HelloWorldScene__

#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>


namespace Azoomee { namespace Chat
{
    
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

    
}}

#endif
