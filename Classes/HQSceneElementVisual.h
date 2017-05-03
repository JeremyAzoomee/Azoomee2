#ifndef __HQSCENEELEMENTVISUAL_H__
#define __HQSCENEELEMENTVISUAL_H__

#include <cocos/cocos2d.h>
#include "network/HttpClient.h"

class HQSceneElementVisual : public cocos2d::Layer
{

public:
    CREATE_FUNC(HQSceneElementVisual);
    virtual bool init();
    cocos2d::Layer* addHQSceneElement(std::string category, std::map<std::string, std::string>itemData, cocos2d::Vec2 shape, float delay, bool createForOffline);
    
    cocos2d::LayerColor *baseLayer;
    cocos2d::LayerColor *overlayWhenTouched;
    
private:
    
    //-----SETUP-----
    void resizeSceneElement();
    void createBaseLayer();
    void setShouldDisplayVisualElementsOverImage();
    void createCallbackFunction(float delay);
    
    //-----VISUALS----
    void addImageDownloader();
    void addGradientToBottom();
    cocos2d::Sprite* addIconToImage();
    void addLabelsToImage(cocos2d::Sprite* nextToIcon);
    void addNewBadgeToImage();
    void addLockToElement();
    void addTouchOverlayToElement();
    
    //----OTHER-------
    void reduceLabelTextToFitWidth(cocos2d::Label* label,float maxWidth);
    void onExitTransitionDidStart();
    void onExit();
    
    bool aboutToExit;
    bool isOffline;
    bool shouldDisplayVisualElementsOverImage;
    
    std::string elementUrl;
    std::string elementCategory;
    cocos2d::Vec2 elementShape;
    
    std::map<std::string, std::string> elementItemDataMap;
};

#endif
