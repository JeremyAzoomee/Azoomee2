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
    
    void setElementProperties(const HQContentItemObjectRef &elementProperties);
    void setScale(float scale);
    void setStartDelay(float startDelay);
    void setThumbUrl(const std::string &url);
    
    void createContainer();

private:
    HQContentItemObjectRef _elementProperties;
    float _scale = 0;
    float _startDelay = 0;
    std::string _thumbUrl = "";
    
    
    void addListenerToContainer(cocos2d::Node *addTo, int maxOpacity, const HQContentItemObjectRef &elementProperties, bool IAPEnabled);
    
    cocos2d::LayerColor *bgLayer;
    void createBgLayer(const HQContentItemObjectRef &elementProperties, float scale, float startDelay);
    
    void addImageToLayer(std::string url,std::string type, float startDelay);
    void addGradientToBottom(cocos2d::Color3B colour, float startDelay);
    void addIconToImage(std::string type, float startDelay);
    void addLabelToImage(std::string name, float startDelay);
    void addLockToImageContainer(const std::string& elementType, float startDelay);
    void addReponseLayerToImage(const HQContentItemObjectRef &elementProperties, float scale);
    
    void startAudio(std::string audioName);
    
    bool iamtouched = false;
    
    float appearPause = 0.0f;
    float interTime = 0.0f;
    float scaleTime = 0.0f;
    float scaleFactor;
    
    cocos2d::Sprite* icon;
};

NS_AZOOMEE_END

#endif
