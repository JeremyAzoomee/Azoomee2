#ifndef __HQSCENEELEMENTVISUAL_H__
#define __HQSCENEELEMENTVISUAL_H__

#include <cocos/cocos2d.h>
#include "network/HttpClient.h"
#include <AzoomeeCommon/Azoomee.h>
#include <AzoomeeCommon/Data/HQDataObject/HQContentItemObject.h>

NS_AZOOMEE_BEGIN

class HQSceneElementVisual : public cocos2d::Layer
{

public:
    CREATE_FUNC(HQSceneElementVisual);
    virtual bool init();
    
    void setCategory(std::string category);
    void setItemData(const HQContentItemObjectRef &itemData);
    void setShape(cocos2d::Vec2 shape);
    void setDelay(float delay);
    void setCreatedForOffline(bool createdForOffline);
    void setManualSizeMultiplier(float multiplier);
    void setMargin(float margin);
    void setThumbUrl(const std::string &url);
    
    cocos2d::Layer* createHQSceneElement();
    
    cocos2d::LayerColor *_baseLayer = nullptr;
    cocos2d::LayerColor *_overlayWhenTouched = nullptr;
    
private:
    bool _shouldDisplayVisualElementsOverImage = false;
    std::string _elementCategory;
    HQContentItemObjectRef _elementItemData;
    cocos2d::Vec2 _elementShape;
    float _elementDelay;
    bool _elementCreatedForOffline = false;
    bool _elementCreatedForMainHub = false;
    std::string _elementUrl;
    float _manualSizeMultiplier = 0.0f;
    float _margin = 20.0f;
    
    cocos2d::Sprite* _downloadedImage;
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
    void addGroupLabelsToImage();
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
