#ifndef __MAINHUBBGELEMENTS_SCENE_H__
#define __MAINHUBBGELEMENTS_SCENE_H__

#include "cocos2d.h"

class MainHubBgElements : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    void onEnter();
    CREATE_FUNC(MainHubBgElements);
};

#endif
