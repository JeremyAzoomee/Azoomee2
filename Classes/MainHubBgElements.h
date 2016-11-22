#ifndef __MAINHUBBGELEMENTS_SCENE_H__
#define __MAINHUBBGELEMENTS_SCENE_H__

#include "cocos2d.h"

class MainHubBgElements : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(MainHubBgElements);
};

#endif // __MAINHUBBGELEMENTS__
