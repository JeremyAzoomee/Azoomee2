#ifndef __EXIT_OR_LOGOUT_LAYER_H__
#define __EXIT_OR_LOGOUT_LAYER_H__

#include "cocos2d.h"
#include "ElectricDreamsButton.h"

USING_NS_CC;

class ExitOrLogoutLayer : public Layer, public ElectricDreamsButtonDelegate
{
private:
    Size visibleSize;
    Vec2 origin;
    
    LayerColor *backgroundLayer;
    
    ElectricDreamsButton *exitButton;
    ElectricDreamsButton *logoutButton;
    ElectricDreamsButton *cancelButton;
    
    void createAndFadeInLayer();
    void addListenerToBackgroundLayer();
    void addUIObjects();
    
    void removeSelf();
    
public:
    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(ExitOrLogoutLayer);
    
    void setCenterPosition(Vec2 position);
    Vec2 getCenterPosition();
    
    //Delegate Functions
    void buttonPressed(ElectricDreamsButton* button);
    
};

#endif
