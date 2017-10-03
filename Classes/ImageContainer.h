#ifndef __IMAGECONTAINER_SCENE_H__
#define __IMAGECONTAINER_SCENE_H__

#include <cocos/cocos2d.h>
#include <AzoomeeCommon/Azoomee.h>
#include <AzoomeeCommon/Data/HQDataObject/HQContentItemObject.h>

NS_AZOOMEE_BEGIN

class ImageContainer : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(ImageContainer);
    void createContainer(HQContentItemObject* elementProperties, float scale, float startDelay, cocos2d::Point position);

private:

    void addListenerToContainer(cocos2d::Node *addTo, int maxOpacity, HQContentItemObject* elementProperties, bool IAPEnabled);
    
    cocos2d::LayerColor *bgLayer;
    void createBgLayer(HQContentItemObject* elementProperties, float scale, float startDelay, cocos2d::Point position);
    
    void addImageToLayer(std::string url,std::string type, float startDelay);
    void addGradientToBottom(cocos2d::Color3B colour, float startDelay);
    void addIconToImage(std::string type, float startDelay);
    void addLabelToImage(std::string name, float startDelay);
    void addLockToImageContainer(const std::string& elementType, float startDelay);
    void addReponseLayerToImage(HQContentItemObject* elementProperties, float scale);
    
    void startAudio(std::string audioName);
    
    bool iamtouched = false;
    
    float appearPause;
    float interTime;
    float scaleTime;
    float scaleFactor;
    
    cocos2d::Sprite* icon;
};

NS_AZOOMEE_END

#endif
