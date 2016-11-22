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
    
    // a selector callback
    
    // implement the "static create()" method manually
    CREATE_FUNC(OomeeLayer);
};

#endif // __HELLOWORLD_SCENE_H__
