#ifndef __AWAITING_ADULT_PIN_LAYER_H__
#define __AWAITING_ADULT_PIN_LAYER_H__

#include "cocos2d.h"
#include "TextInputLayer.h"
#include "ElectricDreamsButton.h"

USING_NS_CC;

class AwaitingAdultPinLayer;

class AwaitingAdultPinLayerDelegate
{
public:
    virtual void PinCancelled(AwaitingAdultPinLayer* layer) = 0;
    virtual void AdultPinAccepted(AwaitingAdultPinLayer* layer) = 0;
};

class AwaitingAdultPinLayer : public Layer, public TextInputLayerDelegate, public ElectricDreamsButtonDelegate
{
private:

    Size visibleSize;
    Vec2 origin;
    
    LayerColor *backgroundLayer;
    
    TextInputLayer* editBox_pin;
    
    ElectricDreamsButton *acceptButton;
    ElectricDreamsButton *cancelButton;
    
    void createAndFadeInLayer();
    void addListenerToBackgroundLayer();
    void addUIObjects();
    
    void removeSelf(float dt);
    
public:
    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(AwaitingAdultPinLayer);
    
    CC_SYNTHESIZE(AwaitingAdultPinLayerDelegate*, _delegate, Delegate);
    
    void setCenterPosition(Vec2 position);
    Vec2 getCenterPosition();
    
    //Delegate Functions
    void textInputIsValid(TextInputLayer* inputLayer, bool isValid);
    void buttonPressed(ElectricDreamsButton* button);
    
};

#endif
