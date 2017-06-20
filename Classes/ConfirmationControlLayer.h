#ifndef __CONFIRMATION_CONTROL_LAYER_H__
#define __CONFIRMATION_CONTROL_LAYER_H__

#include <cocos/cocos2d.h>
#include <AzoomeeCommon/UI/ElectricDreamsButton.h>
#include "ConfirmationLayer.h"

USING_NS_CC;
using namespace Azoomee;

class ConfirmationControlLayer : public Layer, public ElectricDreamsButtonDelegate
{
private:
    ConfirmationLayer* confirmationFrameLayer;
    int confirmationNumber;
    
    ElectricDreamsButton *confirmButton;
    ElectricDreamsButton *rejectButton;
    ElectricDreamsButton *yesButton;
    ElectricDreamsButton *noButton;
    
    void clearAllButtons();
    
    void addConfirmationFrame();
    void addButtons();
    
public:
    static Layer* createController(Size layerSize, int setConfirmationNumber);
    virtual bool init();
    
    CREATE_FUNC(ConfirmationControlLayer);
    
    //Delegate Functions
    void buttonPressed(ElectricDreamsButton* button);

};

#endif
