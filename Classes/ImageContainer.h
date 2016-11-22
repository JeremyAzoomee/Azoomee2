#ifndef __IMAGECONTAINER_SCENE_H__
#define __IMAGECONTAINER_SCENE_H__

#include "cocos2d.h"

class ImageContainer : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    void createContainer(std::string imageName, cocos2d::Color4B bgColour, float scale, float startDelay, cocos2d::Point position);
    void addListenerToContainer(cocos2d::Node *addTo, int maxOpacity);
    
    // implement the "static create()" method manually
    CREATE_FUNC(ImageContainer);
};

#endif // __MAINHUBBGELEMENTS__
