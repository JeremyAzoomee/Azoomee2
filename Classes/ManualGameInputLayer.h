#ifndef __MANUAL_GAME_INPUT_LAYER_H__
#define __MANUAL_GAME_INPUT_LAYER_H__

#include <cocos/cocos2d.h>
#include "ElectricDreamsButton.h"
#include <AzoomeeCommon/Input/TextInputLayer.h>


USING_NS_CC;

class ManualGameInputLayer : public Layer, public ElectricDreamsButtonDelegate
{
private:
    Size visibleSize;
    Vec2 origin;
    
    LayerColor *backgroundLayer;
    
    ElectricDreamsButton* backButton;
    ElectricDreamsButton* startGameButton;
    
    TextInputLayer *uriTextInput;
    
    void createBackgroundLayer();
    void addListenerToBackgroundLayer();
    
    void createUpSaleLayer();
    
    void addTitle();
    void addButtons();
    void addTextBox();

    void removeSelf();
    
public:
    
    virtual bool init();
    
    CREATE_FUNC(ManualGameInputLayer);

    //Delegate Functions
    void buttonPressed(ElectricDreamsButton* button);
};

#endif
