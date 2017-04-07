#ifndef __AWAITING_ADULT_PIN_LAYER_H__
#define __AWAITING_ADULT_PIN_LAYER_H__

#include "cocos2d.h"
#include "TextInputLayer.h"
#include "ElectricDreamsButton.h"
#include "MessageBox.h"

USING_NS_CC;

class AwaitingAdultPinLayer;

class AwaitingAdultPinLayerDelegate
{
public:
    virtual void AdultPinCancelled(AwaitingAdultPinLayer* layer) = 0;
    virtual void AdultPinAccepted(AwaitingAdultPinLayer* layer) = 0;
};

class AwaitingAdultPinLayer : public Layer, public TextInputLayerDelegate, public ElectricDreamsButtonDelegate, public MessageBoxDelegate
{
private:

    Size visibleSize;
    Vec2 origin;
    
    LayerColor *backgroundLayer;
    
    Layer* windowLayer;
    
    TextInputLayer* editBox_pin;
    
    ElectricDreamsButton *acceptButton;
    ElectricDreamsButton *cancelButton;
    
    void createAndFadeInLayer();
    void addListenerToBackgroundLayer();
    void addUIObjects();
    
    void removeSelf(float dt);
    
public:
    virtual bool init();
    
    CREATE_FUNC(AwaitingAdultPinLayer);
    
    CC_SYNTHESIZE(AwaitingAdultPinLayerDelegate*, _delegate, Delegate);
    
    //Delegate Functions
    void textInputIsValid(TextInputLayer* inputLayer, bool isValid);
    void buttonPressed(ElectricDreamsButton* button);
    void MessageBoxButtonPressed(std::string messageBoxTitle,std::string buttonTitle);
    
    void secondCheckForPin();
    
};

#endif
