#ifndef __SETTINGS_CONTROL_LAYER_H__
#define __SETTINGS_CONTROL_LAYER_H__

#include <cocos/cocos2d.h>
#include <AzoomeeCommon/UI/ElectricDreamsButton.h>
#include "AwaitingAdultPinLayer.h"

USING_NS_CC;

class SettingsControlLayer : public Layer, public ElectricDreamsButtonDelegate, public AwaitingAdultPinLayerDelegate
{
private:
    Size visibleSize;
    Vec2 origin;
    
    Layer* currentLayer;
    
    LayerColor *backgroundLayer;
    float linePositionY;
    
    ElectricDreamsButton *cancelButton;
    ElectricDreamsButton *accountButton;
    ElectricDreamsButton *confirmationButton;
    ElectricDreamsButton *childrenButton;
    
    void createSettingsLayer();
    void addListenerToLayer(Layer* layerToAdd);
    
    void createSettingsController();
    void createCancelButton();
    void createLine();
    void createTabs();
    
    void removeSelf();
    
    //Button Actions
    void selectNewTab(Layer* newCurrentLayer, ElectricDreamsButton* buttonToBringForward);
    
public:
    virtual bool init();
    
    CREATE_FUNC(SettingsControlLayer);
    
    Layer* getCurrentLayer();
    
    //Delegate Functions
    void buttonPressed(ElectricDreamsButton* button);
    void AdultPinCancelled(AwaitingAdultPinLayer* layer);
    void AdultPinAccepted(AwaitingAdultPinLayer* layer);
};

#endif
