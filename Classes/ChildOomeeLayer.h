#ifndef __CHILD_OOMEE_LAYER_H__
#define __CHILD_OOMEE_LAYER_H__

#include <cocos/cocos2d.h>


USING_NS_CC;

class ChildOomeeLayer : public Layer
{
private:
    Size visibleSize;
    Vec2 origin;

    void addExitOrLogoutUIObjects();

    
public:
    virtual bool init();
    
    CREATE_FUNC(ChildOomeeLayer);

};

#endif
