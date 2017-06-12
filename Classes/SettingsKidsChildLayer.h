#ifndef __SETTINGS_KIDS_CHILD_LAYER_H__
#define __SETTINGS_KIDS_CHILD_LAYER_H__

#include <cocos/cocos2d.h>
#include <AzoomeeCommon/UI/ElectricDreamsButton.h>

USING_NS_CC;
using namespace Azoomee;

class SettingsKidsChildLayer : public Layer, public ElectricDreamsButtonDelegate
{
private:
    std::string childName;
    int oomeeNo;
    
    Layer* childFrameLayer;
    
    ElectricDreamsButton *inviteButton;
    ElectricDreamsButton *acceptButton;

    void addChildFrame();
    void addButtons();
    
public:
    
    static Layer* createWithChildDetails(std::string setChildName, int setOomeeNo);
    virtual bool init();
    
    CREATE_FUNC(SettingsKidsChildLayer);
    
    //Delegate Functions
    void buttonPressed(ElectricDreamsButton* button);

};

#endif
