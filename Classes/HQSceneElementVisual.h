#ifndef __HQSCENEELEMENTVISUAL_H__
#define __HQSCENEELEMENTVISUAL_H__

#include <cocos/cocos2d.h>
#include "network/HttpClient.h"
#include <AzoomeeCommon/Azoomee.h>
#include "HQContentItemObject.h"

NS_AZOOMEE_BEGIN

class HQSceneElementVisual : public cocos2d::Layer
{

public:
    CREATE_FUNC(HQSceneElementVisual);
    virtual bool init();
    
    void setCategory(std::string category);
    void setItemData(HQContentItemObject* itemData);
    void setShape(cocos2d::Vec2 shape);
    void setDelay(float delay);
    void setCreatedForOffline(bool createdForOffline);
    
    cocos2d::Layer* createHQSceneElement();
    
    cocos2d::LayerColor *baseLayer;
    cocos2d::LayerColor *overlayWhenTouched;
    
private:
    bool shouldDisplayVisualElementsOverImage = false;
    std::string elementCategory;
    HQContentItemObject* elementItemData;
    cocos2d::Vec2 elementShape;
    float elementDelay;
    bool elementCreatedForOffline = false;
    std::string elementUrl;
    
    cocos2d::Sprite* downloadedImage;
    void createColourLayer();
    
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
    void addLockToElement();
    void addTouchOverlayToElement();
    
    //----OTHER-------
    void reduceLabelTextToFitWidth(cocos2d::Label* label,float maxWidth);
    void onExitTransitionDidStart();
    void onExit();
    
    bool aboutToExit = false;
    bool isOffline = false;
};

NS_AZOOMEE_END

#endif
