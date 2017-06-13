#ifndef __SETTINGS_KIDS_CHILD_CONTROL_LAYER_H__
#define __SETTINGS_KIDS_CHILD_CONTROL_LAYER_H__

#include <cocos/cocos2d.h>
#include <AzoomeeCommon/UI/ElectricDreamsButton.h>
#include "SettingsKidsLayer.h"

USING_NS_CC;
using namespace Azoomee;

class SettingsKidsChildControlLayer : public Layer, public ElectricDreamsButtonDelegate
{
private:
    int childNumber;
    SettingsKidsLayer* parentLayer;
    
    Layer* childFrameLayer;
    
    ElectricDreamsButton *inviteButton;
    ElectricDreamsButton *acceptButton;
    ElectricDreamsButton *closeButton;

    void addChildFrame();
    void addButtons();
    
public:
    
    static Layer* createController(SettingsKidsLayer* setParent, int setChildNumber);
    virtual bool init();
    
    CREATE_FUNC(SettingsKidsChildControlLayer);
    
    //Delegate Functions
    void buttonPressed(ElectricDreamsButton* button);

};

#endif
