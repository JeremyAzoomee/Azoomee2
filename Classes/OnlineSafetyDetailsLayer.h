#ifndef __ONLINE_SAFETY_DETAILS_LAYER_H__
#define __ONLINE_SAFETY_DETAILS_LAYER_H__

#include <cocos/cocos2d.h>
#include <AzoomeeCommon/UI/ElectricDreamsButton.h>
#include <AzoomeeCommon/Azoomee.h>

using namespace cocos2d;

NS_AZOOMEE_BEGIN

class OnlineSafetyDetailsLayer : public Layer, public ElectricDreamsButtonDelegate
{
private:
    Size visibleSize;
    Vec2 origin;
    
    float layerHeight;
    
    LayerColor* backgroundLayer;
    
    Label* titleLabel;
    Label* slideTitleLabel;
    Label* mainTextLabel;
    
    ElectricDreamsButton *watchSearchItUpButton;
    ElectricDreamsButton *chevronLeftButton;
    ElectricDreamsButton *chevronRightButton;
    
    Sprite* mainImage;
    Sprite* leftButton;
    Sprite* rightButton;
    
    void createBackground();
    void addUIObjects();
    
public:
    static Layer* createWithHeight(float setLayerHeight);
    virtual bool init();
    
    CREATE_FUNC(OnlineSafetyDetailsLayer);
    
    //Delegate Functions
    void buttonPressed(ElectricDreamsButton* button);
};

NS_AZOOMEE_END

#endif
