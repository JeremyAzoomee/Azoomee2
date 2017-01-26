
#include "cocos2d.h"
#include "ElectricDreamsButton.h"

//------------- ERROR MESSAGE CODES --------------
// For use in createErrorMessage
#define ERROR_CODE_INVALID_CREDENTIALS 1001
#define ERROR_CODE_ANY_OTHER_LOGIN_ERROR 1002
#define ERROR_CODE_EMAIL_VARIFICATION_REQUIRED 1003
#define ERROR_CODE_INCORRECT_PIN 1004
#define ERROR_CODE_NAME_EXISTS 1005

class ModalMessages: public cocos2d::Ref, public ElectricDreamsButtonDelegate
{
public:
    /** Returns the shared instance of the Game Manager */
    static ModalMessages* getInstance(void);
    
private:
    void createAndFadeInLayer();
    void addListenerToBackgroundLayer();
    void removeLayer();
    
    void createMessageWithSingleButton(std::string messageTitle, std::string messageBody, std::string buttonText);
    
    //MessageBox functions
    void createTitle(std::string messageTitle);
    void underlineTitle(cocos2d::Label* titleLabel);
    void createBody(std::string messageBody);
    void createButton(std::string buttonText);
    void createMessageBackground();
    
    cocos2d::LayerColor *loadingLayer;
    
    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;
    
    float messageBoxWidth;
    
public:
    
    virtual ~ModalMessages();
    bool init(void);
    
    void startLoading();
    void stopLoading();
    
    void createErrorMessage(long errorCode);
    
    //Delegate Functions
    void buttonPressed(ElectricDreamsButton* button);

};
