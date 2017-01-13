#ifndef __HQSCENEELEMENT_SCENE_H__
#define __HQSCENEELEMENT_SCENE_H__

#include "cocos2d.h"

class HQSceneElement : public cocos2d::Layer
{
public:
    CREATE_FUNC(HQSceneElement);
    static cocos2d::Scene* createScene();
    virtual bool init();
    void addHQSceneElement(std::string category, std::map<std::string, std::string>itemData, cocos2d::Vec2 shape);
    
private:
    void resizeSceneElement(cocos2d::Vec2 shape, std::string category);
    void createColourLayer(std::string category);
    void addImageToBaseLayer(std::string filename);
    void addGradientToBottom(std::string category);
    void addIconToImage(std::string category);
    void addLabelToImage(std::string name);
    cocos2d::Size getSizeOfLayerWithGap();
    
    std::map<std::string, int> category_translator;
    
    cocos2d::LayerColor *baseLayer;
    cocos2d::LayerColor *overlayWhenTouched;
    
    void addTouchOverlayToElement();
    void addListenerToElement(std::string uri);
    void addLockToElement();
    
    cocos2d::Point touchPoint;
    bool movedAway;
};

#endif
