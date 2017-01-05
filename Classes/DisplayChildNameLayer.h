#ifndef __DISPLAY_CHILD_NAME_LAYER_H__
#define __DISPLAY_CHILD_NAME_LAYER_H__

#include "cocos2d.h"

class DisplayChildNameLayer : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    
    cocos2d::Label* addChildNameToLayer();
    void addFrameToLayer(cocos2d::Label* childNameLayer);
    std::string getLoggedInChildName();
    
    CREATE_FUNC(DisplayChildNameLayer);
};

#endif
