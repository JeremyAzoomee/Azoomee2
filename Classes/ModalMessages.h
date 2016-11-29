#ifndef __MODALMESSAGES_SCENE_H__
#define __MODALMESSAGES_SCENE_H__

#include "cocos2d.h"

class ModalMessages: public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(ModalMessages);
    
    void startLoading();
    void stopLoading();
    void addListenerToBackgroundLayer();
    
    cocos2d::LayerColor *loadingLayer;
    
    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;
};

#endif // __MODALMESSAGES_SCENE_H__
