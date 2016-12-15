#ifndef __IMAGECONTAINER_SCENE_H__
#define __IMAGECONTAINER_SCENE_H__

#include "cocos2d.h"

class ImageContainer : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    void createContainer(std::map<std::string, std::string> elementProperties, float scale, float startDelay, cocos2d::Point position);
    void addListenerToContainer(cocos2d::Node *addTo, int maxOpacity, std::string uri, std::string itemId);
    
    cocos2d::LayerColor *bgLayer;
    void createBgLayer(std::map<std::string, std::string> elementProperties, float scale, float startDelay, cocos2d::Point position);
    
    void addImageToLayer(std::string url);
    void addGradientToBottom(cocos2d::Color3B colour, float startDelay);
    void addIconToImage(std::string type, float startDelay);
    void addLabelToImage(std::string name, float startDelay);
    void addLockToImageContainer(float startDelay);
    void addReponseLayerToImage(std::map<std::string, std::string> elementProperties, float scale);

    CREATE_FUNC(ImageContainer);
};

#endif
