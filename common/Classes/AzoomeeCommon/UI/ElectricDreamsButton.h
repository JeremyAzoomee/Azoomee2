#ifndef AzoomeeCommon_ElectricDreamsButton_h
#define AzoomeeCommon_ElectricDreamsButton_h

#include <cocos/cocos2d.h>
#include <cocos/ui/UIScale9Sprite.h>


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
    static ElectricDreamsButton* createSecondaryGreenButton(std::string buttonText, float textPadding, float minWidth);
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
    static ElectricDreamsButton* createButtonWithText(std::string buttonText, float textPadding);
    static ElectricDreamsButton* createNextButton();
    static ElectricDreamsButton* createBackButton();
    static ElectricDreamsButton* createWindowCloseButton();
    
    void setCenterPosition(cocos2d::Vec2 position);
    cocos2d::Vec2 getCenterPosition();
    void setMixPanelButtonName(std::string buttonName);
    
    void setEnabled(bool isEnabled);
    
};
  
}

#endif
