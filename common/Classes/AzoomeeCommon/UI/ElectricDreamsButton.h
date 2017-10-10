#ifndef AzoomeeCommon_ElectricDreamsButton_h
#define AzoomeeCommon_ElectricDreamsButton_h

#include <cocos/cocos2d.h>
#include <cocos/ui/UIScale9Sprite.h>
#include "OomeeButtonLayer.h"


namespace Azoomee
{

class ElectricDreamsButton;

class ElectricDreamsButtonDelegate
{
public:
    
    virtual void buttonPressed(ElectricDreamsButton* button) = 0;
};

class ElectricDreamsButton : public cocos2d::Layer
{
private:
    void callDelegateFunction(float dt);
    
    cocos2d::Sprite* spriteButton;
    std::string buttonAudioFile;
    OomeeButtonLayer* oomeeLayer;
    
    bool buttonEnabled;
    std::string buttonEnabledImage;
    std::string buttonDisablednabledImage;
  
protected:
    std::string mixPanelButtonName = "";
    
    virtual bool init();
    virtual void onButtonPressed();
    
    void addListener();
    static ElectricDreamsButton* createPrimaryButton(std::string buttonText, float textPadding, float minWidth);
    static ElectricDreamsButton* createPrimaryGreenButton(std::string buttonText, float textPadding, float minWidth);
    static ElectricDreamsButton* createSecondaryButton(std::string buttonText, float textPadding, float minWidth);
    cocos2d::ui::Scale9Sprite *createButtonOutline(std::string buttonText);
    void sendMixPanelEvent();
    cocos2d::Sprite* createSpriteButton(std::string buttonImage, std::string buttonAudio);
    static ElectricDreamsButton* createSmallSprite9Button(std::string buttonText, float buttonWidth,std::string sprite9, cocos2d::Color3B textColor);
  
public:
    
    // implement the "static create()" method manually
    CREATE_FUNC(ElectricDreamsButton);
    
    CC_SYNTHESIZE(ElectricDreamsButtonDelegate*, _delegate, Delegate);
    
    static ElectricDreamsButton* createTextAsButton(std::string buttonText);
    static ElectricDreamsButton* createTextAsButton(std::string buttonText, float fontSize, bool underlined);
    static ElectricDreamsButton* createTextAsButtonAqua(std::string buttonText, float fontSize, bool underlined);
    static ElectricDreamsButton* createTextAsButtonWithColor(std::string buttonText, float fontSize, bool underlined, cocos2d::Color3B color);
    static ElectricDreamsButton* createButtonWithText(std::string buttonText);
    static ElectricDreamsButton* createButtonWithWidth(std::string buttonText, float buttonWidth);
    static ElectricDreamsButton* createGreenButtonWithWidth(std::string buttonText, float buttonWidth);
    static ElectricDreamsButton* createSecondaryButtonWithWidth(std::string buttonText, float buttonWidth);
    static ElectricDreamsButton* createButtonWithText(std::string buttonText, float textPadding);
    static ElectricDreamsButton* createOutlineButtonWithText(std::string buttonText);
    static ElectricDreamsButton* createNextButton();
    static ElectricDreamsButton* createBackButton();
    static ElectricDreamsButton* createAcceptButton();
    static ElectricDreamsButton* createCancelButton();
    static ElectricDreamsButton* createWindowCloseButton();
    static ElectricDreamsButton* createWindowCloseButtonGreen();
    static ElectricDreamsButton* createWhiteWindowCloselButton();
    static ElectricDreamsButton* createAddButton();
    static ElectricDreamsButton* createPlaceHolderButton();
    static ElectricDreamsButton* createPlaceHolderButton(float withWidth);
    static ElectricDreamsButton* createChildSelectorButton();
    static ElectricDreamsButton* createTabButton(std::string buttonText);
    static ElectricDreamsButton* createChevronLeftButton();
    static ElectricDreamsButton* createChevronRightButton();
    
    //----------- CHAT INVITE BUTTONS -------------------
    
    static ElectricDreamsButton* createKidCodeShareButton(std::string kidCode, float buttonWidth);
    static ElectricDreamsButton* createGreenButton(std::string buttonText, float buttonWidth);
    static ElectricDreamsButton* createTextInputAsButton(std::string buttonText, float buttonWidth);
    static ElectricDreamsButton* createSendButton();
    static ElectricDreamsButton* createRedOutlineButton(std::string buttonText, float buttonWidth);
    static ElectricDreamsButton* createRedFilledButton(std::string buttonText, float buttonWidth);
    
    // ---------- OOMEE BUTTONS AND FUNCTIONS------------
    static ElectricDreamsButton* createOomeeButtonWithOutline(int oomeeNumber, std::string oomeeName);
    static ElectricDreamsButton* createOomeeAsButton(int oomeeNumber);
    void playOomeeAnimation(std::string OomeeAnimation, bool loop);
    void playOomeeAnimationNoSound(std::string OomeeAnimation);
    void hideOomee();
    
    void setCenterPosition(cocos2d::Vec2 position);
    cocos2d::Vec2 getCenterPosition();
    void setMixPanelButtonName(std::string buttonName);
    
    void setEnabled(bool isEnabled);
    
};
  
}

#endif
