#ifndef __OOMEE_SCENE_H__
#define __OOMEE_SCENE_H__

#include "cocos2d.h"
#include <spine/spine-cocos2dx.h>

class OomeeLayer : public cocos2d::Layer
{
public:
    CREATE_FUNC(OomeeLayer);
    static cocos2d::Scene* createScene();
    virtual bool init();
    
private:
    spine::SkeletonAnimation* addOomeeToScreen();
    spine::SkeletonAnimation* addOomeeToScreenQuick();
    void addTouchListenerToOomee(spine::SkeletonAnimation* toBeAddedTo);
    void addCompleteListenerToOomee(spine::SkeletonAnimation* toBeAddedTo);
    void setDisplayedOomee(int oomeeNumber);
    
    int displayedOomeeNumber;
};

#endif
