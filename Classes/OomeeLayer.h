#ifndef __OOMEE_SCENE_H__
#define __OOMEE_SCENE_H__

#include "cocos2d.h"
#include "cocos2d.h"
#include <spine/spine-cocos2dx.h>

class OomeeLayer : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    spine::SkeletonAnimation* addOomeeToScreen();
    void addTouchListenerToOomee(spine::SkeletonAnimation* toBeAddedTo);
    void addCompleteListenerToOomee(spine::SkeletonAnimation* toBeAddedTo);
    
    
    CREATE_FUNC(OomeeLayer);
};

#endif
