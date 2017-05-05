#ifndef __HQSCENEELEMENTVISUAL_H__
#define __HQSCENEELEMENTVISUAL_H__

#include <cocos/cocos2d.h>
#include "network/HttpClient.h"

class HQSceneElementVisual : public cocos2d::Layer
{

public:
    CREATE_FUNC(HQSceneElementVisual);
    static cocos2d::Scene* createScene();
    virtual bool init();
    
    void setCategory(std::string category);
    void setItemData(std::map<std::string, std::string> itemData);
    void setShape(cocos2d::Vec2 shape);
    void setDelay(float delay);
    void setCreatedForOffline(bool createdForOffline);
    
    cocos2d::Layer* createHQSceneElement();
    
    cocos2d::LayerColor *baseLayer;
    cocos2d::LayerColor *overlayWhenTouched;
    
private:
    std::string elementCategory;
    std::map<std::string, std::string> elementItemData;
    cocos2d::Vec2 elementShape;
    float elementDelay;
    bool elementCreatedForOffline;
    std::string elementUrl;
    
    cocos2d::Sprite* downloadedImage;
    void reduceLabelTextToFitWidth(cocos2d::Label* label,float maxWidth);
    void resizeSceneElement();
    void createColourLayer();
    void addImageDownloader();
    void addGradientToBottom();
    cocos2d::Sprite* addIconToImage();
    void addLabelsToImage(cocos2d::Sprite* nextToIcon);
    
    cocos2d::Size getSizeOfLayerWithGap();
    
    std::map<std::string, int> category_translator;
    
    void addTouchOverlayToElement();
    void addLockToElement();
    
    void onExitTransitionDidStart();
    void onExit();
    
    bool aboutToExit;
    bool isOffline;
};

#endif
