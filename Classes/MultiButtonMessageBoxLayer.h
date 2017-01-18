#ifndef __MULTI_BUTTON_MESSAGEBOX_LAYER_H__
#define __MULTI_BUTTON_MESSAGEBOX_LAYER_H__

#include "cocos2d.h"
#include "ElectricDreamsButton.h"

USING_NS_CC;

class MultiButtonMessageBoxLayer;

class MultiButtonMessageBoxLayerDelegate
{
public:
    virtual void MultiButtonMessageBoxPressed(int ButtonNumber) = 0;
};

class MultiButtonMessageBoxLayer : public Layer, public ElectricDreamsButtonDelegate
{
private:

    Size visibleSize;
    Vec2 origin;
    
    LayerColor *backgroundLayer;
    
    ElectricDreamsButton *acceptButton;
    ElectricDreamsButton *cancelButton;
    
    float messageBoxWidth;
    
    void createAndFadeInLayer();
    void addListenerToBackgroundLayer();
    
    //MessageBox functions
    void createMessageWithSingleButton(std::string messageTitle, std::string messageBody, std::string buttonText);
    void createTitle(std::string messageTitle);
    void underlineTitle(cocos2d::Label* titleLabel);
    void createBody(std::string messageBody);
    void createButton(std::string buttonText);
    void createMessageBackground();
    
    void removeSelf(float dt);
    
public:
    static Layer* createMessageBox(std::string Title, std::string Body, std::string buttontext);
    
    virtual bool init();
    
    CREATE_FUNC(MultiButtonMessageBoxLayer);
    
    CC_SYNTHESIZE(MultiButtonMessageBoxLayer*, _delegate, Delegate);
    
    //Delegate Functions
    void buttonPressed(ElectricDreamsButton* button);
};

#endif
