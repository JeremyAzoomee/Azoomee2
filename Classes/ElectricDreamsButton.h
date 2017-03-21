#ifndef __ELECTRIC_DREAM_BUTTON_H__
#define __ELECTRIC_DREAM_BUTTON_H__

#include "cocos2d.h"
#include <ui/UIScale9Sprite.h>
#include "OomeeButtonLayer.h"

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
    static ElectricDreamsButton* createPrimaryButton(std::string buttonText, float textPadding, float minWidth);
    static ElectricDreamsButton* createSecondaryButton(std::string buttonText, float textPadding, float minWidth);
    ui::Scale9Sprite *createButtonOutline(std::string buttonText);
    
    void callDelegateFunction(float dt);
    
    bool isSettingsButton;
    
    std::string buttonAudioFile;
    std::string mixPanelButtonName = "";
    OomeeButtonLayer* oomeeLayer;
    
    void sendMixPanelEvent();
    
    Sprite* createSpriteButton(std::string buttonImage, std::string buttonAudio);
    
public:
    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(ElectricDreamsButton);
    
    CC_SYNTHESIZE(ElectricDreamsButtonDelegate*, _delegate, Delegate);
    
    static ElectricDreamsButton* createTextAsButton(std::string buttonText);
    static ElectricDreamsButton* createTextAsButton(std::string buttonText, float fontSize, bool underlined);
    static ElectricDreamsButton* createButtonWithText(std::string buttonText);
    static ElectricDreamsButton* createButtonWithWidth(std::string buttonText, float buttonWidth);
    static ElectricDreamsButton* createSecondaryButtonWithWidth(std::string buttonText, float buttonWidth);
    static ElectricDreamsButton* createButtonWithText(std::string buttonText, float textPadding);
    static ElectricDreamsButton* createOutlineButtonWithText(std::string buttonText);
    static ElectricDreamsButton* createNextButton();
    static ElectricDreamsButton* createBackButton();
    static ElectricDreamsButton* createAcceptButton();
    static ElectricDreamsButton* createCancelButton();
    static ElectricDreamsButton* createWindowCloselButton();
    static ElectricDreamsButton* createSettingsButton(float creationDelay);
    static ElectricDreamsButton* createAddButton();
    
    // ---------- OOMEE BUTTONS AND FUNCTIONS------------
    static ElectricDreamsButton* createOomeeButtonWithOutline(int oomeeNumber, std::string oomeeName);
    static ElectricDreamsButton* createOomeeAsButton(int oomeeNumber);
    void playOomeeAnimation(std::string OomeeAnimation, bool loop);
    void playOomeeAnimationNoSound(std::string OomeeAnimation);
    void hideOomee();
    
    void setCenterPosition(Vec2 position);
    Vec2 getCenterPosition();
    void setMixPanelButtonName(std::string buttonName);
    
};

#endif
