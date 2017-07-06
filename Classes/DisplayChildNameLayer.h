#ifndef __DISPLAY_CHILD_NAME_LAYER_H__
#define __DISPLAY_CHILD_NAME_LAYER_H__

#include <cocos/cocos2d.h>
#include <AzoomeeCommon/Azoomee.h>

NS_AZOOMEE_BEGIN

class DisplayChildNameLayer : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(DisplayChildNameLayer);

private:
    void createChildNameToLayer();
    void createLoggedInAsToLayer();
    void createInviteCodeLayer();
    void setLayerSizeAndAnimate();
    
    float delayTime;
    float scaleTime;
    
    cocos2d::Label* childNameLabel;
    cocos2d::Label* loggedInAsLabel;
    cocos2d::Label* inviteCodeLabel;
    
};

NS_AZOOMEE_END

#endif
