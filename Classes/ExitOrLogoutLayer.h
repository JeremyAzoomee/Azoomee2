#ifndef __EXIT_OR_LOGOUT_LAYER_H__
#define __EXIT_OR_LOGOUT_LAYER_H__

#include "cocos2d.h"
#include "ElectricDreamsButton.h"
#include "AwaitingAdultPinLayer.h"

USING_NS_CC;

class ExitOrLogoutLayer : public Layer, public ElectricDreamsButtonDelegate, public AwaitingAdultPinLayerDelegate
{
private:
    Size visibleSize;
    Vec2 origin;
    
    LayerColor *backgroundLayer;
    Layer* windowLayer;
    
    ElectricDreamsButton *logoutButton;
    ElectricDreamsButton *cancelButton;
    
    void askForPin();
    void createAndFadeInLayer();
    void addListenerToBackgroundLayer();
    void addExitOrLogoutUIObjects();
    
    void removeSelf();
    
public:
    virtual bool init();
    
    CREATE_FUNC(ExitOrLogoutLayer);
    
    void setCenterPosition(Vec2 position);
    Vec2 getCenterPosition();
    
    //Delegate Functions
    void buttonPressed(ElectricDreamsButton* button);
    void AdultPinCancelled(AwaitingAdultPinLayer* layer);
    void AdultPinAccepted(AwaitingAdultPinLayer* layer);
};

#endif
