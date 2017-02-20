#ifndef __HQSCENEELEMENT_SCENE_H__
#define __HQSCENEELEMENT_SCENE_H__

#include "cocos2d.h"
#include "network/HttpClient.h"

class HQSceneElement : public cocos2d::Layer
{

public:
    CREATE_FUNC(HQSceneElement);
    static cocos2d::Scene* createScene();
    virtual bool init();
    void addHQSceneElement(std::string category, std::map<std::string, std::string>itemData, cocos2d::Vec2 shape, float delay);
    
private:
    void reduceLabelTextToFitWidth(cocos2d::Label* label,float maxWidth);
    void resizeSceneElement(cocos2d::Vec2 shape, std::string category);
    void createColourLayer(std::string category, float delay);
    void addImageToBaseLayer(std::string filename);
    void addGradientToBottom(std::string category);
    cocos2d::Sprite* addIconToImage(std::string category);
    void addLabelsToImage(std::map<std::string, std::string>itemData, cocos2d::Sprite* nextToIcon);
    cocos2d::Size getSizeOfLayerWithGap();
    
    std::map<std::string, int> category_translator;
    
    cocos2d::LayerColor *baseLayer;
    cocos2d::LayerColor *overlayWhenTouched;
    
    void addTouchOverlayToElement();
    void addListenerToElement(std::string uri, std::string contentId, std::string category, std::string title, std::string description, std::string type, bool preview);
    void startUpElementDependingOnType(std::string uri, std::string contentId, std::string category);
    void addLockToElement();
    
    cocos2d::Point touchPoint;
    bool movedAway;
    bool iamtouched;
    
    void onExitTransitionDidStart();
    
    bool aboutToExit;
};

class HQSceneElementHQListener : public cocos2d::Ref
{
    
};

class HQSceneelementOfflineHQListener : public cocos2d::Ref
{
    
};

#endif
