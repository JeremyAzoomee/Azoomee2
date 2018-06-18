#ifndef __EMPTYSCENEFORSETTINGS_SCENE_H__
#define __EMPTYSCENEFORSETTINGS_SCENE_H__

#include <cocos/cocos2d.h>
#include "SettingsControlLayer.h"

NS_AZOOMEE_BEGIN

enum SettingsOrigin { CHAT, MAIN_APP};

class EmptySceneForSettings : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene(SettingsOrigin origin = CHAT);
    virtual bool init();
    void onEnterTransitionDidFinish();
    
    SettingsOrigin _origin;
    
    CREATE_FUNC(EmptySceneForSettings);
};

NS_AZOOMEE_END

#endif
