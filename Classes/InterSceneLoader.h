#ifndef __INTERSCENELOADER_SCENE_H__
#define __INTERSCENELOADER_SCENE_H__

#include "cocos2d.h"

class InterSceneLoader : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    
    cocos2d::LayerColor* addColourLayerToScreen(cocos2d::Color4B colour, cocos2d::Size size, cocos2d::Point position, std::string imageName);
    cocos2d::LayerColor* part1;
    cocos2d::LayerColor* part2;
    
    void startLoading();
    void stopLoading(float dt);
    void layersStopAllActions();
    
    cocos2d::Size visibleSize;
    cocos2d::Point origin;
    
    // implement the "static create()" method manually
    CREATE_FUNC(InterSceneLoader);
};

#endif // __INTERSCENELOADER__
