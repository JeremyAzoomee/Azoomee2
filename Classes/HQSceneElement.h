#ifndef __HQSCENEELEMENT_SCENE_H__
#define __HQSCENEELEMENT_SCENE_H__

#include "cocos2d.h"

class HQSceneElement : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    void fillUpColoursAndImagesArray();
    void createColourLayer(int category, int highlight);
    void addImageToBaseLayer(std::string filename);
    void addGradientToBottom(int category);
    void addIconToImage(int category);
    void addLabelToImage(std::string name);
    cocos2d::Size getSizeOfLayerWithGap();
    
    std::vector<cocos2d::Color4B> baseColours;
    std::vector<std::string> iconImages;
    std::vector<cocos2d::Size> baseSizes;
    std::vector<cocos2d::Vec2> highlightSizeMultipliers;
    
    void addHQSceneElement(int category, int highlight, std::string filename, std::string name);
    
    cocos2d::LayerColor *baseLayer;
    
    // implement the "static create()" method manually
    CREATE_FUNC(HQSceneElement);
};

#endif // __HQSCENE_SCENE_ELEMENT_H__
