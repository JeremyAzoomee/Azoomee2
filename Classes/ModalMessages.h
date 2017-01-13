
#include "cocos2d.h"
#include "ElectricDreamsButton.h"

class ModalMessages: public cocos2d::Ref, public ElectricDreamsButtonDelegate
{
public:
    /** Returns the shared instance of the Game Manager */
    static ModalMessages* getInstance(void);
    
private:
    void createAndFadeInLayer();
    void addListenerToBackgroundLayer();
    void removeLayer();
    
    //MessageBox functions
    void createAndFadeInTitle(std::string messageTitle);
    void underlineTitle(cocos2d::Label* titleLabel);
    void createAndFadeInBody(std::string messageBody);
    void createAndFadeInButton(std::string buttonText);
    void createAndFadeInMessageBackground();
    
    cocos2d::LayerColor *loadingLayer;
    
    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;
    
    float messageBoxWidth;
    
public:
    
    virtual ~ModalMessages();
    bool init(void);
    
    void startLoading();
    void stopLoading();
    
    void createMessageWithSingleButton(std::string messageTitle, std::string messageBody, std::string buttonText);
    
    //Delegate Functions
    void buttonPressed(ElectricDreamsButton* button);

};
