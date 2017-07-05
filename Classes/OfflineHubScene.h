#ifndef __OFFLINEHUB_SCENE_H__
#define __OFFLINEHUB_SCENE_H__

#include <cocos/cocos2d.h>
#include <AzoomeeCommon/Azoomee.h>

NS_AZOOMEE_BEGIN

class OfflineHubScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    
    CREATE_FUNC(OfflineHubScene);
    
    virtual bool init();
    void onEnterTransitionDidFinish();
    
private:
    
    void addVisuals();
    void addHubHQ();
    void addOfflineText();

};

NS_AZOOMEE_END

#endif
