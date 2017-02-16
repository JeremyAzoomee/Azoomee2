#ifndef __ELECTRIC_DREAM_BUTTON_H__
#define __ELECTRIC_DREAM_BUTTON_H__

#include "cocos2d.h"
#include <ui/UIScale9Sprite.h>


USING_NS_CC;

class ElectricDreamsButton;

class ElectricDreamsButtonDelegate
{
public:
    
    virtual void buttonPressed(ElectricDreamsButton* button) = 0;
};

class ElectricDreamsButton : public Layer
{
private:
    void addListener();
    ui::Scale9Sprite *createButtonBackground(std::string buttonText);
    
    void callDelegateFunction(float dt);
    
    bool isSettingsButton;
    
    std::string buttonAudioFile;
    std::string mixPanelButtonName = "";
    
    void sendMixPanelEvent();
    
public:
    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(ElectricDreamsButton);
    
    CC_SYNTHESIZE(ElectricDreamsButtonDelegate*, _delegate, Delegate);
    
    static ElectricDreamsButton* createTextAsButton(std::string buttonText);
    static ElectricDreamsButton* createButtonWithText(std::string buttonText);
    static ElectricDreamsButton* createNextButton();
    static ElectricDreamsButton* createBackButton();
    static ElectricDreamsButton* createAcceptButton();
    static ElectricDreamsButton* createCancelButton();
    static ElectricDreamsButton* createSettingsButton(float creationDelay);
    
    void setCenterPosition(Vec2 position);
    Vec2 getCenterPosition();
    void setMixPanelButtonName(std::string buttonName);
    
};

#endif
