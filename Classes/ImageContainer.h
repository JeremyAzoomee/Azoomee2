#ifndef __IMAGECONTAINER_SCENE_H__
#define __IMAGECONTAINER_SCENE_H__

#include "cocos2d.h"

class ImageContainer : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(ImageContainer);
    void createContainer(std::map<std::string, std::string> elementProperties, float scale, float startDelay, cocos2d::Point position);

private:

    void addListenerToContainer(cocos2d::Node *addTo, int maxOpacity, std::string uri, std::string itemId);
    void addPreviewListenerToContainer(cocos2d::Node *addTo);
    
    cocos2d::LayerColor *bgLayer;
    void createBgLayer(std::map<std::string, std::string> elementProperties, float scale, float startDelay, cocos2d::Point position);
    
    void addImageToLayer(std::string url, float startDelay);
    void addGradientToBottom(cocos2d::Color3B colour, float startDelay);
    void addIconToImage(std::string type, float startDelay);
    void addLabelToImage(std::string name, float startDelay);
    void addLockToImageContainer(float startDelay);
    void addReponseLayerToImage(std::map<std::string, std::string> elementProperties, float scale);
    
    void startAudio(std::string audioName);
    
    bool iamtouched;
    
    float appearPause;
    float interTime;
    float scaleTime;
};

#endif
