#ifndef __MANUAL_GAME_INPUT_LAYER_H__
#define __MANUAL_GAME_INPUT_LAYER_H__

#include <cocos/cocos2d.h>
#include <AzoomeeCommon/Azoomee.h>
#include <AzoomeeCommon/UI/ElectricDreamsButton.h>
#include <AzoomeeCommon/Input/TextInputLayer.h>

NS_AZOOMEE_BEGIN

class ManualGameInputLayer : public cocos2d::Layer, public ElectricDreamsButtonDelegate
{
private:
    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;
    
    cocos2d::LayerColor *backgroundLayer;
    
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

NS_AZOOMEE_END

#endif
