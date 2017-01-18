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
    Label* messageTitleLabel;
    Label* messageBodyLabel;
    std::vector<ElectricDreamsButton*> buttonsList;
    
    float messageBoxWidth;
    
    void createAndFadeInLayer();
    void addListenerToBackgroundLayer();
    
    //MessageBox functions
    void createTitle(std::string messageTitle);
    void underlineTitle(cocos2d::Label* titleLabel);
    void createBody(std::string messageBody);
    void createButtons(std::vector<std::string> buttonTitleList);
    void createMessageBackground();
    
    void removeSelf(float dt);
    
public:
    static Layer* createMessageBox(std::string Title, std::string Body, std::vector<std::string> buttonTitleList, MultiButtonMessageBoxLayerDelegate* _delegate);
    
    virtual bool init();
    
    CREATE_FUNC(MultiButtonMessageBoxLayer);
    
    CC_SYNTHESIZE(MultiButtonMessageBoxLayerDelegate*, _delegate, Delegate);
    
    //Delegate Functions
    void buttonPressed(ElectricDreamsButton* button);
};

#endif
