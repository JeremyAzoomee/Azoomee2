
#include "cocos2d.h"
#include "ElectricDreamsButton.h"
#include "MultiButtonMessageBoxLayer.h"
#include "TextInputLayer.h"

//------------- ERROR MESSAGE CODES --------------
// For use in createErrorMessage
#define ERROR_CODE_INVALID_CREDENTIALS 1001
#define ERROR_CODE_ANY_OTHER_LOGIN_ERROR 1002
#define ERROR_CODE_EMAIL_VARIFICATION_REQUIRED 1003
#define ERROR_CODE_INCORRECT_PIN 1004
#define ERROR_CODE_NAME_EXISTS 1005
#define ERROR_CODE_BAD_TIME_SETTING 2001

class ModalMessages: public cocos2d::Ref, public ElectricDreamsButtonDelegate, public MultiButtonMessageBoxLayerDelegate
{
public:
    /** Returns the shared instance of the Game Manager */
    static ModalMessages* getInstance(void);
    
private:
    void createAndFadeInLayer();
    void addListenerToBackgroundLayer();
    void removeLayer();
    
    //MessageBox functions
    void createTitle(std::string messageTitle);
    void underlineTitle(cocos2d::Label* titleLabel);
    void createBody(std::string messageBody);
    void createButton(std::string buttonText);
    void createMessageBackground();
    
    cocos2d::LayerColor *loadingLayer;
    
    cocos2d::Size visibleSize;
    cocos2d::Vec2 origin;
    
    std::string somethingWentWrongTitle;
    std::string somethingWentWrongButton;
    
    float messageBoxWidth;
    
    std::vector<std::string> messageBoxButtonTitles;
    
    //-------Object To Hide -------
    // Due to UIEditbox always being on top
    
    TextInputLayer* savedTextInputToHide;
    void hideTextInput(TextInputLayer* textInputToHide);
    void UnHideTextInput();
    
public:
    
    virtual ~ModalMessages();
    bool init(void);
    
    void startLoading();
    void stopLoading();
    
    void createErrorMessage(long errorCode, TextInputLayer* textInputToHide);
    void createMessageWithSingleButton(std::string messageTitle, std::string messageBody, std::string buttonText, TextInputLayer* textInputToHide);
    void createPreviewLoginSignupMessageBox();
    void createSomethingWentWrongMessage();
    
    //Delegate Functions
    void buttonPressed(ElectricDreamsButton* button);
    void MultiButtonMessageBoxPressed(std::string messageBoxTitle,std::string buttonTitle);

};
