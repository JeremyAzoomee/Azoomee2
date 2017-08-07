#ifndef __ONLINE_SAFETY_DETAILS_LAYER_H__
#define __ONLINE_SAFETY_DETAILS_LAYER_H__

#include <cocos/cocos2d.h>
#include <AzoomeeCommon/UI/ElectricDreamsButton.h>
#include <AzoomeeCommon/Azoomee.h>
#include "ui/UIWebView.h"

using namespace cocos2d;

NS_AZOOMEE_BEGIN

class OnlineSafetyDetailsLayer : public Layer, public ElectricDreamsButtonDelegate
{
private:
    Size visibleSize;
    Vec2 origin;
    
    float layerHeight;
    int currentSlideNumber;
    
    LayerColor* backgroundLayer;
    LayerColor* videoLayer;
    
    Label* titleLabel;
    Label* slideTitleLabel;
    Label* mainTextLabel;
    
    experimental::ui::WebView* videoWebview;
    
    ElectricDreamsButton *watchSearchItUpButton;
    ElectricDreamsButton *chevronLeftButton;
    ElectricDreamsButton *chevronRightButton;
    ElectricDreamsButton *closeVideoButton;
    
    Sprite* mainImage;
    
    void createBackground();
    void addUIObjects();
    void setToCurrentSlideNumber();
    void moveSlideNumberBy(int moveBy);
    
    void playVideo();
    
    void addListenerToVideoLayer(Layer* listenerToLayer);
    
public:
    static Layer* createWithHeight(float setLayerHeight);
    virtual bool init();
    
    CREATE_FUNC(OnlineSafetyDetailsLayer);
    
    //Delegate Functions
    void buttonPressed(ElectricDreamsButton* button);
};

NS_AZOOMEE_END

#endif
