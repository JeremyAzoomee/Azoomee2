#ifndef AzoomeeCommon_Scene_h
#define AzoomeeCommon_Scene_h

#include "../Azoomee.h"
#include "Style.h"
#include <cocos/cocos2d.h>
#include <cocos/ui/CocosGUI.h>


NS_AZOOMEE_BEGIN

/**
 * Can be used instead of cocos2d::Scene to ensure the scene content size 
 * uses only he visible size of the screen.
 */
class Scene : public cocos2d::Scene
{
private:
    typedef cocos2d::Scene Super;
    bool _initialised = false;
    
protected:
    
    /// Called when the content size of this scene has changed
    virtual void onContentSizeChanged(const cocos2d::Size& contentSize);
    
public:
    
    virtual bool init() override;
    
    virtual void setContentSize(const cocos2d::Size& contentSize) override;
};

NS_AZOOMEE_END

#endif
