#ifndef __EMPTYSCENEFORSETTINGS_SCENE_H__
#define __EMPTYSCENEFORSETTINGS_SCENE_H__

#include <cocos/cocos2d.h>
#include "SettingsControlLayer.h"

NS_AZOOMEE_BEGIN

class EmptySceneForSettings : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    void onEnterTransitionDidFinish();
    
    CREATE_FUNC(EmptySceneForSettings);
};

NS_AZOOMEE_END

#endif
