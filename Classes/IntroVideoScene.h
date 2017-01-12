#ifndef __INTRO_VIDEO_SCENE_H__
#define __INTRO_VIDEO_SCENE_H__

#include "cocos2d.h"

class IntroVideoScene: public cocos2d::Layer
{
private:
    
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    
    // implement the "static create()" method manually
    CREATE_FUNC(IntroVideoScene);
};

#endif // __INTRO_VIDEO_SCENE_H__
