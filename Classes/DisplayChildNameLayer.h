#ifndef __DISPLAY_CHILD_NAME_LAYER_H__
#define __DISPLAY_CHILD_NAME_LAYER_H__

#include "cocos2d.h"

class DisplayChildNameLayer : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    
    void addChildNameToLayer();
    void addFrameToLayer();
    void setMaxScaleForLabel(cocos2d::Label* childNameLabel);
    std::string shortenString(std::string text, int numberOfCharacters);
    std::string getLoggedInChildName();
    
    CREATE_FUNC(DisplayChildNameLayer);
};

#endif
