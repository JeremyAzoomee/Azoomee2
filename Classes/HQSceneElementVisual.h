#ifndef __HQSCENEELEMENTVISUAL_H__
#define __HQSCENEELEMENTVISUAL_H__

#include "cocos2d.h"
#include "network/HttpClient.h"

class HQSceneElementVisual : public cocos2d::Layer
{

public:
    CREATE_FUNC(HQSceneElementVisual);
    static cocos2d::Scene* createScene();
    virtual bool init();
    cocos2d::Layer* addHQSceneElement(std::string category, std::map<std::string, std::string>itemData, cocos2d::Vec2 shape, float delay);
    
    cocos2d::LayerColor *baseLayer;
    cocos2d::LayerColor *overlayWhenTouched;
    
private:
    void reduceLabelTextToFitWidth(cocos2d::Label* label,float maxWidth);
    void resizeSceneElement(cocos2d::Vec2 shape, std::string category);
    void createColourLayer(std::string category, float delay);
    void addImageToBaseLayer(std::string filename, std::string type, cocos2d::Vec2 shape);
    void addGradientToBottom(std::string category);
    cocos2d::Sprite* addIconToImage(std::string category);
    void addLabelsToImage(std::map<std::string, std::string>itemData, cocos2d::Sprite* nextToIcon);
    
    cocos2d::Size getSizeOfLayerWithGap();
    
    std::map<std::string, int> category_translator;
    
    void addTouchOverlayToElement();
    void addLockToElement();
    
    void onExitTransitionDidStart();
    
    bool aboutToExit;
};

#endif
