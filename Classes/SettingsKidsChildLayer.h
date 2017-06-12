#ifndef __SETTINGS_KIDS_CHILD_LAYER_H__
#define __SETTINGS_KIDS_CHILD_LAYER_H__

#include <cocos/cocos2d.h>
#include <AzoomeeCommon/UI/ElectricDreamsButton.h>
#include "SettingsKidsLayer.h"

USING_NS_CC;
using namespace Azoomee;

class SettingsKidsChildLayer : public Layer, public ElectricDreamsButtonDelegate
{
private:
    std::string childName;
    int oomeeNo;
    SettingsKidsLayer* parentLayer;
    
    Layer* childFrameLayer;
    
    ElectricDreamsButton *inviteButton;
    ElectricDreamsButton *acceptButton;
    ElectricDreamsButton *closeButton;

    void addChildFrame();
    void addButtons();
    
public:
    
    static Layer* createWithChildDetails(std::string setChildName, int setOomeeNo, SettingsKidsLayer* setParent);
    virtual bool init();
    
    CREATE_FUNC(SettingsKidsChildLayer);
    
    //Delegate Functions
    void buttonPressed(ElectricDreamsButton* button);

};

#endif
