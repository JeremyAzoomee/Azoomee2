#ifndef AzoomeeCommon_MessageBoxLayer_h
#define AzoomeeCommon_MessageBoxLayer_h

#include <cocos/cocos2d.h>
#include "../MessageBox.h"
#include "../../Azoomee.h"

NS_AZOOMEE_BEGIN

class MessageBoxLayer : public cocos2d::Layer
{
  
protected:
    
    Layer* _parentLayer;
    
    cocos2d::Size currentRunningSceneSize;
    void setCurrentRunningSceneSize();
    
    cocos2d::Layer* windowLayer;
    
public:
    
    /// Called when the content size of this scene has changed
    virtual void onSizeChanged();
  

};
  
NS_AZOOMEE_END

#endif
