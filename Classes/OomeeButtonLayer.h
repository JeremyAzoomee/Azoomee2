#ifndef __OOMEE_BUTTON_LAYER_H__
#define __OOMEE_BUTTON_LAYER_H__

#include "cocos2d.h"
#include <spine/spine-cocos2dx.h>

class OomeeButtonLayer : public cocos2d::Layer
{
public:
    CREATE_FUNC(OomeeButtonLayer);
    static OomeeButtonLayer* createOomeeLayer(int oomeeNumber);
    virtual bool init();
    
    void animationBeforeButtonPress();
    void hideOomee();
    
private:
    void addOomeeToLayer();
    void addCompleteListenerToOomee(spine::SkeletonAnimation* toBeAddedTo);
    
    int displayedOomeeNumber;
    int animationsTillWave;
    
    spine::SkeletonAnimation* oomee;
};

#endif
