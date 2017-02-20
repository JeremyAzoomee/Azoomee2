#ifndef __MULTI_BUTTON_MESSAGEBOX_LAYER_H__
#define __MULTI_BUTTON_MESSAGEBOX_LAYER_H__

#include "cocos2d.h"
#include "ElectricDreamsButton.h"
#include "TextInputLayer.h"

USING_NS_CC;

//------------- ERROR MESSAGE CODES --------------
// For use in createWith(ErrorCode)
#define ERROR_CODE_INVALID_CREDENTIALS 1001
#define ERROR_CODE_ANY_OTHER_LOGIN_ERROR 1002
#define ERROR_CODE_EMAIL_VARIFICATION_REQUIRED 1003
#define ERROR_CODE_INCORRECT_PIN 1004
#define ERROR_CODE_NAME_EXISTS 1005
#define ERROR_CODE_BAD_TIME_SETTING 2001

/*------- NOTES FOR USAGE -------

In .h file, add the delegate definition.

    public MultiButtonMessageBoxLayerDelegate
 
and add in the public functions of the .h file, declare the delegate function.

    void MultiButtonMessageBoxPressed(std::string messageBoxTitle,std::string buttonTitle);
 
In the cpp to call the multimessagebox, create a vector of button titles.
Then call the CreateMessageBox giving the message details,
Pass the function 'this' so it can assign this class as its delegate.
 
    std::vector<std::string> buttonTitles;
    buttonTitles.push_back("b1");
    buttonTitles.push_back("b2");
    buttonTitles.push_back("b3");
 
    MultiButtonMessageBoxLayer::createMessageBox("title", "body", buttonTitles, this);

Define your delegate function, the message box title and button title will be given back.
 
void <YOUR CLASS>::MultiButtonMessageBoxPressed(std::string messageBoxTitle,std::string buttonTitle)
 
*/

class MessageBox;

class MessageBoxDelegate
{
public:
    virtual void MessageBoxButtonPressed(std::string messageBoxTitle,std::string buttonTitle) = 0;
};

class MessageBox : public Layer, public ElectricDreamsButtonDelegate
{
private:

    cocos2d::Size visibleSize;
    Vec2 origin;
    
    std::vector<std::string> _buttonsTitleList;
    std::string _messageBoxTitle;
    
    LayerColor *backgroundLayer;
    Label* messageTitleLabel;
    Label* messageBodyLabel;
    std::vector<ElectricDreamsButton*> buttonsList;
    
    float messageBoxWidth;
    
    void createAndFadeInLayer();
    void addListenerToBackgroundLayer();
    
    //MessageBox functions
    void createTitle();
    void underlineTitle();
    void createBody(std::string messageBody);
    void createButtons();
    void positionButtonsBasedOnWidth(float totalButtonsWidth);
    void createMessageBackground();
    
    void removeSelf(float dt);
    
    //-------Object To Hide -------
    // Due to UIEditbox always being on top
    TextInputLayer* savedTextInputToHide;
    void hideTextInput(TextInputLayer* textInputToHide);
    void UnHideTextInput();
    
public:
    //Main function for creating a multiButtonMessageBox
    static Layer* createWith(std::string Title, std::string Body, std::vector<std::string> buttonTitleList, MessageBoxDelegate* _delegate);
    static Layer* createWith(std::string Title, std::string Body, std::string Button, MessageBoxDelegate* _delegate);
    static Layer* createWith(long errorCode, MessageBoxDelegate* _delegate);
    static Layer* createWith(long errorCode, TextInputLayer* textInputToHide, MessageBoxDelegate* _delegate);
    
    void initMessageBoxLayer(std::string Title, std::string Body, MessageBoxDelegate* _delegate);
    
    virtual bool init();
    
    CREATE_FUNC(MessageBox);
    
    CC_SYNTHESIZE(MessageBoxDelegate*, _delegate, Delegate);
    
    //Delegate Functions
    void buttonPressed(ElectricDreamsButton* button);
};

#endif
