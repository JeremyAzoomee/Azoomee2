#ifndef Privacy_Layer_H
#define Privacy_Layer_H

#include "cocos2d.h"
#include "../Azoomee.h"
#include "ElectricDreamsButton.h"
#include "Style.h"

NS_AZOOMEE_BEGIN

class PrivacyLayer : public cocos2d::Layer, public Azoomee::ElectricDreamsButtonDelegate
{
private:
    
    Azoomee::ElectricDreamsButton *_privacyButton = nullptr;
    
    void createButton(cocos2d::Color3B newColor);
    void setSizeAndAddItems();

public:
    
    static PrivacyLayer* createWithColor(cocos2d::Color3B newColor = Style::Color::brightAqua);
    
    virtual bool init() override;
    
    CREATE_FUNC(PrivacyLayer);
    
    void setCenterPosition(cocos2d::Vec2 position);
    cocos2d::Vec2 getCenterPosition();
    
    //Delegate Functions
    void buttonPressed(Azoomee::ElectricDreamsButton* button) override;
};

NS_AZOOMEE_END

#endif
