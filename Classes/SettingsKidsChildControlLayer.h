#ifndef __SETTINGS_KIDS_CHILD_CONTROL_LAYER_H__
#define __SETTINGS_KIDS_CHILD_CONTROL_LAYER_H__

#include <cocos/cocos2d.h>
#include <AzoomeeCommon/UI/ElectricDreamsButton.h>
#include "SettingsKidsLayer.h"
#include "ChildOomeeLayer.h"
#include <AzoomeeCommon/Input/TextInputLayer.h>

USING_NS_CC;
using namespace Azoomee;

class SettingsKidsChildControlLayer : public Layer, public ElectricDreamsButtonDelegate, public TextInputLayerDelegate
{
private:
    int childNumber;
    SettingsKidsLayer* parentLayer;
    
    ChildOomeeLayer* childFrameLayer;
    
    TextInputLayer *kidCodeTextInput;
    
    ElectricDreamsButton *getCodeButton;
    ElectricDreamsButton *addFriendButton;
    ElectricDreamsButton *closeButton;
    ElectricDreamsButton *shareButton;
    ElectricDreamsButton *sendCodeButton;
    ElectricDreamsButton *tryAgainButton;
    ElectricDreamsButton *addAnotherButton;
    ElectricDreamsButton *textInputButton;
    
    void clearAllButCloseButton();
    
    void addChildFrame();
    void addButtons();
    void createTextInput();
    
public:
    
    static Layer* createController(SettingsKidsLayer* setParent, int setChildNumber);
    virtual bool init();
    
    CREATE_FUNC(SettingsKidsChildControlLayer);
    
    //Delegate Functions
    void buttonPressed(ElectricDreamsButton* button);
    void textInputIsValid(TextInputLayer* inputLayer, bool isValid);
    void textInputReturnPressed(TextInputLayer* inputLayer);

};

#endif
