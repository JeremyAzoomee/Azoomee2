#ifndef Privacy_Layer_H
#define Privacy_Layer_H

#include "cocos2d.h"
#include "../Azoomee.h"
#include "ElectricDreamsButton.h"

NS_AZOOMEE_BEGIN

class PrivacyLayer : public cocos2d::Layer, public Azoomee::ElectricDreamsButtonDelegate
{
private:
    
    Azoomee::ElectricDreamsButton *_privacyButton = nullptr;
    
    void createButtons();
    void setSizeAndAddItems();

public:
    virtual bool init();
    
    CREATE_FUNC(PrivacyLayer);
    
    void setCenterPosition(cocos2d::Vec2 position);
    cocos2d::Vec2 getCenterPosition();
    
    //Delegate Functions
    void buttonPressed(Azoomee::ElectricDreamsButton* button);
};

NS_AZOOMEE_END

#endif
