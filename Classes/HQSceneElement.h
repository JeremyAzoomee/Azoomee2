#ifndef __HQSCENEELEMENT_SCENE_H__
#define __HQSCENEELEMENT_SCENE_H__

#include "cocos2d.h"

class HQSceneElement : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    void createColourLayer(int highlight);
    void addImageToBaseLayer(std::string filename);
    void addGradientToBottom();
    void addIconToImage();
    void addLabelToImage(std::string name);
    cocos2d::Size getSizeOfLayerWithGap();
    
    std::map<std::string, int> category_translator;
    
    void addHQSceneElement(std::string category, std::map<std::string, std::string>itemData);
    
    cocos2d::LayerColor *baseLayer;
    cocos2d::LayerColor *overlayWhenTouched;
    
    void addTouchOverlayToElement();
    void addListenerToElement(std::string uri);
    void addLockToElement();
    
    cocos2d::Point touchPoint;
    bool movedAway;
    
    CREATE_FUNC(HQSceneElement);
};

#endif
