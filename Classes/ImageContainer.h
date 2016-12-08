#ifndef __IMAGECONTAINER_SCENE_H__
#define __IMAGECONTAINER_SCENE_H__

#include "cocos2d.h"

class ImageContainer : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    void createContainer(std::map<std::string, std::string> elementProperties, float scale, float startDelay, cocos2d::Point position);
    cocos2d::Color4B getColourByType(std::string type);
    void addListenerToContainer(cocos2d::Node *addTo, int maxOpacity, std::string uri);
    
    
    void addGradientToBottom(cocos2d::Color3B colour, float startDelay);
    void addIconToImage(std::string type, float startDelay);
    void addLabelToImage(std::string name, float startDelay);
    void addLockToImageContainer(float startDelay);
    
    cocos2d::LayerColor *bgLayer;
    
    // implement the "static create()" method manually
    CREATE_FUNC(ImageContainer);
};

#endif // __MAINHUBBGELEMENTS__
